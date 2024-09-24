//Common
#include "Log.hpp"
//AbstractionLayer
#include "Error.hpp"
//C++
#include <cerrno>

ErrorType toPlatformError(int32_t err) {

    switch (err) {
        case -1:
            return ErrorType::Failure;
        case 0:
            return ErrorType::Success;
        case ENOMEM:
            return ErrorType::NoMemory;
        case ETIME:
        case ETIMEDOUT:
            return ErrorType::Timeout;
        case ENOSYS:
            return ErrorType::NotSupported;
        case EACCES:
        case EPERM:
        case EDEADLK:
            return ErrorType::PrerequisitesNotMet;
        case EINVAL:
        case ENOTSOCK:
        case EBADF:
        case ENAMETOOLONG:
            return ErrorType::InvalidParameter;
        case ENOENT:
            return ErrorType::FileNotFound;
        case EEXIST:
            return ErrorType::FileExists;
        case ENODATA:
        case ESRCH:
            return ErrorType::NoData;
        case EAGAIN:
        case EOVERFLOW:
            return ErrorType::LimitReached;
        default:
            CBT_LOGW("ErrnoError", "Got unhandled error code %d", err);
            return ErrorType::Failure;
    }
}