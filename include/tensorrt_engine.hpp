#pragma once

#ifdef HAVE_TENSORRT
#include <NvInfer.h>
#include <cuda_runtime.h>
#endif

#include <vector>
#include <memory>
#include <string>

namespace castor {

/**
 * @brief TensorRT engine wrapper for model inference
 */
class TensorRTEngine {
public:
    TensorRTEngine();
    ~TensorRTEngine();

#ifdef HAVE_TENSORRT
    /**
     * @brief Load TensorRT engine from serialized plan file
     * @param engine_path Path to .plan file
     * @return true if loading successful
     */
    bool load(const std::string& engine_path);

    /**
     * @brief Run inference
     * @param inputs Vector of input tensors
     * @param outputs Vector of output tensors (pre-allocated)
     * @return true if inference successful
     */
    bool infer(const std::vector<std::vector<float>>& inputs,
               std::vector<std::vector<float>>& outputs);

    /**
     * @brief Get number of input tensors
     */
    int get_num_inputs() const;

    /**
     * @brief Get number of output tensors
     */
    int get_num_outputs() const;

private:
    nvinfer1::IRuntime* runtime_ = nullptr;
    nvinfer1::ICudaEngine* engine_ = nullptr;
    nvinfer1::IExecutionContext* context_ = nullptr;
    std::vector<void*> bindings_;
#else
    // CPU-only stub
    bool load(const std::string& engine_path);
    bool infer(const std::vector<std::vector<float>>& inputs,
               std::vector<std::vector<float>>& outputs);
    int get_num_inputs() const { return 1; }
    int get_num_outputs() const { return 1; }
#endif
};

} // namespace castor
