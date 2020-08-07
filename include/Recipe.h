#pragma once

#include "JA/JA.h"
#include "JA/string.h"

class Recipe
{
public:
    enum Type
    {
        Git
    };

private:
    String _name;
    String _url;
    String _sha;
    Type _type;

public:
    Recipe(String name, String url, String sha, Type type = Type::Git);

    const String& name() const { return _name.safe(); }
    const String& url() const { return _url.safe(); }
    const String& sha() const { return _sha.safe(); }
    const Type type() const { return _type; }
};