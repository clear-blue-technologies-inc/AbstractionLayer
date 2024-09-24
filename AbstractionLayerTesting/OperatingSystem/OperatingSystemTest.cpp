//C++
#include <vector>
#include <functional>
//Modules
#include "Log.hpp"
#include "OperatingSystemModule.hpp"

static const char TAG[] = "operatingSystem";
bool threadWasCreated = false;

#ifdef __cplusplus
extern "C" {
#endif

static void *testThreadStartFunction(void *arg) {
    threadWasCreated = true;

    return nullptr;
}

#ifdef __cplusplus
}
#endif

static int createThreadTest() {
    OperatingSystemConfig::Priority priority = OperatingSystemConfig::Priority::Normal;
    std::string name("testThread");
    Bytes stackSize = 4096;
    Id threadId;
    ErrorType error;

    error = OperatingSystem::Instance().createThread(priority, name, nullptr, stackSize, testThreadStartFunction, threadId);
    assert(ErrorType::PrerequisitesNotMet != error);

    return EXIT_SUCCESS;
}

static int runAllTests() {
    std::vector<std::function<int(void)>> tests = {
        createThreadTest
    };

    for (auto test : tests) {
        if (EXIT_SUCCESS != test()) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int main() {

    OperatingSystem::Init();
    Logger::Init();

    int result = runAllTests();
    toPlatformError(result);

    return result;
}