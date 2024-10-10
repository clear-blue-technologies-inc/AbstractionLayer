//Modules
#include "StorageModule.hpp"
//ESP
#include "nvs.h"
#include "nvs_flash.h"

ErrorType Storage::initStorage() {
    std::unique_ptr<EventAbstraction> event = std::make_unique<Event<Storage>>(std::bind(&Storage::initStorageInternal, this));
    return addEvent(event);
} 

ErrorType Storage::deinitStorage() {
    std::unique_ptr<EventAbstraction> event = std::make_unique<Event<Storage>>(std::bind(&Storage::deinitStorageInternal, this));
    return addEvent(event);
} 

ErrorType Storage::maxStorageSize(Bytes &size, std::string partitionName) {
    nvs_stats_t stats;
    esp_err_t err = nvs_get_stats(NULL, &stats);

    //An entry is a key-pair value, and 1 entry is equal to the smallest unit we can store (1 byte)
    size = stats.total_entries;
    
    return toPlatformError(err);
}

ErrorType Storage::availableStorage(Bytes &size, std::string partitionName) {
    nvs_stats_t stats;
    esp_err_t err = nvs_get_stats(NULL, &stats);

    size = stats.free_entries;

    return toPlatformError(err);
}

ErrorType Storage::maxRamSize(Bytes &size, std::string memoryRegionName) {
    return ErrorType::NotImplemented;
}

ErrorType Storage::availableRam(Bytes &size, std::string memoryRegionName) {
    return ErrorType::NotImplemented;
}

ErrorType Storage::erasePartition(const std::string &partitionName) {
    std::unique_ptr<EventAbstraction> event = std::make_unique<Event<Storage, const std::string &>>(std::bind(&Storage::erasePartitionInternal, this, std::placeholders::_1), partitionName);
    return addEvent(event);
}

ErrorType Storage::eraseAllPartitions() {
    std::unique_ptr<EventAbstraction> event = std::make_unique<Event<Storage>>(std::bind(&Storage::eraseAllPartitionsInternal, this));
    return addEvent(event);
}

ErrorType Storage::mainLoop() {
    ErrorType error;

    if (ErrorType::NoData == (error = runNextEvent())) {
        return error;
    }
    else if (ErrorType::Success == error) {
        return error;
    }

    return ErrorType::Failure;
}

ErrorType Storage::initStorageInternal() {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        //TODO: What should I do here? Try to ship off data to the cloud before resetting? Does Mark support that?
        return toPlatformError(err);
    }

    _nvsHandle = nvs::open_nvs_handle(name().c_str(), NVS_READWRITE, &err);
    if (err != ESP_OK) {
        return toPlatformError(err);
    }

    _status.isInitialized = true;
    return toPlatformError(err);
} 
    
ErrorType Storage::deinitStorageInternal() {
        if (_nvsHandle) {
            _nvsHandle.release();
        }

        return toPlatformError(nvs_flash_deinit());
} 
    
ErrorType Storage::erasePartitionInternal(const std::string &partitionName) {
    return toPlatformError(nvs_flash_erase_partition(partitionName.c_str()));
}

ErrorType Storage::eraseAllPartitionsInternal() {
    return toPlatformError(_nvsHandle->erase_all());
}