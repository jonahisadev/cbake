#include "Cookbook.h"
#include <string>

using JSON = nlohmann::json;

Cookbook::Cookbook(fs::path config_path, bool valid)
{
    _config_path = config_path;
    _valid = valid;

    // Load JSON
    if (_valid) {
        JSON doc;
        std::ifstream in(_config_path);
        in >> doc;
        in.close();

        // Load recipes
        for (auto& r : doc["recipes"]) {
            _recipes.emplace_back(
                r["name"].get<std::string>().c_str(), 
                r["url"].get<std::string>().c_str(),
                r["sha"].get<std::string>().c_str()
            );
        }

        _recipe_path = fs::path(doc["recipe_path"].get<std::string>());
    }
}

const Recipe& Cookbook::addRecipe(String url)
{
    git_libgit2_init();
    fs::create_directory(_recipe_path);
    
    auto args = url.split('/');
    auto name = args.last();
    name = name.subs(0, name.indexOf('.'));

    // Clone repo
    git_repository* repo = nullptr;
    const char* c_url = url.c_str();
    int error = git_clone(&repo, c_url, (String(_recipe_path.c_str()) + "/" + name).c_str(), nullptr);
    if (error < 0) {
        const git_error* e = git_error_last();
        std::cout << e->message << std::endl;
        throw CBakeException("Could not clone given git repository");
    }

    // Get latest SHA
    git_oid id;
    error = git_reference_name_to_id(&id, repo, "HEAD");
    if (error < 0) {
        const git_error* e = git_error_last();
        std::cout << e->message << std::endl;
        throw CBakeException("Could not resolve repo HEAD");
    }
    char sha[40];
    git_oid_fmt(sha, &id);

    _recipes.emplace_back(name, url, sha);
    save();

    git_libgit2_shutdown();
    return _recipes.last();
}

void Cookbook::save()
{
    // Read current doc
    JSON doc;
    std::ifstream in(_config_path);
    in >> doc;
    in.close();

    // Save recipes
    if (doc.count("recipes") > 0)
        doc.erase("recipes");
    doc["recipes"] = JSON::array();
    for (auto recipe : _recipes) {
        doc["recipes"].push_back({
            { "name", recipe.name().c_str() },
            { "url", recipe.url().c_str() },
            { "sha", recipe.sha().c_str() }
        });
    }

    // Write
    std::ofstream out(_config_path);
    out << doc.dump(4);
    out.close();
}

void Cookbook::install()
{
    fs::create_directory(_recipe_path);

    for (auto& r : _recipes) {
        switch (r.type()) {
            case Recipe::Type::Git:
                install_git(r);
                break;
        }
    }
}

void Cookbook::install_git(const Recipe& recipe)
{
    git_libgit2_init();
    
    // Clone repo
    git_repository* repo = nullptr;
    git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
    clone_opts.fetch_opts.callbacks.transfer_progress = fetch_progress;
    const char* c_url = recipe.url().c_str();
    int error = git_clone(&repo, c_url, (String(_recipe_path.c_str()) + "/" + recipe.name()).c_str(), nullptr);
    if (error < 0) {
        const git_error* e = git_error_last();
        std::cout << e->message << std::endl;
        throw CBakeException("Could not clone given git repository");
    }

    // Get ID from given SHA
    git_oid id;
    error = git_oid_fromstr(&id, recipe.sha().c_str());
    if (error < 0) {
        const git_error* e = git_error_last();
        std::cout << e->message << std::endl;
        throw CBakeException("Could not convert SHA to commit ID");
    }

    // Get commit from SHA
    git_commit* commit = nullptr;
    error = git_commit_lookup(&commit, repo, &id);
    if (error < 0) {
        const git_error* e = git_error_last();
        std::cout << e->message << std::endl;
        throw CBakeException("Could not find given commit");
    }

    // Checkout given commit
    git_checkout_options opts = GIT_CHECKOUT_OPTIONS_INIT;
    error = git_checkout_tree(repo, (const git_object*)commit, &opts);
    if (error < 0) {
        const git_error* e = git_error_last();
        std::cout << e->message << std::endl;
        throw CBakeException("Could not checkout given commit");
    }

    git_libgit2_shutdown();
}

bool Cookbook::check_for_existing()
{
    return fs::exists(fs::current_path()/"cookbook.json");
}

void Cookbook::init(String name, String version, String author, String path)
{
    auto config_path = fs::path(fs::current_path()/"cookbook.json");

    JSON config;
    config["name"] = name.c_str();
    config["version"] = version.c_str();
    config["author"] = author.c_str();
    config["recipe_path"] = path.c_str();

    std::ofstream out(config_path.c_str());
    out << config.dump(4);
    out.close();
}

Cookbook Cookbook::load()
{
    if (!check_for_existing()) {
        return Cookbook(fs::path(), false);
    }

    return Cookbook(fs::path(fs::current_path()/"cookbook.json"), true);
}