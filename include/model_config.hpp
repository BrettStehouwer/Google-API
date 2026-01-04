#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

namespace castor {

/**
 * @brief Model configuration and metadata
 */
struct ModelConfig {
    std::string model_name;
    uint32_t max_batch_size = 1;
    uint32_t max_seq_length = 4096;
    uint32_t vocab_size = 32000;
    uint32_t hidden_dim = 4096;
    uint32_t num_layers = 32;
    float precision_threshold = 1e-6f;
};

/**
 * @brief Tensor shape and metadata
 */
struct TensorInfo {
    std::vector<int64_t> shape;
    std::string name;
    std::string dtype;
    size_t element_count = 0;
};

} // namespace castor
