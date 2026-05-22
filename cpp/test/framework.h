#pragma once
#include <cstdio>
#include <functional>
#include <string>
#include <vector>
#include <stdexcept>

using std::string;
using std::vector;
using std::function;
using std::move;
using std::runtime_error;
using std::to_string;
using std::exception;

struct TestCase {
    string name;
    function<void()> fn;
};

inline vector<TestCase>& testRegistry() {
    static vector<TestCase> reg;
    return reg;
}

struct TestRegistrar {
    TestRegistrar(const char* name, function<void()> fn) {
        testRegistry().push_back({name, move(fn)});
    }
};

#define TEST(suite, name)                                                       \
    static void suite##_##name();                                               \
    static TestRegistrar suite##_##name##_reg(                                   \
        #suite "." #name, suite##_##name);                                      \
    static void suite##_##name()

#define STRINGIFY(x) #x
#define ASSERT(cond, msg) do {                                                  \
    if (!(cond)) {                                                              \
        throw runtime_error(                                                    \
            string(__FILE__) + ":" + to_string(__LINE__) +                      \
            ": ASSERT(" #cond ") failed: " + msg);                              \
    }                                                                           \
} while(0)

#define ASSERT_TRUE(cond) ASSERT(cond, "")
#define ASSERT_FALSE(cond) ASSERT(!(cond), "")

#define ASSERT_EQ(a, b) do {                                                    \
    auto _a = (a); auto _b = (b);                                              \
    if (_a != _b) {                                                             \
        throw runtime_error(                                                    \
            string(__FILE__) + ":" + to_string(__LINE__) +                      \
            ": ASSERT_EQ(" #a ", " #b ") failed");                              \
    }                                                                           \
} while(0)

#define ASSERT_NEAR(a, b, eps) do {                                             \
    auto _d = ((a) > (b)) ? ((a) - (b)) : ((b) - (a));                         \
    if (_d > (eps)) {                                                           \
        throw runtime_error(                                                    \
            string(__FILE__) + ":" + to_string(__LINE__) +                      \
            ": ASSERT_NEAR(" #a ", " #b ") failed");                            \
    }                                                                           \
} while(0)

#define ASSERT_STREQ(a, b) do {                                                 \
    string _a(a); string _b(b);                                                \
    if (_a != _b) {                                                             \
        throw runtime_error(                                                    \
            string(__FILE__) + ":" + to_string(__LINE__) +                      \
            ": ASSERT_STREQ(" #a ", " #b ") failed: \"" + _a + "\" != \"" + _b + "\""); \
    }                                                                           \
} while(0)

inline int runAllTests() {
    int passed = 0, failed = 0;
    for (auto& t : testRegistry()) {
        printf("  %s ... ", t.name.c_str());
        fflush(stdout);
        try {
            t.fn();
            printf("OK\n");
            passed++;
        } catch (const exception& e) {
            printf("FAIL\n    %s\n", e.what());
            failed++;
        } catch (...) {
            printf("FAIL (unknown exception)\n");
            failed++;
        }
    }
    printf("\n%d passed, %d failed out of %zu\n", passed, failed, testRegistry().size());
    return failed;
}
