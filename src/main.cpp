#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;

#define JA_IMPL
#include "JA/JA.h"
#include "JA/string.h"

#include "cxxopts.hpp"

#include "Common.h"
#include "Cookbook.h"

int main(int argc, char** argv)
{
    // Arguments
    cxxopts::Options options("cbake", "A tool to aid in managing C/C++ project dependencies");
    options.add_options()
        ("skip", "Skip a process", cxxopts::value<bool>())
        ("add", "Add", cxxopts::value<std::string>())
        ("help", "Show help", cxxopts::value<std::string>());
    auto opts = options.parse(argc, argv);

    // Show help
    if (opts["help"].count() > 0 || argc < 2) {
        std::cerr << options.help() << std::endl;
        return -1;
    }

    // Commands
    std::string cmd(argv[1]);

    // Recipe
    if (cmd == "recipe") {
        Cookbook c = Cookbook::load();
        if (!c) {
            std::cout << "No cookbook.json file to write to; please init" << std::endl;
            return -1;
        }

        // Add recipe
        if (opts["add"].count() > 0) {
            auto r = c.addRecipe(String(opts["add"].as<std::string>().c_str()));
            std::cout << String("Added recipe '{}'").arg(r.name()) << std::endl;
        }
    }

    // Install given recipes
    else if (cmd == "install")
    {
        Cookbook c = Cookbook::load();
        if (!c) {
            std::cout << "No cookbook.json" << std::endl;
            return -1;
        }

        c.install();
    }

    // Initialize
    else if (cmd == "init") {
        if (Cookbook::check_for_existing()) {
            std::cerr << "There is already a cookbook.json file!" << std::endl;
            return -1;
        }

        // Default project values
        String name = String(fs::current_path().c_str()).split('/').last().c_str();
        String version = "1.0";
        String author = USER;
        String path = "recipes";

        if (opts["skip"].count() == 0) {
            std::string buf;

            std::cout << String("Project name ({}): ").arg(name);
            std::getline(std::cin, buf);
            if (!buf.empty())
                name = buf.c_str();

            std::cout << String("Version ({}): ").arg(version);
            std::getline(std::cin, buf);
            if (!buf.empty())
                version = buf.c_str();

            std::cout << String("Author ({}): ").arg(author);
            std::getline(std::cin, buf);
            if (!buf.empty())
                author = buf.c_str();

            std::cout << String("Recipe path ({}): ").arg(path);
            std::getline(std::cin, buf);
            if (!buf.empty())
                path = buf.c_str();
        }

        Cookbook::init(name, version, author, path);
    }

    // Unknown
    else {
        std::cerr << options.help() << std::endl;
        std::cerr << String("Unknown command '{}'").arg(cmd.c_str()) << std::endl;
        return -1;
    }

    return 0;
}