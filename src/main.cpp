#include <iostream>
#include <vector>
#include "engine.hpp"
#include "tokenizer.hpp"

int main(int argc, char* argv[]) {
    std::cout << "========================================\n";
    std::cout << "Castor-RT: LLM Inference Engine (Ada)\n";
    std::cout << "========================================\n";

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

    // Initialize tokenizer
    castor::Tokenizer tokenizer;
    std::cout << "\n[Tokenizer] Loading...\n";
    // tokenizer.load("models/tokenizer.json");
    std::cout << "[Tokenizer] Not loaded (placeholder)\n";

    // Initialize engine
    castor::Engine engine;
    std::cout << "\n[Engine] Initializing...\n";
    // engine.initialize("models/llama-2-7b.plan", config);
    std::cout << "[Engine] Not initialized (placeholder, awaiting .plan file)\n";

    // Example inference (placeholder)
    std::cout << "\n[Inference] Example prompt: \"Hello, world!\"\n";
    std::vector<int32_t> input_ids = {1, 2, 3}; // Placeholder token IDs
    std::vector<float> output_logits;

    // Uncomment when engine is ready:
    // if (engine.infer(input_ids, output_logits)) {
    //     std::cout << "[Inference] Success! Output logits size: " << output_logits.size() << "\n";
    // } else {
    //     std::cout << "[Inference] Failed\n";
    // }

    std::cout << "\n========================================\n";
    std::cout << "Castor-RT initialized (architecture only)\n";
    std::cout << "Next: Add TensorRT engine + tokenizer models\n";
    std::cout << "========================================\n";

    return 0;
}
