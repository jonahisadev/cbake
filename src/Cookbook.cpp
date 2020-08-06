#include "Cookbook.h"

using JSON = nlohmann::json;

bool Cookbook::check_for_existing()
{
    return fs::exists(fs::current_path()/"cookbook.json");
}

void Cookbook::init(String name, String version, String author)
{
    auto config_path = fs::path(fs::current_path()/"cookbook.json");

    JSON config;
    config["name"] = name.c_str();
    config["version"] = version.c_str();
    config["author"] = author.c_str();

    std::ofstream out(config_path.c_str());
    out << config.dump(4);
    out.close();
}