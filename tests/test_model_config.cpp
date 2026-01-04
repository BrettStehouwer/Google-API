#include "catch.hpp"
#include <memory>
#include "model_config.hpp"

TEST_CASE("ModelConfig initialization", "[model_config]") {
    castor::ModelConfig config;
    
    REQUIRE(config.max_batch_size == 1);
    REQUIRE(config.max_seq_length == 4096);
    REQUIRE(config.vocab_size == 32000);
}

TEST_CASE("ModelConfig custom values", "[model_config]") {
    castor::ModelConfig config;
    config.model_name = "llama-2-13b";
    config.max_batch_size = 4;
    config.vocab_size = 50000;
    
    REQUIRE(config.model_name == "llama-2-13b");
    REQUIRE(config.max_batch_size == 4);
    REQUIRE(config.vocab_size == 50000);
}

TEST_CASE("TensorInfo creation", "[model_config]") {
    castor::TensorInfo info;
    info.name = "input";
    info.dtype = "int32";
    info.shape = {1, 4096};
    
    REQUIRE(info.name == "input");
    REQUIRE(info.dtype == "int32");
    REQUIRE(info.shape.size() == 2);
    REQUIRE(info.shape[0] == 1);
    REQUIRE(info.shape[1] == 4096);
}
