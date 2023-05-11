#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

json get_config();
