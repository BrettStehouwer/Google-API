#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include "engine.hpp"
#include "tokenizer.hpp"
#include "server.hpp"
#include "../tests/test_runner.hpp"

int main(int argc, char* argv[]) {
    std::cout << "========================================\n";
    std::cout << "Castor-RT: LLM Inference Engine (Ada)\n";
    std::cout << "========================================\n";

    // Run unit tests
    TestRunner::run_all();

    // Initialize model configuration
    castor::ModelConfig config;
    config.model_name = "llama-2-7b";
    config.max_batch_size = 1;
    config.max_seq_length = 4096;
    config.vocab_size = 32000;
    config.hidden_dim = 4096;
    config.num_layers = 32;

    std::cout << "\n[Config] Model: " << config.model_name << "\n";
    std::cout << "[Config] Max Batch: " << config.max_batch_size << "\n";
    std::cout << "[Config] Max Sequence: " << config.max_seq_length << "\n";
    std::cout << "[Config] Vocab Size: " << config.vocab_size << "\n";

    // Create engine and tokenizer
    auto engine = std::make_shared<castor::Engine>();
    auto tokenizer = std::make_shared<castor::Tokenizer>();

    // Initialize tokenizer
    std::cout << "\n[Tokenizer] Loading...\n";
    std::string tokenizer_path = "models/tokenizer.json";
    
    // Create dummy tokenizer file if it doesn't exist
    std::ifstream check(tokenizer_path);
    if (!check.good()) {
        std::cout << "[Tokenizer] Creating dummy tokenizer...\n";
        std::ofstream dummy(tokenizer_path);
        dummy << R"({"model_type": "bpe", "vocab_size": 32000})";
        dummy.close();
    }
    
    if (tokenizer->load(tokenizer_path)) {
        std::cout << "[Tokenizer] Loaded successfully (vocab: " << tokenizer->vocab_size() << ")\n";
        engine->set_tokenizer(tokenizer);
    } else {
        std::cout << "[Tokenizer] Failed to load\n";
    }

    // Initialize engine
    std::cout << "\n[Engine] Initializing...\n";
    std::string engine_path = "models/llama-2-7b.plan";
    
    // Create dummy engine file if it doesn't exist
    std::ifstream check_engine(engine_path);
    if (!check_engine.good()) {
        std::cout << "[Engine] Creating dummy engine file...\n";
        std::ofstream dummy_engine(engine_path);
        dummy_engine << "DUMMY_TENSORRT_ENGINE_PLACEHOLDER";
        dummy_engine.close();
    }
    
    if (engine->initialize(engine_path, config)) {
        std::cout << "[Engine] Initialized successfully\n";
    } else {
        std::cout << "[Engine] Failed to initialize\n";
    }

    // Create server
    std::cout << "\n[Server] Starting REST API...\n";
    auto server = std::make_unique<castor::Server>(8080);
    
    if (server->initialize(engine, tokenizer)) {
        std::cout << "[Server] Initialized\n";
        std::cout << "[Server] Endpoints:\n";
        std::cout << "  GET  http://localhost:8080/health\n";
        std::cout << "  GET  http://localhost:8080/model\n";
        std::cout << "  POST http://localhost:8080/infer (body: {\"prompt\": \"...\"})\n";
        
        // Start server (blocks main thread until shutdown)
        server->run();
        // Code after this point will only execute after server shutdown
    }

    // Example inference (without server) can be placed here if needed after server shutdown
    // std::cout << "\n[Inference] Example test...\n";
    // std::vector<int32_t> input_ids = tokenizer->encode("Hello, world!");
    // std::vector<float> output_logits;
    // if (engine->infer(input_ids, output_logits)) {
    //     std::cout << "[Inference] Success!\n";
    //     std::cout << "  Input tokens: " << input_ids.size() << "\n";
    //     std::cout << "  Output logits: " << output_logits.size() << "\n";
    // } else {
    //     std::cout << "[Inference] Failed (expected - TensorRT not integrated yet)\n";
    // }

    std::cout << "\n========================================\n";
    std::cout << "Castor-RT: Ready for inference\n";
    std::cout << "Next: Integrate full TensorRT + CUDA\n";
    std::cout << "========================================\n";

    return 0;
}
