#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>

#include "JA/JA.h"
#include "JA/string.h"

#include "json.hpp"

namespace fs = std::filesystem;

class Cookbook
{
private:
    fs::path _config_path;

public:
    static bool check_for_existing();
    static void init(String name, String version, String author);
};