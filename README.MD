# CBake

### Intro

* Definitions
    * Recipe - Needed dependency
    * recipe.json - Configuration file for a recipe
    * cookbook.json - Configuration file for project requiring recipes
* Purpose
    The purpose of CBake is similar to that of NPM: Provide easy C/C++
    dependency management and seamless transfer of machines and team 
    members. As far as I know, there is no good cross-platform C/C++
    library/dependency management tool, and I plan on changing that.

### Building

Below are the steps for building locally

```
git clone --recursive https://github.com/jonahisadev/cbake.git
cd cbake
mkdir build && cd build
cmake ..
make
```

### Included Libraries

* cxxopts (MIT)
* nholmann's JSON library (MIT)
* libgit2 (GPLv2 w/ Linking Exception)
* JA (MIT)