#include "catch.hpp"
#include <memory>
#include "engine.hpp"
#include "tokenizer.hpp"
#include "model_config.hpp"

TEST_CASE("Engine creation", "[engine]") {
    auto engine = std::make_shared<castor::Engine>();
    REQUIRE(!engine->is_initialized());
}

TEST_CASE("Engine initialization", "[engine]") {
    auto engine = std::make_shared<castor::Engine>();
    castor::ModelConfig config;
    config.model_name = "test-model";
    
    bool result = engine->initialize("dummy.plan", config);
    REQUIRE(result);
    REQUIRE(engine->is_initialized());
    REQUIRE(engine->get_config().model_name == "test-model");
}

TEST_CASE("Engine double initialization fails", "[engine]") {
    auto engine = std::make_shared<castor::Engine>();
    castor::ModelConfig config;
    
    engine->initialize("dummy.plan", config);
    bool result = engine->initialize("dummy.plan", config);
    REQUIRE(!result);
}

TEST_CASE("Engine inference without init", "[engine]") {
    auto engine = std::make_shared<castor::Engine>();
    std::vector<int32_t> input = {1, 2, 3};
    std::vector<float> output;
    
    bool result = engine->infer(input, output);
    REQUIRE(!result);
}

TEST_CASE("Engine inference after init", "[engine]") {
    auto engine = std::make_shared<castor::Engine>();
    castor::ModelConfig config;
    
    engine->initialize("dummy.plan", config);
    
    std::vector<int32_t> input = {1, 2, 3};
    std::vector<float> output;
    
    bool result = engine->infer(input, output);
    REQUIRE(result);
}

TEST_CASE("Engine tokenizer binding", "[engine]") {
    auto engine = std::make_shared<castor::Engine>();
    auto tokenizer = std::make_shared<castor::Tokenizer>();
    
    REQUIRE(engine->get_tokenizer() == nullptr);
    
    engine->set_tokenizer(tokenizer);
    REQUIRE(engine->get_tokenizer() != nullptr);
    REQUIRE(engine->get_tokenizer() == tokenizer);
}

TEST_CASE("Engine shutdown", "[engine]") {
    auto engine = std::make_shared<castor::Engine>();
    castor::ModelConfig config;
    
    engine->initialize("dummy.plan", config);
    REQUIRE(engine->is_initialized());
    
    engine->shutdown();
    REQUIRE(!engine->is_initialized());
}
