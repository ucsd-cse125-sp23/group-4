#include <boost/container_hash/hash.hpp>
#include <boost/functional/overloaded_function.hpp>
#include <config/lib.hpp>
#include <network/tcp_server.hpp>
#include <server/manager.hpp>

int main(int argc, char* argv[]) {
  auto manager = Manager();
  std::unordered_map<ClientID, int, boost::hash<ClientID>> pids;

  auto accept_handler = [&manager, &pids](ClientID client_id, Server& server) {
    // assign client their player_id
    int pid = manager.add_player();
    pids[client_id] = pid;
    server.write<message::Assign>(client_id, pid);

    // notify everyone a new player has joined
    // TODO: don't notify newly joined client
    std::string notification =
        "Client " + boost::uuids::to_string(client_id) + " has joined";
    server.write_all<message::Notify>(notification);
  };

  auto close_handler = [&manager, &pids](ClientID client_id, Server& server) {
    int pid = pids.at(client_id);
    manager.remove_player(pid);
  };

  auto read_handler = [&manager](const message::Message& m, Server& server) {
    ClientID client_id = m.metadata.id;

    auto greeting_handler = [&server,
                             &client_id](const message::Greeting& body) {
      std::string greeting =
          "Hello, client " + boost::uuids::to_string(client_id) + "!";
      server.write<message::Greeting>(client_id, greeting);
    };

    auto user_state_update_handler =
        [&manager](const message::UserStateUpdate& body) {
          manager.handle_game_update(body);
        };

    auto any_handler = [](const message::Message::Body&) {};

    auto message_handler = boost::make_overloaded_function(
        greeting_handler, user_state_update_handler, any_handler);
    boost::apply_visitor(message_handler, m.body);
  };

  auto write_handler = [](std::size_t bytes_transferred,
                          const message::Message& m, Server& server) {};

  auto tick_handler = [&manager](Server& server) {
    // advance game, send update to client
    manager.tick_game();
    auto update = manager.get_game_update();
    server.write_all<message::GameStateUpdate>(update);
  };

  auto config = get_config();
  Server server(config["server_port"], accept_handler, close_handler,
                read_handler, write_handler, tick_handler);

  return 0;
}
