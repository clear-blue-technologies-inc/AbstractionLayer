//C++
#include <vector>
#include <functional>
#include <iostream>
#include <string>
#include <cstring> 
//Modules
#include "CbtSerialization.hpp"
//Foundation
#include "CbtLog.hpp"
//API Protobufs
#include "api_fc.pb.h"
#include "api_cf.pb.h"

static const char TAG[] = "serializationTest";
using namespace Fnd;

static int testEmptyInput() {
    ErrorType error = ErrorType::Success;

    Serializer serializer;
    std::string data = "";
    std::string serializedData = "";

    error = serializer.serialize(data, serializedData, SerializationType::FoundationToCleonDataRequest);
    assert(error == ErrorType::Failure); // Expect failure due to empty data

    error = serializer.deserialize(serializedData, data, SerializationType::FoundationToCleonDataRequest);
    assert(error == ErrorType::Failure); // Expect failure due to empty serialized data

    if (error == ErrorType::Failure) {
      return EXIT_SUCCESS;
    }
    else {
        CBT_LOGI(TAG, "Error with empty input data %u", error);
        return EXIT_FAILURE;
    }
}

static int testInvalidSerializationType() {
    ErrorType error = ErrorType::Success;

    Serializer serializer;
    // Initialize the input data
    FC exampleFC = FC_init_default;
    exampleFC.has_ut = true;
    exampleFC.ut = 12345;
    std::string data(reinterpret_cast<char*>(&exampleFC), sizeof(FC));
    std::string serializedData(100, '\0');

    error = serializer.serialize(data, serializedData, static_cast<SerializationType>(-1));
    assert(error == ErrorType::NotSupported); // Expect not supported error for invalid type

    error = serializer.deserialize(serializedData, data, static_cast<SerializationType>(-1));
    assert(error == ErrorType::NotSupported); // Expect not supported error for invalid type

    if (error == ErrorType::NotSupported) {
        return EXIT_SUCCESS;
    }
    else {
        CBT_LOGI(TAG, "Error with invalid serialization type %u", error);
        return EXIT_FAILURE;
    }
}

static int testStringBeforeAndAfter() {
    ErrorType error = ErrorType::Failure;

    Logger::Init();

    Serializer serializer;
    // Initialize the input data
    FC exampleFC = FC_init_default;
    exampleFC.has_ut = true;
    exampleFC.ut = 12345;
    std::string data(reinterpret_cast<char*>(&exampleFC), sizeof(FC));
    std::string serializedData(1024, '\0');
    std::string deserializedData(1024, '\0');

    // Serialization
    error = serializer.serialize(data, serializedData, SerializationType::FoundationToCleonDataRequest);
    assert(error == ErrorType::Success);
    // Deserialization
    error = serializer.deserialize(serializedData, deserializedData, SerializationType::FoundationToCleonDataRequest);
    assert(error == ErrorType::Success);

    FC message = FC_init_default;
    std::memcpy(&message, deserializedData.data(), sizeof(FC));

    // Compare original data before serialization with the deserialized data
    if (std::memcmp(data.data(), deserializedData.data(), data.size()) == 0) {
        return EXIT_SUCCESS;
    }
    else {
        CBT_LOGI(TAG, "Error with testing string before and after %u", error);
        return EXIT_FAILURE;
    }
}

static int testInvalidDataToSerialize() {
    ErrorType error = ErrorType::Success;

    Serializer serializer;
    std::string data(sizeof(FC), 'a');
    std::string serializedData(sizeof(FC), '\0');

    // Simulate serialization
    error = serializer.serialize(data, serializedData, SerializationType::FoundationToCleonDataRequest);

    if (error == ErrorType::InvalidParameter) {
        return EXIT_SUCCESS;
    }
    else {
        CBT_LOGI(TAG, "Error with serializing invalid data %u", error);
        return EXIT_FAILURE;
    }
}

static int testInvalidDataToDeserialize() {
    ErrorType error = ErrorType::Success;

    Serializer serializer;
    std::string data(sizeof(FC), 'a');
    std::string serializedData = "\x08\xb9\x60\x08\xb9\x60"; // Not a valid protobuf string because it contains the same field twice

    // Simulate serialization
    error = serializer.deserialize(serializedData, data, SerializationType::FoundationToCleonDataRequest);

    if (error == ErrorType::InvalidParameter) {
        return EXIT_SUCCESS;
    }
    else {
        CBT_LOGI(TAG, "Error with deserializing invalid data %u", error);
        return EXIT_FAILURE;
    }
}

static int testCorruptedSerializedData() {
    ErrorType error = ErrorType::Success;

    Serializer serializer;
    // Initialize the input data
    FC exampleFC = FC_init_default;
    exampleFC.has_ut = true;
    exampleFC.ut = 12345;
    std::string data(reinterpret_cast<char*>(&exampleFC), sizeof(FC));
    std::string serializedData(1024, '\0');
    std::string deserializedData(1024, '\0');

    // Serialization
    error = serializer.serialize(data, serializedData, SerializationType::FoundationToCleonDataRequest);
    assert(error == ErrorType::Success);

    // Corrupt serialized data (only certain memory location will actually ruin the data)
    serializedData[1] = '\xFF';

    // Deserialization
    error = serializer.deserialize(serializedData, deserializedData, SerializationType::FoundationToCleonDataRequest);
    assert(error == ErrorType::Success);

    // Check that the corrupted data does not match
    if (std::memcmp(data.data(), deserializedData.data(), data.size()) != 0) {
        return EXIT_SUCCESS;
    }
    else {
        CBT_LOGI(TAG, "Error with testing corrupted data %u", error);
        return EXIT_FAILURE;
    }
}

static int runAllTests() {
    std::vector<std::function<int(void)>> tests = {
        testEmptyInput,
        testInvalidSerializationType,
        testStringBeforeAndAfter,
        testInvalidDataToSerialize,
        testInvalidDataToDeserialize,
        testCorruptedSerializedData,
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