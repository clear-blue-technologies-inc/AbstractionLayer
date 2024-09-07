#ifndef __STORAGE_MODULE_HPP__
#define __STORAGE_MODULE_HPP__

//Utilities
#include "Global.hpp"
#include "Error.hpp"
#include "EventQueue.hpp"
//AbstractionLayer
#include "StorageAbstraction.hpp"
#include "OperatingSystemAbstraction.hpp"
//ESP
#include "nvs_handle.hpp"

/**
 * @class Storage
*/
class Storage : public StorageAbstraction, public Global<Storage, std::string>, public EventQueue{

    public:
    /**
     * @brief Constructor
     * @param[in] name The name of the storage
     * @param[in] os The operating system
     * @attention Stdlib version is a Global class that is constructed with Init()
    */
    Storage(std::string name) : StorageAbstraction(name) {
        _status.isInitialized = false;
    }
    virtual ~Storage() = default;

    /**
     * @attention Returns ErrorType::NotAvailable since we assume the system that has stdlib also has an operating system
     * to init the storage for you.
    */
    ErrorType initStorage() override;
    /**
     * @attention Returns ErrorType::NotAvailable since we assume the system that has stdlib also has an operating system
     * to deinit the storage for you.
    */
    ErrorType deinitStorage() override;
    ErrorType maxStorageSize(Bytes &size) override;
    ErrorType availableStorage(Bytes &size) override;
    ErrorType erasePartition(const std::string &partitionName) override;
    ErrorType eraseAllPartitions() override;

    ErrorType mainLoop() override;

    private:
    std::unique_ptr<nvs::NVSHandle> _nvsHandle;

    ErrorType initStorageInternal();
    ErrorType deinitStorageInternal();
    ErrorType maxStorageSizeInternal(Bytes &size);
    ErrorType availableStorageInternal(Bytes &size);
    ErrorType erasePartitionInternal(const std::string &partitionName);
    ErrorType eraseAllPartitionsInternal();

};

#endif //__CBT_FLASH_HPP__