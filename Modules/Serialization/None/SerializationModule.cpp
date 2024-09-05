#include "SerializationModule.hpp"

namespace Fnd {

    ErrorType Serializer::serialize(const std::string &data, std::string &serializedData, SerializationType type) {
        return ErrorType::NotImplemented;
    }
    ErrorType Serializer::deserialize(const std::string &serializedData, std::string &data, SerializationType type) {
        return ErrorType::NotImplemented;
    }
    ErrorType Serializer::maxSerializedSize(Bytes &maxSize, SerializationType type) {
        return ErrorType::NotImplemented;
    }
    ErrorType Serializer::maxDeserializedSize(Bytes &maxSize, SerializationType type) {
        return ErrorType::NotImplemented;
    }
}