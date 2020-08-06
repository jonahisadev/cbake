#include "catch.hpp"

#include "../include/JA.h"
#include "../include/JA/string.h"

#include <type_traits>

static String str = "Hello world!";

TEST_CASE("String::length()", "[string]") {
    REQUIRE(str.length() == 12);
}

TEST_CASE("String::startsWith()", "[string]") {
    REQUIRE(str.startsWith("H") == true);
    REQUIRE(str.startsWith("Hello") == true);
    REQUIRE(str.startsWith("Hello world!") == true);
    REQUIRE(str.startsWith("hello") == false);
}

TEST_CASE("String::endsWith()", "[string]") {
    REQUIRE(str.endsWith("!") == true);
    REQUIRE(str.endsWith("world!") == true);
    REQUIRE(str.endsWith("Hello world!") == true);
    REQUIRE(str.endsWith("world") == false);
}

TEST_CASE("String::equals()", "[string]") {
    REQUIRE(str.equals("Hello world!"));
    REQUIRE(!str.equals("Hello world! "));
    REQUIRE(!str.equals("Anything else"));
    REQUIRE(str == "Hello world!");
    REQUIRE(str != "Hello world! ");
}

TEST_CASE("String::contains()", "[string]") {
    REQUIRE(str.contains("Hello") == true);
    REQUIRE(str.contains("world") == true);
    REQUIRE(str.contains("Hello world!") == true);
    REQUIRE(str.contains("o w") == true);
    REQUIRE(str.contains("hello world!") == false);
}

TEST_CASE("String::subs()", "[string]") {
    REQUIRE(str.subs(0, 5) == "Hello");
    REQUIRE(str.subs(0, 100) == "Hello world!");
}

TEST_CASE("String::indexOf()", "[string]") {
    REQUIRE(str.indexOf('H') == 0);
    REQUIRE(str.indexOf('o') == 4);
    REQUIRE(str.indexOf('o', 2) == 7);
    REQUIRE(str.indexOf('z') == str.length());
}

TEST_CASE("String::toLowerCase()", "[string]") {
    String copy = str;
    REQUIRE(copy.toLowerCase() == "hello world!");
}

TEST_CASE("String::toUpperCase()", "[string]") {
    String copy = str;
    REQUIRE(copy.toUpperCase() == "HELLO WORLD!");
}

TEST_CASE("String::append()", "[string]") {
    String copy = str;
    REQUIRE(copy.append("!!") == "Hello world!!!");
    REQUIRE(copy.append("!!") == "Hello world!!!!!");
    REQUIRE(copy + "..." == "Hello world!!!!!...");
    const String& safe = str.safe();
    REQUIRE(safe.append("!!") == "Hello world!!!");
    REQUIRE(safe.append("!!") == "Hello world!!!");
    REQUIRE(safe + "!!" == "Hello world!!!");
}

TEST_CASE("String::arg()", "[string]") {
    REQUIRE(String("My name is %").arg("Lucas") == "My name is Lucas");
    REQUIRE(String("I have % dogs").arg(42) == "I have 42 dogs");
    REQUIRE(String("name: %, age: %").arg("Doug").arg(35) == "name: Doug, age: 35");
}

TEST_CASE("String::reverse()", "[string]") {
    String copy = str;
    REQUIRE(copy.reverse() == "!dlrow olleH");
    REQUIRE(str.safe().reverse() == "!dlrow olleH");
    REQUIRE(str == "Hello world!");
}

TEST_CASE("String::split()", "[string]") {
    String s = "1,2,3,4";
    auto list = s.split(',');

    REQUIRE(s == "1,2,3,4");
    REQUIRE(list.size() == 4);
    REQUIRE(list[0] == "1");
    REQUIRE(list[1] == "2");
    REQUIRE(list[2] == "3");
    REQUIRE(list[3] == "4");
}
