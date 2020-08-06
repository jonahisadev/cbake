#include <iostream>
#include <fstream>
#include <string>

#define JA_IMPL
#include "JA/JA.h"
#include "JA/string.h"

#include "cxxopts.hpp"
#include "git2/global.h"
#include "git2/clone.h"
#include "git2/errors.h"

#include "Cookbook.h"

int main(int argc, char** argv)
{
    // Arguments
    cxxopts::Options options("cbake", "A tool to aid in managing C/C++ project dependencies");
    options.add_options()
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
    if (cmd == "recipe") {

        git_libgit2_init();

        // Add recipe
        if (opts["add"].count() > 0) {
            auto url = String(opts["add"].as<std::string>().c_str());
            auto args = url.split('/');
            auto name = args[args.size() - 1];
            name = name.subs(0, name.indexOf('.'));

            // Clone repo
            git_repository* repo = nullptr;
            const char* c_url = url.c_str();
            int error = git_clone(&repo, c_url, name.c_str(), nullptr);
            if (error < 0) {
                const git_error* e = git_error_last();
                std::cout << e->message << std::endl;
                return -1;
            }

            std::cout << "Added recipe! (" << name << ")" << std::endl;
        }

        git_libgit2_shutdown();
    }

    else if (cmd == "init") {
        if (Cookbook::check_for_existing()) {
            std::cerr << "There is already a cookbook.json file!" << std::endl;
            return -1;
        }

        String name;
        String version;
        String author;

        std::cout << "Project name: ";
        std::cin >> name;
        std::cout << "Version: ";
        std::cin >> version;
        std::cout << "Author: ";
        std::cin >> author;

        Cookbook::init(name, version, author);
    }

    return 0;
}