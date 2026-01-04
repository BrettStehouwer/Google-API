#include <iostream>
#include <cassert>
#include <memory>
#include "../include/engine.hpp"
#include "../include/tokenizer.hpp"
#include "../include/model_config.hpp"

int main() {
    std::cout << "\n========== Unit Tests ==========\n";
    
    int passed = 0, failed = 0;
    
    // Test ModelConfig
    {
        castor::ModelConfig config;
        assert(config.max_batch_size == 1);
        assert(config.max_seq_length == 4096);
        std::cout << "✓ ModelConfig initialization\n";
        passed++;
    }
    
    // Test Engine creation
    {
        auto engine = std::make_shared<castor::Engine>();
        assert(!engine->is_initialized());
        std::cout << "✓ Engine creation\n";
        passed++;
    }
    
    // Test Engine initialization
    {
        auto engine = std::make_shared<castor::Engine>();
        castor::ModelConfig config;
        config.model_name = "test-model";
        bool result = engine->initialize("dummy.plan", config);
        assert(result);
        assert(engine->is_initialized());
        std::cout << "✓ Engine initialization\n";
        passed++;
    }
    
    // Test Tokenizer creation
    {
        auto tokenizer = std::make_shared<castor::Tokenizer>();
        assert(!tokenizer->is_loaded());
        std::cout << "✓ Tokenizer creation\n";
        passed++;
    }
    
    // Test Engine tokenizer binding
    {
        auto engine = std::make_shared<castor::Engine>();
        auto tokenizer = std::make_shared<castor::Tokenizer>();
        engine->set_tokenizer(tokenizer);
        assert(engine->get_tokenizer() == tokenizer);
        std::cout << "✓ Engine tokenizer binding\n";
        passed++;
    }
    
    std::cout << "\n========== Test Summary ==========\n";
    std::cout << "Passed: " << passed << "\n";
    std::cout << "Failed: " << failed << "\n\n";
    
    return failed == 0 ? 0 : 1;
}
