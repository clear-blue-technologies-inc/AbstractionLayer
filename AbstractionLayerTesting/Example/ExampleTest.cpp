//C++
#include <vector>
#include <functional>
//Modules
#include "LoggerModule.hpp"

static const char TAG[] = "exampleTest";

static int exampleTest1() {
    const bool testWillPass = true;

    if (testWillPass) {
        CBT_LOGI(TAG, "exampleTest1 passed");
        return EXIT_SUCCESS;
    }
    else {
        CBT_LOGE(TAG, "exampleTest1 failed");
        return EXIT_FAILURE;
    }
}

static int runAllTests() {
    std::vector<std::function<int(void)>> tests = {
        exampleTest1
    };

    for (auto test : tests) {
        if (EXIT_SUCCESS != test()) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int main() {

    Logger::Init();

    int result = runAllTests();

    return result;
}