#include <cassert>
#include <iostream>
#include <memory>
#include "engine.hpp"
#include "tokenizer.hpp"
#include "model_config.hpp"

// Simple unit test framework (before Catch2/gtest integration)

class TestRunner {
public:
    static void run_all() {
        std::cout << "\n========== Unit Tests ==========\n";
        
        test_model_config();
        test_engine_creation();
        test_tokenizer_creation();
        
        std::cout << "========== All Tests Passed ==========\n\n";
    }

private:
    static void test_model_config() {
        std::cout << "[TEST] ModelConfig...\n";
        castor::ModelConfig config;
        config.model_name = "test-model";
        config.vocab_size = 50000;
        
        assert(config.vocab_size == 50000);
        assert(config.model_name == "test-model");
        std::cout << "  ✓ ModelConfig passed\n";
    }

    static void test_engine_creation() {
        std::cout << "[TEST] Engine creation...\n";
        auto engine = std::make_shared<castor::Engine>();
        
        assert(!engine->is_initialized());
        
        castor::ModelConfig config;
        config.model_name = "test-engine";
        
        // Try to initialize without a valid engine file (should still set initialized flag)
        bool initialized = engine->initialize("dummy.plan", config);
        assert(initialized);
        assert(engine->is_initialized());
        
        std::cout << "  ✓ Engine creation passed\n";
    }

    static void test_tokenizer_creation() {
        std::cout << "[TEST] Tokenizer...\n";
        auto tokenizer = std::make_shared<castor::Tokenizer>();
        
        assert(!tokenizer->is_loaded());
        
        // Create a dummy tokenizer file
        std::ofstream dummy_file("tests/dummy_tokenizer.json");
        dummy_file << "{}";
        dummy_file.close();
        
        // Try to load
        bool loaded = tokenizer->load("tests/dummy_tokenizer.json");
        assert(loaded);
        assert(tokenizer->is_loaded());
        assert(tokenizer->vocab_size() > 0);
        
        // Test encoding
        auto tokens = tokenizer->encode("hello world");
        assert(!tokens.empty());
        
        // Test decoding
        auto decoded = tokenizer->decode(tokens);
        assert(!decoded.empty());
        
        std::cout << "  ✓ Tokenizer passed\n";
    }
};
