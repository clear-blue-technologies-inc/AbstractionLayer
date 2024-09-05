#ifndef __SERIALIZATION_MODULE_HPP__
#define __SERIALIZATION_MODULE_HPP__

#include "SerializationAbstraction.hpp"

class Serializer : public SerializationAbstraction {

    public:
    Serializer() = default;
    ~Serializer() = default;

    ErrorType serialize(const std::string &data, std::string &serializedData, SerializationType type) override;
    ErrorType deserialize(const std::string &serializedData, std::string &data, SerializationType type) override;
    ErrorType maxSerializedSize(Bytes &maxSize, SerializationType type) override;
    ErrorType maxDeserializedSize(Bytes &maxSize, SerializationType type) override;
};

#endif // __SERIALIZATION_MODULE_HPP__