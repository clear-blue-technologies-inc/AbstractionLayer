/**************************************************************************//**
* @author Ben Haubrich                                        
* @file   StorageAbstraction.hpp
* @details \b Synopsis: \n Foundation core functions for any storage type
* @ingroup AbstractionLayer
******************************************************************************/
#ifndef __STORAGE_ABSTRACTION_HPP__
#define __STORAGE_ABSTRACTION_HPP__

//Foundation
#include "Types.hpp"
#include "EventQueue.hpp"
//Modules
#include "Error.hpp"
//C++
#include <string>
#include <memory>

/**
 * @struct StorageStatus
 * @brief The status of the storage.
*/
struct StorageStatus {
    bool isInitialized; ///< Is the storage initialized and ready to use.
};

/**
 * @class StorageAbstraction
 * @brief Foundation core functions for any storage type
*/
class StorageAbstraction : public EventQueue {

    public:
    /**
     * @brief Constructor
     * @param [in] name The name of the storage
    */
    StorageAbstraction(std::string name) :  _name(name) {}
    /// @brief Destructor
    virtual ~StorageAbstraction() = default;

    /**
     * @brief Initializes the storage
     * @returns ErrorType::Success if initiatized.\n
     * @returns ErrorType::Failure if failed.\n
     * @returns ErrorType::NotImplemented if not be implemented.\n
     * @returns ErrorType::NotAvailable if the storage does not need to be initialized.\n
    */
    virtual ErrorType initStorage() = 0;
    /**
     * @brief Deinitializes the storage
     * @returns ErrorType::Success if the storage was deinitialized.
     * @returns ErrorType::Failure if it was not deinitialized.
    */
    virtual ErrorType deinitStorage() = 0;
    /**
     * @brief Get the size of the storage
     * @param [out] size The size of the storage
     * @returns ErrorType::Success if the operation is successful, otherwise an error code
     * @returns ErrorType::NotImplemented if not be implemented
     * @returns ErrorType::Failure for any other failure that prevents the operation from returning the max storage size to size
     */
    virtual ErrorType maxStorageSize(Bytes &size) = 0;
    /**
     * @brief Get the size of the storage
     * @param [out] size The size of the storage
     * @pre initStorage must be called first
     * @returns ErrorType::Success if the operation is successful, otherwise an error code
     * @returns ErrorType::PrerequisiteNotMet if initStorage has not been called
     * @returns ErrorType::NotImplemented if not implemented
     * @returns ErrorType::Failure for any other failure that prevents the operation from returning the available storage size to size
     * 
     */
    virtual ErrorType availableStorage(Bytes &size) = 0;
    /**
     * @brief erase the specified partition
     * @param[in] partitionName The name of the partition to erase
     * @returns ErrorType::Success if the operation is successful.
     * @returns ErrorType::Failure if the operation fails.
    */
    virtual ErrorType erasePartition(const std::string &partitionName) = 0;
    /**
     * @brief erase all partitions
     * @returns ErrorType::Success if the operation is successful.
     * @returns ErrorType::Failure if the operation fails.
    */
    virtual ErrorType eraseAllPartitions() = 0;

    /// @brief Get the status of the storage
    StorageStatus status() const { return _status; }
    /// @brief Get the name of the storage
    std::string name() const { return _name; }
    /// @brief Get the root prefix
    std::string rootPrefix() const { return _rootPrefix; }

    /// @brief Tag for logging
    static constexpr char TAG[] = "Storage";

    private:
    /// @brief The name of the storage.
    std::string _name;

    protected:
    /// @brief The status of the storage.
    StorageStatus _status = { .isInitialized = false };
    /// @brief A path that is prepened to all file names for storage
    std::string _rootPrefix;
};

#endif //__STORAGE_ABSTRACTION_HPP__