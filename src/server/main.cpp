#include <boost/asio.hpp>
#include <boost/functional/overloaded_function.hpp>
#include <config/lib.hpp>
#include <network/game.hpp>
#include <network/tcp_server.hpp>

int main(int argc, char* argv[]) {
  auto config = get_config();

  auto game = Game();

  auto accept_handler = [&game](ClientID client_id, Server& server) {
    // assign client their player_id
    int new_player_id = game.create_player();
    server.write<message::Assign>(client_id, new_player_id);

    // notify everyone a new player has joined
    // TODO: don't notify newly joined client
    std::string notification =
        "Client " + boost::uuids::to_string(client_id) + " has joined";
    server.write_all<message::Notify>(notification);
  };

  auto read_handler = [&game](const message::Message& m, Server& server) {
    ClientID client_id = m.metadata.id;

    auto greeting_handler = [&server,
                             &client_id](const message::Greeting& body) {
      std::string greeting =
          "Hello, client " + boost::uuids::to_string(client_id) + "!";
      server.write<message::Greeting>(client_id, greeting);
    };

    auto user_state_update_handler =
        [&game](const message::UserStateUpdate& body) { game.update(body); };

    auto any_handler = [](const message::Message::Body&) {};

    auto message_handler = boost::make_overloaded_function(
        greeting_handler, user_state_update_handler, any_handler);
    boost::apply_visitor(message_handler, m.body);
  };

  auto write_handler = [](std::size_t bytes_transferred,
                          const message::Message& m, Server& server) {};

  auto tick_handler = [&game](Server& server) {
    // advance game, send update to client
    game.tick();
    auto things = game.to_network();
    server.write_all<message::GameStateUpdate>(things);
  };

  boost::asio::io_context io_context;
  Server server(io_context, config["server_port"], accept_handler, read_handler,
                write_handler, tick_handler);
  io_context.run();

  return 0;
}
