#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>

#include "JA/JA.h"
#include "JA/string.h"
#include "JA/list.h"

#include "json.hpp"

#include "git2/global.h"
#include "git2/clone.h"
#include "git2/commit.h"
#include "git2/errors.h"
#include "git2/indexer.h"

#include "Common.h"
#include "Recipe.h"

namespace fs = std::filesystem;

static int fetch_progress(const git_indexer_progress* stats, void* payload)
{
    std::cout << (stats->received_objects / stats->total_objects) << "%" << std::endl;
    return 0;
}

class Cookbook
{
private:
    fs::path _config_path;
    fs::path _recipe_path;
    bool _valid;
    List<Recipe> _recipes;

    Cookbook(fs::path config_path, bool valid);

protected:
    void install_git(const Recipe& recipe);

public:
    const Recipe& addRecipe(String url);
    void save();
    void install();

    static bool check_for_existing();
    static void init(String name, String version, String author, String path);
    static Cookbook load();

    operator bool() const
    {
        return _valid;
    }
};