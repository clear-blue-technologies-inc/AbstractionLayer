#ifndef __STORAGE_MODULE_HPP__
#define __STORAGE_MODULE_HPP__

//AbstractionLayer
#include "StorageAbstraction.hpp"
//Common
#include "Error.hpp"
//Modules
#include "Global.hpp"

class Storage : public StorageAbstraction, public Global<Storage, std::string>{
    public:
    Storage(std::string name) : StorageAbstraction(name) {
        _status.isInitialized = false;
    }
    virtual ~Storage() = default;

    ErrorType initStorage() override;
    ErrorType deinitStorage() override;
    ErrorType maxStorageSize(Bytes &size) override;
    ErrorType availableStorage(Bytes &size) override;
    ErrorType erasePartition(const std::string &partitionName) override;
    ErrorType eraseAllPartitions() override;

    ErrorType mainLoop() override;
};

#endif //__STORAGE_MODULE_HPP__