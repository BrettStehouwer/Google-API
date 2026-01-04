cmake_minimum_required(VERSION 3.24)
project(Castor-RT LANGUAGES CXX CUDA)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CUDA_STANDARD 20)
set(CUDA_STANDARD_REQUIRED ON)

# Build Type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")

# Find required packages
find_package(CUDA REQUIRED)
find_package(TensorRT REQUIRED)

# Check for CUDA Compute Capability (RTX 4090 = sm_89)
set(CUDA_ARCH "89" CACHE STRING "CUDA Architecture (89 for RTX 4090)")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode arch=compute_${CUDA_ARCH},code=sm_${CUDA_ARCH}")

message(STATUS "CUDA Version: ${CUDA_VERSION}")
message(STATUS "CUDA Architecture: sm_${CUDA_ARCH}")
message(STATUS "TensorRT: ${TensorRT_FOUND}")

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CUDA_INCLUDE_DIRS}
    ${TensorRT_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/crow/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/tokenizers-cpp
)

# Source files
set(SOURCES
    src/main.cpp
    src/engine.cpp
    src/tokenizer.cpp
)

set(HEADERS
    include/engine.hpp
    include/tokenizer.hpp
    include/model_config.hpp
)

# Create executable
add_executable(castor-rt ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(castor-rt
    PRIVATE
    CUDA::cudart
    CUDA::cublas
    CUDA::curand
    nvinfer
    nvinfer_plugin
)

# Compiler flags
if(MSVC)
    target_compile_options(castor-rt PRIVATE /W4)
else()
    target_compile_options(castor-rt PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Output directory
set_target_properties(castor-rt PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Print configuration summary
message(STATUS "========== Castor-RT Configuration ==========")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "CUDA Arch: sm_${CUDA_ARCH}")
message(STATUS "TensorRT: FOUND")
message(STATUS "Source files: ${SOURCES}")
message(STATUS "============================================")
