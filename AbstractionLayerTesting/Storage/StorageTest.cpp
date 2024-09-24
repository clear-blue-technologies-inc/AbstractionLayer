//Modules
#include "StorageModule.hpp"
#include "FileModule.hpp"
#include "OperatingSystemModule.hpp"
//Applications
#include "Log.hpp"


static const char TAG[] = "storageTest";

static void *startStorageThread(void *arg) {
    while (true) {
        OperatingSystem::Instance().delay(1000);
        Storage::Instance().mainLoop();
    }
    return nullptr;
}

static int testReadWrite() {
    ErrorType error = ErrorType::Success;
    std::string filename("/testFile");

    File file(Storage::Instance());
    if (ErrorType::Success != (error = file.open(filename, OpenMode::ReadWriteAppend))) {
        CBT_LOGE(TAG, "Error opening file %u", (uint8_t)error);
        assert(false);
    }

    const std::string writeString("Hello World!");
    std::string readString(writeString.size(), 0);
    Bytes written = 0;

    if (ErrorType::Success != (error = file.writeBlocking(0, writeString))) {
        CBT_LOGE(TAG, "Error writing to file %u", (uint8_t)error);
        assert(false);
    }
    if (ErrorType::Success != (error = file.readBlocking(0, readString))) {
        CBT_LOGE(TAG, "Error reading from file %u", (uint8_t)error);
        assert(false);
    }

    if (0 == readString.compare(writeString)) {
        if (ErrorType::Success != file.close()) {
            CBT_LOGE(TAG, "Error closing file %u", (uint8_t)error);
            assert(false);
        }
        if (ErrorType::Success != file.remove()) {
            CBT_LOGE(TAG, "Error removing file %u", (uint8_t)error);
            assert(false);
        }
        return EXIT_SUCCESS;
    }
    else {
        CBT_LOGE(TAG, "Error reading from file %u. data: %s", (uint8_t)error, readString.c_str());
    }

    assert(false);
}

static int testNonBlockingReadWrite() {
    ErrorType error = ErrorType::Success;
    std::string filename("/testFile");

    File file(Storage::Instance());
    if (ErrorType::Success != (error = file.open(filename, OpenMode::ReadWriteTruncate))) {
        CBT_LOGE(TAG, "Error opening file %u", (uint8_t)error);
        assert(false);
    }

    const auto writeString = std::make_shared<std::string>("Hello World!");
    auto buffer = std::make_shared<std::string>(writeString->size(), 0);
    Bytes written = 0;
    const Bytes toWrite = writeString->size();
    ErrorType readError = ErrorType::Failure;
    ErrorType writeError = ErrorType::Failure;

    auto readCallback = [&readError, writeString](const ErrorType error, std::shared_ptr<std::string> buffer) {
        if (0 == buffer->compare(*(writeString.get()))) {
            readError = error;
        }
        else {
            CBT_LOGE(TAG, "Error reading from file Error: %u, data: %s", error, buffer->c_str());
            readError = error;
            assert(false);
        }
    };

    auto writeCallback = [&writeError, toWrite](const ErrorType error, const Bytes written) {
        if (ErrorType::Success == error && written == toWrite) {
            writeError = error;
        }
        else {
            CBT_LOGE(TAG, "Error writing to file Error: %u, written: %u, toWrite: %u", error, written, toWrite);
            writeError = error;
            assert(false);
        }
    };

    file.writeNonBlocking(writeString, writeCallback);
    file.readNonBlocking((FileOffset)0, buffer, readCallback);

    //Wait for the thread to process the read and write events.
    while (ErrorType::Success != readError && ErrorType::Success != writeError) {
        OperatingSystem::Instance().delay(10);
    }

    if (ErrorType::Success != (error = file.close())) {
        CBT_LOGE(TAG, "Error closing file %u", (uint8_t)error);
        assert(false);
    }
    if (ErrorType::Success != (error = file.remove())) {
        CBT_LOGE(TAG, "Error removing file %u", (uint8_t)error);
        assert(false);
    }

    return EXIT_SUCCESS;
}

static int useWithoutOpening() {
    ErrorType error = ErrorType::Success;
    std::string filename("/testFile");
    Bytes written = 0;

    File file(Storage::Instance());

    //Write without opening
    error = file.writeBlocking(0, filename.data());
    assert(ErrorType::Success != error);

    //Close without opening
    error = file.close();
    if (ErrorType::Success != error) {
        CBT_LOGE(TAG, "File should already by closed.");
        assert(false);
    }

    //File doesn't exist, so let's try removing it.
    error = file.remove();
    if (ErrorType::Failure != error) {
        CBT_LOGE(TAG, "File should not exist %u", (uint8_t)error);
        assert(false);
    }

    //Try reading the non-existent file.
    error = file.readBlocking(0, filename);
    if (ErrorType::PrerequisitesNotMet != error) {
        CBT_LOGE(TAG, "File should not be readable %u", (uint8_t)error);
        assert(false);
    }

    if (error != ErrorType::Success) {
      return EXIT_SUCCESS;
    }
    else {
        assert(false);
    }
}

static int openFlags() {
    ErrorType error = ErrorType::Success;
    std::string filename("/testFile");
    std::string writeString("openFlagsTest");
    std::string readString(writeString.size(), 0);
    Bytes written = 0;

    File file(Storage::Instance());
    //Open the file as read-only.
    error = file.open(filename, OpenMode::ReadOnly);
    if (ErrorType::Failure != error) {
        CBT_LOGE(TAG, "Can't open a file in read-only that doesn't exist %u", (uint8_t)error);
        assert(false);
    }
    //Open the file as write-only
    error = file.open(filename, OpenMode::WriteOnlyAppend);
    if (ErrorType::Success != error) {
        CBT_LOGE(TAG, "Error opening file %u", (uint8_t)error);
        assert(false);
    }
    //Now try to write to it
    error = file.writeBlocking(0, writeString);
    if (ErrorType::Success != error) {
        CBT_LOGE(TAG, "Error writing file %u", (uint8_t)error);
        assert(false);
    }

    //Close the file
    error = file.close();
    if (ErrorType::Success != error) {
        CBT_LOGE(TAG, "Error closing file %u", (uint8_t)error);
        assert(false);
    }

    //Then reopen as read-only
    file.open(filename, OpenMode::ReadOnly);
    if (ErrorType::Success != error) {
        CBT_LOGE(TAG, "Error opening file %u", (uint8_t)error);
        assert(false);
    }

    //Write to the read-only file
    error = file.writeBlocking(0, writeString);
    if (ErrorType::Success == error && written == writeString.length()) {
        CBT_LOGE(TAG, "Wrote to read-only file! %u", (uint8_t)error);
        assert(false);
    }

    error = file.readBlocking(0, readString);
    const bool success = ErrorType::Success != error;
    const bool correctNumBytesRead = readString.length() == writeString.length();
    if (!success && !correctNumBytesRead) {
        CBT_LOGE(TAG, "Error reading file success: %d correctNumBytesRead: %d", success, correctNumBytesRead);
        assert(false);
    }

    error = file.remove();
    if (ErrorType::Success != error) {
        CBT_LOGE(TAG, "Error removing file %u", (uint8_t)error);
        assert(false);
    }

    return EXIT_SUCCESS;
}

static int runAllTests() {
    std::vector<std::function<int(void)>> tests = {
        testReadWrite,
        testNonBlockingReadWrite,
        useWithoutOpening,
        openFlags
    };

    for (auto test : tests) {
        if (EXIT_SUCCESS != test()) {
            assert(false);
        }
    }

    return EXIT_SUCCESS;
}

int main() {

    OperatingSystem::Init();
    Logger::Init();

    Storage::Init(std::string("testStorage"));
    if (ErrorType::Success != Storage::Instance().initStorage()) {
        CBT_LOGE(TAG, "initStorage failed");
        assert(false);
    }

    constexpr uint16_t kilobyte = 1024;
    Id storageId;

    OperatingSystem::Instance().createThread(OperatingSystemConfig::Priority::Low, "storageThread", nullptr, 4*kilobyte, startStorageThread, storageId);

    int result = runAllTests();

    return result;
}