cmake_minimum_required(VERSION 3.24)
project(Castor-RT LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optional CUDA support
include(CheckLanguage)
check_language(CUDA)
if(CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
    set(CUDA_STANDARD 20)
    set(CUDA_STANDARD_REQUIRED ON)
    set(HAVE_CUDA TRUE)
else()
    message(WARNING "CUDA not found. Building C++ only (no GPU inference)")
    set(HAVE_CUDA FALSE)
endif()

# Build Type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")

# Find CUDA (optional)
if(HAVE_CUDA)
    find_package(CUDAToolkit REQUIRED)
    message(STATUS "CUDA Version: ${CUDAToolkit_VERSION}")
    
    # Check for CUDA Compute Capability (RTX 4090 = sm_89)
    set(CUDA_ARCH "89" CACHE STRING "CUDA Architecture (89 for RTX 4090)")
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode arch=compute_${CUDA_ARCH},code=sm_${CUDA_ARCH}")
    message(STATUS "CUDA Architecture: sm_${CUDA_ARCH}")
endif()

# Find TensorRT (optional)
find_package(TensorRT QUIET)
if(TensorRT_FOUND)
    message(STATUS "TensorRT: FOUND")
else()
    message(WARNING "TensorRT not found. Building without GPU inference")
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/crow/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/tokenizers-cpp
)

if(HAVE_CUDA AND CUDAToolkit_FOUND)
    include_directories(${CUDAToolkit_INCLUDE_DIRS})
endif()

if(TensorRT_FOUND)
    include_directories(${TensorRT_INCLUDE_DIRS})
endif()

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
)

if(HAVE_CUDA AND CUDAToolkit_FOUND)
    target_link_libraries(castor-rt
        PRIVATE
        CUDA::cudart
        CUDA::cublas
        CUDA::curand
    )
endif()

if(TensorRT_FOUND)
    target_link_libraries(castor-rt
        PRIVATE
        nvinfer
        nvinfer_plugin
    )
endif()

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
if(HAVE_CUDA)
    message(STATUS "CUDA Support: ENABLED (sm_${CUDA_ARCH})")
else()
    message(STATUS "CUDA Support: DISABLED")
endif()
if(TensorRT_FOUND)
    message(STATUS "TensorRT: ENABLED")
else()
    message(STATUS "TensorRT: DISABLED")
endif()
message(STATUS "Source files: ${SOURCES}")
message(STATUS "============================================")
