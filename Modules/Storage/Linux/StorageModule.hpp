#ifndef __STORAGE_MODULE_HPP__
#define __STORAGE_MODULE_HPP__

//AbstractionLayer applications
#include "Global.hpp"
#include "EventQueue.hpp"
//AbstractionLayer
#include "StorageAbstraction.hpp"

/**
 * @class Storage
*/
class Storage : public StorageAbstraction, public Global<Storage, std::string>, public EventQueue {

    public:
    Storage(std::string name) : StorageAbstraction(name) {
        _status.isInitialized = false;
    }
    virtual ~Storage() = default;

    ErrorType initStorage() override;
    ErrorType deinitStorage() override;
    ErrorType maxStorageSize(Bytes &size, std::string partitionName = std::string()) override;
    ErrorType availableStorage(Bytes &size, std::string partitionName = std::string()) override;
    ErrorType maxRamSize(Bytes &size, std::string memoryRegionName = std::string()) override;
    ErrorType availableRam(Bytes &size, std::string memoryRegionName = std::string()) override;
    ErrorType erasePartition(const std::string &partitionName) override;
    ErrorType eraseAllPartitions() override;

    ErrorType mainLoop() override;

    private:
    ErrorType deinitStorageInternal();
    ErrorType maxStorageSizeInternal(Bytes &size, std::string partitionName);
    ErrorType availableStorageInternal(Bytes &size, std::string partitionName);
    ErrorType erasePartitionInternal(const std::string &partitionName);
    ErrorType eraseAllPartitionsInternal();

    std::string getEnvironment(std::string variable, ErrorType &error);
};

#endif //__STORAGE_MODULE_HPP__