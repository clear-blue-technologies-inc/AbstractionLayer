/**************************************************************************//**
* @author Ben Haubrich                                        
* @file   Error.hpp
* @details \b Synopsis: \n Error types.
* @ingroup AbstractionLayer
*******************************************************************************/

#ifndef __ERROR_HPP__
#define __ERROR_HPP__

#include <stdint.h>

/**
 * @enum ErrorType
 * @brief Error types
*/
enum class ErrorType : uint8_t {
    Success             = 0,    ///< Nothing to worry about. Move on!
    Failure             = 1,    ///< General failure. Try to be more specific if you can.
    NotImplemented      = 2,    ///< For software that returns because there is no code for it, but there could be code if you just wrote it.
    NoMemory            = 3,    ///< Not enough memory to complete the operation
    Timeout             = 4,    ///< Operation timed out
    NotSupported        = 5,    ///< The operation is implemented, but doesn't support the provided data types. You couldn't implement the code even if you wanted to.
    PrerequisitesNotMet = 6,    ///< One or more prerequisites are not met. Read the inline documentation for more information.
    InvalidParameter    = 7,    ///< One or more parameters are invalid. Read the inline documentation for more information.
    CrcMismatch         = 8,    ///< Cyclic Redundancy Check failed.
    NotAvailable        = 9,    ///< The operation makes sense to be implemented, but the underlying software doesn't support this operation.
    FileNotFound        = 10,   ///< File not found.
    FileExists          = 11,   ///< File already exists.
    EndOfFile           = 12,   ///< End of file reached.
    NoData              = 13,   ///< No data available.
    LimitReached        = 14    ///< An imposed limit has been reached.
};

/**
 * @brief Converts an integer error code to an ErrorType
 * @param error The error code to convert 
 * @returns The ErrorType corresponding to the error code 
 */
ErrorType toPlatformError(int32_t error);

#endif // __ERROR_HPP__
