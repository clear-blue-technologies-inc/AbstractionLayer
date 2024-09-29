//Modules
#include "CbtUart.hpp"
#include "CbtStorage.hpp"
//Foundation
#include "CbtLog.hpp"

static const char TAG[] = "uartTest";
using namespace Fnd;

Uart uart;

static int testBlockingWrite() {
    std::string txData = "hello world";
    std::string rxBuffer(txData.size(), '\0');
    const Milliseconds timeout = 1000;
    ErrorType error;
    Bytes received = 0;

    bool success = ErrorType::Success == (error = uart.txBlocking(txData, timeout));
    if (!success) {
        CBT_LOGI(TAG, "uart tx failed: %u", (uint8_t)error);
        assert(false);
    }

    return EXIT_SUCCESS;
}

static int testBlockingRead() {
    std::string rxBuffer(uart.receiveBufferSize(), '\0');
    ErrorType error;
    const Milliseconds timeout = 1000;

    error = uart.rxBlocking(rxBuffer, timeout);
    bool success = error == ErrorType::Success || error == ErrorType::EndOfFile;
    if (!success) {
        CBT_LOGI(TAG, "uart rx failed. error: %u bytes read: %u", (uint8_t)error, rxBuffer.size());
        assert(false);
    }
    else {
        CBT_LOGI(TAG, "uart rx success. bytes read: %u", rxBuffer.size(), rxBuffer.c_str());
        for (size_t i = 0; i < rxBuffer.size(); i++) {
            CBT_LOGI(TAG, "%x", (uint8_t)rxBuffer[i]);
        }

    }

    return EXIT_SUCCESS;
}

static int runAllTests(std::vector<std::function<int(void)>> &overrideTests) {
    std::vector<std::function<int(void)>> testsToUse;
    std::vector<std::function<int(void)>> defaultTests = {
        testBlockingWrite,
        testBlockingRead
    };

    if (overrideTests.empty()) {
        testsToUse = defaultTests;
    }
    else {
        testsToUse = overrideTests;
    }

    for (auto test : testsToUse) {
        if (EXIT_SUCCESS != test()) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int main() {
    ErrorType error;

    Logger::Init();
    Storage::Init("uartStorage");

    std::vector<std::function<int(void)>> overrideTests = {testBlockingWrite};

    if (ErrorType::Success != (error = Storage::Instance().initStorage())) {
        CBT_LOGI(TAG, "storage init failed %u", (uint8_t)error);
        assert(false);
    }
    if (ErrorType::Success != (error = uart.init())) {
        if (error == ErrorType::FileNotFound) {
            CBT_LOGW(TAG, "uart rx file buffer not found %u", (uint8_t)error);
        }
        else {
            CBT_LOGI(TAG, "uart init failed %u", (uint8_t)error);
            assert(false);
        }
    }
    else {
        overrideTests.push_back(testBlockingRead);
    }

    uart.setFirmwareConfig(64, 64, -1);
    int result = runAllTests(overrideTests);

    if (ErrorType::Success != (error = uart.deinit())) {
        CBT_LOGI(TAG, "uart deinit failed %u", (uint8_t)error);
        assert(false);
    }


    return result;
}