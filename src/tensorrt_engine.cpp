#include "tensorrt_engine.hpp"
#include <iostream>
#include <fstream>

namespace castor {

TensorRTEngine::TensorRTEngine() {}

TensorRTEngine::~TensorRTEngine() {
#ifdef HAVE_TENSORRT
    if (context_) context_->destroy();
    if (engine_) engine_->destroy();
    if (runtime_) runtime_->destroy();
#endif
}

#ifdef HAVE_TENSORRT
bool TensorRTEngine::load(const std::string& engine_path) {
    std::ifstream engine_file(engine_path, std::ios::binary);
    if (!engine_file.good()) {
        std::cerr << "[TensorRT] Failed to open engine file: " << engine_path << "\n";
        return false;
    }

    engine_file.seekg(0, std::ios::end);
    size_t size = engine_file.tellg();
    engine_file.seekg(0, std::ios::beg);

    std::vector<char> model_data(size);
    engine_file.read(model_data.data(), size);
    engine_file.close();

    // TODO: Deserialize TensorRT engine and create execution context
    std::cout << "[TensorRT] Loaded engine: " << size << " bytes\n";
    
    return true;
}

bool TensorRTEngine::infer(const std::vector<std::vector<float>>& inputs,
                           std::vector<std::vector<float>>& outputs) {
    // TODO: Copy inputs to GPU, run inference, copy outputs back
    return true;
}

int TensorRTEngine::get_num_inputs() const {
    // TODO: Return actual number from engine
    return 1;
}

int TensorRTEngine::get_num_outputs() const {
    // TODO: Return actual number from engine
    return 1;
}
#else
// CPU-only implementation
bool TensorRTEngine::load(const std::string& engine_path) {
    std::ifstream engine_file(engine_path, std::ios::binary);
    if (!engine_file.good()) {
        std::cerr << "[TensorRT] Engine file not found (CPU mode): " << engine_path << "\n";
        return false;
    }
    std::cout << "[TensorRT] Loaded engine (CPU stub)\n";
    return true;
}

bool TensorRTEngine::infer(const std::vector<std::vector<float>>& inputs,
                           std::vector<std::vector<float>>& outputs) {
    // Stub: just return success
    std::cout << "[TensorRT] Inference (CPU stub)\n";
    return true;
}
#endif

} // namespace castor
