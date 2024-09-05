//Modules
#include "StorageModule.hpp"
//Common
#include "Error.hpp"

ErrorType Storage::initStorage() {
    return ErrorType::NotImplemented;
} 
ErrorType Storage::deinitStorage() {
    return ErrorType::NotImplemented;
} 
ErrorType Storage::maxStorageSize(Bytes &size) {
    return ErrorType::NotImplemented;
}
ErrorType Storage::availableStorage(Bytes &size) {
    return ErrorType::NotImplemented;
}
ErrorType Storage::erasePartition(const std::string &partitionName) {
    return ErrorType::NotImplemented;
}
ErrorType Storage::eraseAllPartitions() {
    return ErrorType::NotImplemented;
}
ErrorType Storage::mainLoop() {
    return ErrorType::NotImplemented;
}