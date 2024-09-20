/**************************************************************************//**
* @author Ben Haubrich                                        
* @file   CommandObject.hpp
* @details \b Synopsis: \n Command object that can be processed by a processing object.
*******************************************************************************/
#ifndef __COMMAND_OBJECT_HPP__
#define __COMMAND_OBJECT_HPP__

//AbstractionLayer
#include "Types.hpp"
//C++
#include <string>

/**
 * @enum LogicSignature
 * @brief The logic signature tells the chain of responsibility where the command object should be processed.
 *        This is meant to be a shortcut so that we can avoid search times of worker threads searching through
 *        the chain looking for objects that they want to process. Using the signature, we can place them individually
 *        into their own chains so that worker threads know exactly where to look to find command objects to process.
*/
enum class LogicSignature {
    Unknown = 0,       ///< The signature is unknown.
    NetworkData = 1,   ///< The data is network data and origingate from the newtwork.
    StorageData = 2,   ///< The data is storage data and originates from storage.
    CleonData = 3,     ///< The data is Cleon data and originates from cleon.
    FoundationData = 4 ///< The data is Foundation data and originates from Foundation.
};

/**
 * @class CommandObject
 * @brief Command object that can be processed by a processing object.
*/
class CommandObject {
    public:
    /**
     * @brief Constructor.
     * @param signature The logic signature of this command object. Processing objects compare against this to see if they can process it.
     */
    CommandObject(LogicSignature signature) : _logicSignature(signature) {}
    /// @brief Destructor.
    virtual ~CommandObject() = default;

    /// @brief Get the logic signature constant.
    LogicSignature logicSignatureConst() const { return _logicSignature; }
    /// @brief Get the logic signature reference
    LogicSignature &logicSignature() { return _logicSignature; }

    private:
    /// @brief The logic signature of the command object
    LogicSignature _logicSignature = LogicSignature::Unknown;
};

#endif // __COMMAND_OBJECT_HPP__
