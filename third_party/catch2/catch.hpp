// Minimal Catch2 v3 compatible header for basic testing
#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <stdexcept>

namespace Catch2 {
    static int test_count = 0;
    static int fail_count = 0;

    struct TestCase {
        std::string name;
        std::function<void()> test_func;
    };

    static std::vector<TestCase>& get_tests() {
        static std::vector<TestCase> tests;
        return tests;
    }

    void register_test(const std::string& name, std::function<void()> test) {
        get_tests().push_back({name, test});
    }

    struct Section {
        Section(const std::string& name) : name(name) {}
        std::string name;
    };

    template<typename T, typename U>
    void require_equal(const T& a, const U& b, const std::string& file, int line) {
        if (!(a == b)) {
            std::cerr << "FAIL at " << file << ":" << line << " - " << a << " != " << b << "\n";
            fail_count++;
            throw std::runtime_error("Assertion failed");
        }
    }

    template<typename T>
    void require_true(const T& val, const std::string& file, int line) {
        if (!val) {
            std::cerr << "FAIL at " << file << ":" << line << " - assertion false\n";
            fail_count++;
            throw std::runtime_error("Assertion failed");
        }
    }

    class Session {
    public:
        int run(int argc, char* argv[]) {
            std::cout << "\n========== Running Tests ==========\n";
            int passed = 0;
            int failed = 0;
            
            for (auto& test : get_tests()) {
                try {
                    test.test_func();
                    std::cout << "✓ " << test.name << "\n";
                    passed++;
                } catch (const std::exception& e) {
                    std::cout << "✗ " << test.name << " - " << e.what() << "\n";
                    failed++;
                }
            }
            
            std::cout << "\n========== Test Summary ==========\n";
            std::cout << "Passed: " << passed << "\n";
            std::cout << "Failed: " << failed << "\n";
            
            return failed == 0 ? 0 : 1;
        }
    };
}

#define TEST_CASE(name, tags) \
    void test_func_##__LINE__(); \
    namespace { struct test_register_##__LINE__ { test_register_##__LINE__() { \
        Catch2::register_test(name, test_func_##__LINE__); \
    } } test_instance_##__LINE__; } \
    void test_func_##__LINE__()

#define REQUIRE(expr) Catch2::require_true(expr, __FILE__, __LINE__)
#define REQUIRE_EQ(a, b) Catch2::require_equal(a, b, __FILE__, __LINE__)
#define CHECK(expr) REQUIRE(expr)

// Approx for floating point
struct Approx {
    double value;
    double margin = 1e-6;
    
    Approx(double v) : value(v) {}
    
    bool operator==(double other) const {
        return std::abs(value - other) < margin;
    }
};

inline Approx approx(double val) {
    return Approx(val);
}

#define CATCH2_APPROX(v) Catch2::approx(v)
