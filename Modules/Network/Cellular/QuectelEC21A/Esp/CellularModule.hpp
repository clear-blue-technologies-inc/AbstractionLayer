/***************************************************************************//**
* @author   Ben Haubrich
* @file     CellularModule.hpp
* @details  Cellular for Quectel EC21A and ESP
* @ingroup  NoneModules
*******************************************************************************/
#ifndef __CELLULAR_MODULE_HPP__
#define __CELLULAR_MODULE_HPP__

#include "CellularAbstraction.hpp"
#include "GpioModule.hpp"

class Cellular : public CellularAbstraction {
    public:
    Cellular() : CellularAbstraction() {
        _status.isUp = false;
        _status.technology = NetworkTypes::Technology::Cellular;
    }
    ~Cellular() = default;

    ErrorType init() override;
    ErrorType networkUp() override;
    ErrorType networkDown() override;
    
    ErrorType txBlocking(const std::string &frame, const Milliseconds timeout) override;
    ErrorType txNonBlocking(const std::shared_ptr<std::string> frame, std::function<void(const ErrorType error, const Bytes bytesWritten)> callback = nullptr) override;
    ErrorType rxBlocking(std::string &frameBuffer, const Milliseconds timeout) override;
    ErrorType rxNonBlocking(std::shared_ptr<std::string> frameBuffer, std::function<void(const ErrorType error, std::shared_ptr<std::string> frameBuffer)> callback = nullptr) override;
    ErrorType getMacAddress(std::string &macAddress) override;
    ErrorType getSignalStrength(DecibelMilliWatts &signalStrength) override;

    ErrorType mainLoop() override;

    ErrorType reset() override;

    private:
    /**
     * @brief The context for the IP socket.
     * @details Pg. 11 Quectel LTE Standard TCP/IP Application Note. There are a maximum of 3 contexts available.
     *          Each context can open 11 connection IDs (or sockets)
     */
    static constexpr Id _IpContext = 1;
    /**
     * @brief The context for the HTTP socket.
     * @sa ipContext
     */
    static constexpr Id _HttpContext = 2;
    /// @brief The maximum number of contexts.
    static constexpr Id _MaxContexts = 3;
    /// @brief The maximum number of sockets per context.
    static constexpr Id _MaxSocketsPerContext = 11;
    /// @brief The GPIO pin for the reset pin.
    std::unique_ptr<Gpio> _gpioReset;
    /// @brief The line termination character. Set to the default on Pg. 23 of the EC21A AT Command Manual.
    char _commandLineTerminationCharacter = '\r';
    /// @brief The response formatting character. Set to the default on Pg. 24 of the EC21A AT Command Manual.
    char _responseFormattingCharacter = '\n';

    /**
     * @brief Send an AT command to the modem and wait for a response.
     * @param[in] atCommand The AT command to send.
     * @param[in] timeout The timeout for the command.
     * @param[in] maxRetries The maximum number of retries to send the command.
     * @param[out] responseBuffer The buffer to store the response in.
     * @returns ErrorType::Success if the command was sent and a response was received.
     * @returns ErrorType::Failure if the command was not sent or no response was received.
    */
    ErrorType sendCommand(const std::string &atCommand, const Milliseconds timeout, const Count maxRetries);

    /**
     * @brief Receive a response from the modem.
     * @param[out] responseBuffer The buffer to store the response in.
     * @param[in] timeout The timeout for the response.
     * @param[in] maxRetries The maximum number of retries to receive the response.
     * @param[in] expectedResponse Optional. The expected response from the modem. If not provided, this function will ensure that the response ends with the response formatting character.
     *            but will not check for response messages such as "OK" or "ERROR".
     * @returns ErrorType::Success if the response was received.
     * @returns ErrorType::Failure if the response was not received.
    */
    ErrorType receiveCommand(std::string &responseBuffer, const Milliseconds timeout, const Count maxRetries, const std::string expectedResponse = std::string());

    /**
     * @brief Get the manufacturer name of the modem.
     * @param[out] mfgName The string to store the manufacturer name in.
     * @returns ErrorType::Success if the manufacturer name was retrieved successfully.
     * @returns ErrorType::Failure if the manufacturer name was not retrieved successfully.
    */
    ErrorType getManufacturerName(std::string &mfgName);

    /**
     * @brief Check if the SIM card is inserted.
     * @returns ErrorType::Success if the SIM card is inserted.
     * @returns ErrorType::Failure if the SIM card is not inserted. 
    */
    ErrorType simCardIsInserted();
    
    /**
     * @brief Set the echo mode of the modem.
     * @details Controls whether the modem will echo back the characters it receives. See Pg. 22 of the EC21A AT Command Manual.
     * @param[in] enable True to enable echo mode, false to disable.
     * @returns ErrorType::Success if the echo mode was set successfully.
     * @returns ErrorType::Failure if the echo mode was not set successfully.
    */
    ErrorType echoMode(const bool enable);

    /**
     * @brief terminatingCharacter
     * @details Pg. 23 EC21A AT Command Manual
     * @param[out] terminatingCharacter The character that the modem will use to terminate the response.
     * @returns ErrorType::Success if the terminating character was retrieved successfully.
     * @returns ErrorType::Failure if the terminating character was not retrieved successfully.
    */
    ErrorType terminatingCharacter(char &terminatingCharacter);

    /**
     * @brief responseFormattingCharacter
     * @details Pg. 24 EC21A AT Command Manual
     * @param[out] responseFormattingCharacter The character that the modem will use to format the response.
     * @returns ErrorType::Success if the response formatting character was retrieved successfully.
     * @returns ErrorType::Failure if the response formatting character was not retrieved successfully.
    */
    ErrorType responseFormattingCharacter(char &responseFormattingCharacter);
};

#endif // __CELLULAR_MODULE_HPP__