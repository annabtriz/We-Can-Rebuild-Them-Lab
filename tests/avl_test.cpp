#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/benchmark/catch_constructor.hpp>
#include <catch2/generators/catch_generators_range.hpp>

#include "../src/avl.hpp"


TEST_CASE("Insert small set and check serialize-deserialize roundtrip", "[avl]") {
    AVLTree t;
    t.insert(3);
    t.insert(4);
    t.insert(5);
    t.insert(6);
    std::string s = t.serialize();
    REQUIRE(!s.empty());

    AVLTree t2 = AVLTree::deserialize(s);
    REQUIRE(t2.serialize() == s);
}

TEST_CASE("Remove node and confirm structure updates", "[avl]") {
    AVLTree t;
    t.insert(10);
    t.insert(20);
    t.insert(30);
    t.insert(40);
    t.insert(50);
    t.insert(25);

    std::string before = t.serialize();
    REQUIRE(!before.empty());

    t.remove(30);
    std::string after = t.serialize();
    REQUIRE(after != before);

    AVLTree t3 = AVLTree::deserialize(after);
    REQUIRE(t3.serialize() == after);
}

TEST_CASE("Delete alias test", "[avl]") {
    AVLTree t;
    t.insert(2);
    t.insert(1);
    t.insert(3);
    std::string a = t.serialize();
    t.deleteValue(1);
    std::string b = t.serialize();
    REQUIRE(b != a);
    AVLTree t2 = AVLTree::deserialize(b);
    REQUIRE(t2.serialize() == b);
}

TEST_CASE("Empty tree serializes to empty string and deserializes to empty", "[avl]") {
    AVLTree t;
    REQUIRE(t.serialize() == "");
    AVLTree t2 = AVLTree::deserialize("");
    REQUIRE(t2.serialize() == "");
}