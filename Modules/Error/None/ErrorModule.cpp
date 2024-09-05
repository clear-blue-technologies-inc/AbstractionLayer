//AbstractionLayer
#include "Error.hpp"

ErrorType toPlatformError(int32_t err) {

    switch (err) {
        default:
            return ErrorType::NotImplemented;
    }
}