//C++
#include <iostream>
#include <string>
#include <cstring>
//Modules
#include "SerializationModule.hpp"
//Nanopb
#include "pb_common.h"
#include "pb_decode.h"
#include "pb_encode.h"
//Api Protobufs
#include "api_fc.pb.h"
#include "api_cf.pb.h"
#include "v2-cloud-api/generated/apiF0rx.pb.h"
#include "v2-cloud-api/generated/apiF0tx.pb.h"
#include "v2-cloud-api/generated/apiBA.pb.h"
#include "v2-cloud-api/generated/apiF1.pb.h"

// Helper template function for serialization
template <typename MessageType, const pb_msgdesc_t* Fields>
ErrorType serializeMessage(const std::string& data, std::string& serializedData) {
    // Copy input data to message struct
    MessageType message = MessageType();
    std::memcpy(&message, data.data(), sizeof(MessageType));

    // Initialize output stream for serialization
    pb_ostream_t oStream = pb_ostream_from_buffer(reinterpret_cast<pb_byte_t*>(serializedData.data()), serializedData.size());

    // Encode message. This will populate the serialized data parameter
    if (!pb_encode(&oStream, Fields, &message)) {
        return ErrorType::Failure;
    }

    // Resize serializedData to actual size used
    serializedData.resize(oStream.bytes_written);

    // Initialize input stream for deserialization
    pb_istream_t iStream = pb_istream_from_buffer(reinterpret_cast<pb_byte_t*>(serializedData.data()), serializedData.size());

    // Decode message
    if (!pb_decode(&iStream, Fields, &message)) {
        return ErrorType::Failure;
    }

    // Compare original data and deserialized data
    if (std::memcmp(&message, data.data(), data.size()) != 0) {
        return ErrorType::InvalidParameter;
    }

    return ErrorType::Success;
}

ErrorType Serializer::serialize(const std::string &data, std::string &serializedData, SerializationType type) {

    // Return failure if input is empty
    if (data.empty()) {
        return ErrorType::Failure;
    }

    pb_ostream_t oStream;
    oStream = pb_ostream_from_buffer(reinterpret_cast<pb_byte_t*>(serializedData.data()), serializedData.size());

    if (type == SerializationType::FoundationToCleonDataRequest) {
        return serializeMessage<FC, FC_fields>(data, serializedData);
    }
    else if (type == SerializationType::CleonToFoundationDataReply) {
        return serializeMessage<CF, CF_fields>(data, serializedData);
    }
    else if (type == SerializationType::FoundationToCloudSyncRequest) {
        return serializeMessage<F0RX, F0RX_fields>(data, serializedData);
    }
    else if (type == SerializationType::CloudToFoundationSyncRequest || type == SerializationType::FoundationToCleonSyncRequest) {
        return serializeMessage<F0TX, F0TX_fields>(data, serializedData);
    }
    else if (type == SerializationType::FoundationToCloudSyncAcknowledge) {
        return serializeMessage<F1, F1_fields>(data, serializedData);
    }
    else if (type == SerializationType::TelemetryUpdate) {
        return serializeMessage<BA, BA_fields>(data, serializedData);
    }
    else {
        return ErrorType::NotSupported;
    }
}

// Helper template function for deserialization
template <typename MessageType, const pb_msgdesc_t* Fields>
ErrorType deserializeMessage(const std::string& serializedData, std::string& data) {

    // Initialize input stream for deserialization
    pb_istream_t iStream = pb_istream_from_buffer(reinterpret_cast<const pb_byte_t*>(serializedData.data()), serializedData.size());

    // Decode message
    MessageType message = MessageType();
    if (!pb_decode(&iStream, Fields, &message)) {
        return ErrorType::Failure;
    }

    // Resize data to actual size used
    data.resize(sizeof(message));

    // Initialize output stream for serialization
    pb_ostream_t oStream = pb_ostream_from_buffer(reinterpret_cast<pb_byte_t*>(data.data()), data.size());

    // Encode message
    if (!pb_encode(&oStream, Fields, &message)) {
        return ErrorType::Failure;
    }

    // Compare original serialized data and re-serialized data
    if (std::memcmp(serializedData.data(), data.data(), serializedData.size()) != 0) {
        return ErrorType::InvalidParameter;
    }

    // Copy message to output data parameter
    data.assign(reinterpret_cast<char*>(&message), sizeof(MessageType));

    return ErrorType::Success;
}

ErrorType Serializer::deserialize(const std::string &serializedData, std::string &data, SerializationType type) {

    // Return failure if input is empty
    if (serializedData.empty()) {
        return ErrorType::Failure;
    }

    pb_istream_t iStream;
    iStream = pb_istream_from_buffer(reinterpret_cast<const pb_byte_t*>(serializedData.data()), serializedData.size());

    if (type == SerializationType::FoundationToCleonDataRequest) {
        return deserializeMessage<FC, FC_fields>(serializedData, data);
    }
    else if (type == SerializationType::CleonToFoundationDataReply) {
        return deserializeMessage<CF, CF_fields>(serializedData, data);
    }
    else if (type == SerializationType::FoundationToCloudSyncRequest) {
        return deserializeMessage<F0RX, F0RX_fields>(serializedData, data);
    }
    else if (type == SerializationType::CloudToFoundationSyncRequest || type == SerializationType::FoundationToCleonSyncRequest) {
        return deserializeMessage<F0TX, F0TX_fields>(serializedData, data);
    }
    else if (type == SerializationType::FoundationToCloudSyncAcknowledge) {
        return deserializeMessage<F1, F1_fields>(serializedData, data);
    }
    else if (type == SerializationType::TelemetryUpdate) {
        return deserializeMessage<BA, BA_fields>(serializedData, data);
    }
    else {
        return ErrorType::NotSupported;
    }
}

ErrorType Serializer::maxSerializedSize(Bytes &maxSize, SerializationType type) {
    if (type == SerializationType::FoundationToCleonDataRequest) {
        maxSize = FC_size;
        return ErrorType::Success;
    }
    else if (type == SerializationType::CleonToFoundationDataReply) {
        maxSize = CF_size;
        return ErrorType::Success;
    }
    else if (type == SerializationType::FoundationToCloudSyncRequest) {
        //Just gonna guess here. I don't know why the proto doesn't have F0RX_size
        maxSize = sizeof(F0RX) + 512;
        return ErrorType::Success;
    }
    else if (type == SerializationType::CloudToFoundationSyncRequest || type == SerializationType::FoundationToCleonSyncRequest) {
        maxSize = F0TX_size + 512;
        return ErrorType::Success;
    }
    else if (type == SerializationType::FoundationToCloudSyncAcknowledge) {
        maxSize = sizeof(F1) + 512;
        return ErrorType::Success;
    }
    else if (type == SerializationType::TelemetryUpdate) {
        maxSize = BA_size;
        return ErrorType::Success;
    }
    else {
        return ErrorType::NotSupported;
    }
}

ErrorType Serializer::maxDeserializedSize(Bytes &maxSize, SerializationType type) {
    if (type == SerializationType::FoundationToCleonDataRequest) {
        maxSize = sizeof(FC);
        return ErrorType::Success;
    }
    else if (type == SerializationType::CleonToFoundationDataReply) {
        maxSize = sizeof(CF);
        return ErrorType::Success;
    }
    else if (type == SerializationType::FoundationToCloudSyncRequest) {
        maxSize = sizeof(F0RX);
        return ErrorType::Success;
    }
    else if (type == SerializationType::CloudToFoundationSyncRequest || type == SerializationType::FoundationToCleonSyncRequest) {
        maxSize = sizeof(F0TX);
        return ErrorType::Success;
    }
    else if (type == SerializationType::FoundationToCloudSyncAcknowledge) {
        maxSize = sizeof(F1);
        return ErrorType::Success;
    }
    else if (type == SerializationType::TelemetryUpdate) {
        maxSize = sizeof(BA);
        return ErrorType::Success;
    }
    else {
        return ErrorType::NotSupported;
    }
}