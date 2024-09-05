#ifndef __CYCLIC_REDUNDANCY_CHECK_MODULE_HPP__
#define __CYCLIC_REDUNDANCY_CHECK_MODULE_HPP__

#include "CbtError.hpp"
#include "CbtTypes.hpp"

namespace Crc {

    ErrorType crc8LittleEndian(uint8_t initialCrc, const std::string data, uint8_t &result);
    ErrorType crc8BigEndian(uint8_t initialCrc, const std::string data, uint8_t &result);

    ErrorType crc16LittleEndian(uint16_t initialCrc, const std::string data, uint16_t &result);
    ErrorType crc16BigEndian(uint16_t initialCrc, const std::string data, uint16_t &result);

    ErrorType crc32LittleEndian(uint32_t initialCrc, const std::string data, uint32_t &result);
    ErrorType crc32BigEndian(uint32_t initialCrc, const std::string data, uint32_t &result);
}

#endif // __CYCLIC_REDUNDANCY_CHECK_MODULE_HPP__