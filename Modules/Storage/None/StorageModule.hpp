#ifndef __STORAGE_MODULE_HPP__
#define __STORAGE_MODULE_HPP__

//AbstractionLayer
#include "StorageAbstraction.hpp"
//Utilities
#include "Error.hpp"
#include "Global.hpp"
#include "EventQueue.hpp"

class Storage : public StorageAbstraction, public Global<Storage, std::string>, public EventQueue {
    public:
    Storage(std::string name) : StorageAbstraction(name) {
        _status.isInitialized = false;
    }
    virtual ~Storage() = default;

    ErrorType initStorage() override;
    ErrorType deinitStorage() override;
    ErrorType maxStorageSize(Bytes &size, std::string partitionName) override;
    ErrorType availableStorage(Bytes &size, std::string partitionName) override;
    ErrorType maxRamSize(Bytes &size, std::string memoryRegionName) override;
    ErrorType availableRam(Bytes &size, std::string memoryRegionName) override;
    ErrorType erasePartition(const std::string &partitionName) override;
    ErrorType eraseAllPartitions() override;

    ErrorType mainLoop() override;
};

#endif //__STORAGE_MODULE_HPP__