#include "catch.hpp"
#include <fstream>
#include <memory>
#include "tokenizer.hpp"

TEST_CASE("Tokenizer creation", "[tokenizer]") {
    auto tokenizer = std::make_shared<castor::Tokenizer>();
    REQUIRE(!tokenizer->is_loaded());
}

TEST_CASE("Tokenizer load with invalid path", "[tokenizer]") {
    auto tokenizer = std::make_shared<castor::Tokenizer>();
    bool result = tokenizer->load("nonexistent/path/tokenizer.json");
    REQUIRE(!result);
}

TEST_CASE("Tokenizer load with valid file", "[tokenizer]") {
    std::ofstream test_file("tests/test_tokenizer.json");
    test_file << R"({"vocab_size": 50000})";
    test_file.close();
    
    auto tokenizer = std::make_shared<castor::Tokenizer>();
    bool result = tokenizer->load("tests/test_tokenizer.json");
    REQUIRE(result);
    REQUIRE(tokenizer->is_loaded());
    REQUIRE(tokenizer->vocab_size() == 50000);
}

TEST_CASE("Tokenizer double load fails", "[tokenizer]") {
    std::ofstream test_file("tests/test_tokenizer2.json");
    test_file << R"({"vocab_size": 32000})";
    test_file.close();
    
    auto tokenizer = std::make_shared<castor::Tokenizer>();
    tokenizer->load("tests/test_tokenizer2.json");
    
    bool result = tokenizer->load("tests/test_tokenizer2.json");
    REQUIRE(!result);
}

TEST_CASE("Tokenizer encode without load fails", "[tokenizer]") {
    auto tokenizer = std::make_shared<castor::Tokenizer>();
    auto tokens = tokenizer->encode("hello world");
    REQUIRE(tokens.empty());
}

TEST_CASE("Tokenizer encode after load", "[tokenizer]") {
    std::ofstream test_file("tests/test_tokenizer3.json");
    test_file << R"({"vocab_size": 32000})";
    test_file.close();
    
    auto tokenizer = std::make_shared<castor::Tokenizer>();
    tokenizer->load("tests/test_tokenizer3.json");
    
    auto tokens = tokenizer->encode("hello world");
    REQUIRE(!tokens.empty());
    REQUIRE(tokens.size() >= 2);
}
