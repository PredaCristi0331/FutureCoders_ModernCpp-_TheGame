#pragma once
#include <functional>
#include <string>
#include <vector>
#include <iostream>
#include <mutex>
#include <cassert>

class TestRegistry {
public:
    using TestFn = std::function<void()>;

    static TestRegistry& instance() {
        static TestRegistry inst;
        return inst;
    }

    void add(const std::string& name, TestFn fn) {
        std::lock_guard<std::mutex> lock(mutex_);
        tests_.emplace_back(name, std::move(fn));
    }

    void runAll() {
        size_t passed = 0;
        size_t total = tests_.size();
        std::cout << "Running " << total << " automated tests...\n";
        for (auto& [name, fn] : tests_) {
            try {
                fn();
                ++passed;
                std::cout << "[ OK ] " << name << '\n';
            }
            catch (const std::exception& e) {
                std::cout << "[FAIL] " << name << " -> exception: " << e.what() << '\n';
            }
            catch (...) {
                std::cout << "[FAIL] " << name << " -> unknown exception\n";
            }
        }
        std::cout << "Result: " << passed << "/" << total << " tests passed.\n";
        assert(passed == total && "Some tests failed");
    }

private:
    TestRegistry() = default;
    std::vector<std::pair<std::string, TestFn>> tests_;
    std::mutex mutex_;
};

// Helper macro: defines a static registration object and the function
#define REGISTER_TEST(fn_name)                              \
    static void fn_name();                                  \
    namespace {                                             \
        struct reg_##fn_name {                              \
            reg_##fn_name() { TestRegistry::instance().add(#fn_name, fn_name); } \
        };                                                  \
        static reg_##fn_name reg_instance_##fn_name;        \
    }                                                       \
    static void fn_name()