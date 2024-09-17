/**************************************************************************//**
* @author Ben Haubrich                                        
* @file   OperatingSystemAbstraction.hpp
* @details \b Synopsis: \n Encapsulates the operating system. Provides info and functions.
           base class.
* @ingroup AbstractionLayer
*******************************************************************************/
#ifndef __OPERATING_SYSTEM_ABSTRACTION_HPP__
#define __OPERATING_SYSTEM_ABSTRACTION_HPP__

//AbstractionLayer
#include "Types.hpp"
#include "Error.hpp"
//C++
#include <functional>
#include <string>

namespace OperatingSystemConfig {

    /**
     * @enum Priority
     * @brief The priority of a thread. Determines the preference of the scheduler for giving this thread CPU time.
    */
    enum class Priority : uint8_t {
        Unknown = 0, ///< Unknown priority
        Highest,     ///< Highest priority
        High,        ///< High priority
        Normal,      ///< Normal priority
        Low,         ///< Low priority
        Lowest       ///< Lowest priority
    };

    /**
     * @enum ResetReason
     * @brief The reason the processor was reset.
    */
    enum class ResetReason : uint8_t {
        Unknown,     //!< Reset reason can not be determined
        PowerOn,     //!< Reset due to power-on event
        ExternalPin, //!< Reset by external pin (not applicable for ESP32)
        Software,    //!< Software reset via esp_restart
        Exception,   //!< Software reset due to exception/panic
        Watchdog,    //!< Reset (software or hardware) due to interrupt watchdog
        DeepSleep,   //!< Reset after exiting deep sleep mode
        BrownOut,    //!< Brownout reset (software or hardware)
        Sdio         //!< Reset over SDIO
    };
}

/**
 * @class OperatingSystemAbstraction
 * @brief An interface for the operating system.
*/
class OperatingSystemAbstraction {

    public:
    OperatingSystemAbstraction() = default;
    virtual ~OperatingSystemAbstraction() = default;

    static constexpr Count MaxThreads = 256; ///< The maximum number of threads.
    static constexpr char TAG[] = "CbtOperatingSystem"; ///< The tag for logging.
    static constexpr Count MaxCountingSemaphore = 10; ///< The maximum value for a counting semaphore.

    /**
     * @brief delays a thread by placing it in the blocking state.
     * @param[in] delay The amount of time to block for.
     * @returns ErrorType::Success if the thread is successfully blocked.
    */
    virtual ErrorType delay(Milliseconds delay) = 0;
    /**
     * @brief Create a new thread.
     * @param[in] priority The priority of the new thread.
     * @param[in] name The name of the thread.
     * @param[in] arguments The argument to pass to the thread.
     * @param[in] stackSize The stack size of the thread.
     * @param[in] startFunction The function called by the operating system to start the thread.
     * @param[out] number The id of the new thread.
     * @returns ErrorType::Success if the thread is successfully created.
     * @returns ErrorType::NotImplemented if createThread is not implemented.
     * @returns ErrorType::LimitReached if the maximum number of threads is reached.
     * @returns toPlatformError for all other errors produced by the underlying implementation
    */
    virtual ErrorType createThread(OperatingSystemConfig::Priority priority, std::string name, void * arguments, Bytes stackSize, void *(*startFunction)(void *), Id &number) = 0;
    /**
     * @brief Delete a thread
     * @param[in] name The name of the thread to delete.
    */
    virtual ErrorType deleteThread(std::string name) = 0;
    /**
     * @brief Blocks the calling thread until the thread given by name terminates
     * @param[in] name The name of the thread to wait for.
     * @returns ErrorType::Success if the thread is successfully created.
     * @returns ErrorType::NotImplemented if joinThread is not implemented.
     * @returns ErrorType::NoData if no thread with the name given has been created.
     * @returns toPlatformError() for all other errors produced by the underlying implementation
    */
    virtual ErrorType joinThread(std::string name) = 0;
    /**
     * @brief Gets the Id of the thread given by name.
     * @param[in] name The name of the thread.
     * @param[out] id The id of the thread.
     * @returns ErrorType::Success if the thread Id was found.
     * @returns ErrorType::NotImplemented if threadId is not implemented
     * @returns ErrorType::NoData if no thread with the name given has been created.
     * @returns toPlatformError() for all other errors produced by the underlying implementation
    */
    virtual ErrorType threadId(std::string name, Id &id) = 0;
    /**
     * @brief Check if the thread has been deleted by the operating system.
     * @details The operating system keeps a record of the threads it has created. Since the OperatingSystemAbstraction is decoupled from the main application,
     *          it is not able to delete threads directly since it's not possible to tell when it is safe to delete a thread. Instead, it marks the thread as deleted
     *          so that the main application can check if it's been deleted and then delete the thread when it's safe to do so.
     * @param[in] name The name of the thread.
     * @returns ErrorType::Success if the thread has been not deleted.
     * @returns ErrorType::NotImplemented if isDeleted is not implemented.
     * @returns ErrorType::NoData if no thread with the name given is deleted.
    */
    virtual ErrorType isDeleted(std::string &name) = 0;
    /**
     * @brief creates a semaphore.
     * @param[in] max The maximum value of the semaphore.
     * @param[in] initial The initial value of the semaphore.
     * @param[in] name The name of the semaphore.
    */
    virtual ErrorType createSemaphore(Count max, Count initial, std::string name) = 0;
    /**
     * @brief deletes a semaphore.
     * @param[in] name The name of the semaphore.
     * @returns ErrorType::Success if the semaphore was deleted.
     * @returns ErrorType::NoData if the semaphore does not exist.
    */
    virtual ErrorType deleteSemaphore(std::string name) = 0;
    /**
     * @brief waits for a semaphore.
     * @param[in] name The name of the semaphore.
     * @param[in] timeout The amount of time to wait for the semaphore.
     * @returns ErrorType::Timeout if the semaphore couldn't be decremented within the timeout likely because the value is already zero.
     * @returns ErrorType::Success if the was decremented.
     * @returns ErrorType::NoData if the semaphore does not exist.
    */
    virtual ErrorType waitSemaphore(std::string name, Milliseconds timeout) = 0;
    /**
     * @brief increments a semaphore.
     * @param[in] name The name of the semaphore.
     * @returns ErrorType::Success if the semaphore was incremented
     * @returns ErrorType::NoData if the semaphore does not exist.
    */
    virtual ErrorType incrementSemaphore(std::string name) = 0;
    /**
     * @brief decrements a semaphore.
     * @param[in] name The name of the semaphore.
     * @returns ErrorType::Success if the semaphore was incremented
     * @returns ErrorType::NoData if the semaphore does not exist.
    */
    virtual ErrorType decrementSemaphore(std::string name) = 0;
    /**
     * @brief Create a timer.
     * @param[out] timer The id of the timer.
     * @param[in] period The period of the timer.
     * @param[in] autoReload Whether the timer should start again automatically after it's timed out.
     * @param[in] callback The function to call when the timer times out.
     * @returns ErrorType::Success if the timer is successfully created.
     * @returns ErrorType::Failure if the timer could not be created.
    */
    virtual ErrorType createTimer(Id &timer, Milliseconds period, bool autoReload, std::function<void(void)> callback) = 0;
    /**
     * @brief start timer.
     * @param[in] timer The id of the timer.
     * @param[in] timeout The time to wait for the timer to start.
     * @returns ErrorType::Success if the timer could be started.
     * @returns ErrorType::Timeout if the timer could not be started within the time specified.
    */
    virtual ErrorType startTimer(Id timer, Milliseconds timeout) = 0;
    /**
     * @brief stop timer.
     * @param[in] timer The id of the timer.
     * @param[in] timeout The time to wait for the timer to stop.
     * @returns ErrorType::Success if the timer could be stopped.
     * @returns ErrorType::Timeout if the timer could not be stopped within the time specified.
    */
    virtual ErrorType stopTimer(Id timer, Milliseconds timeout) = 0;
    /**
     * @brief Get the current system time
     * @param[out] currentSystemUnixTime The current unix time of the system.
     * @returns ErrorType::Success if the system unix time was obtained
     * @returns ErrorType::NotImplemented if getting the system time is not implemented
    */
    virtual ErrorType getSystemTime(UnixTime &currentSystemUnixTime) = 0;
    /**
     * @brief Get the current system tick value
     * @param[out] currentSystemTicks The current tick value of the system.
     * @returns ErrorType::Success if the system tick value was obtained
     * @returns ErrorType::NotImplemented if getting the system tick value is not implemented
     * @returns ErrorType::Failure if the system tick value could not be obtained
     */
    virtual ErrorType getSystemTick(Ticks &currentSystemTicks) = 0;
    /**
     * @brief Convert a tick value to a time in milliseconds
     * @param[in] ticks The tick value to convert.
     * @param[out] tickEquivalent The converted tick value.
     * @returns ErrorType::Success if the tick was converted to milliseconds
     * @returns ErrorType::NotImplemented if converting the tick to milliseconds is not implemented
     * @returns ErrorType::Failure if the tick could not be converted to milliseconds
     */
    virtual ErrorType ticksToMilliseconds(Ticks ticks, Milliseconds &tickEquivalent) = 0;
    /**
     * @brief Get software version
     * @param[out] softwareVersion The software version in the format a.b.c.d, where a, b, c, and d
     *             are the version numbers in the range (0,10].
     * @returns ErrorType::Success if the software version was obtained
     * @returns ErrorType::NotImplemented if getting the software version is not implemented
    */
    virtual ErrorType getSoftwareVersion(std::string &softwareVersion) = 0;
    /**
     * @brief Get the reset reason from the last time the processor was reset.
     * @param[out] resetReason The reset reason.
     * @returns ErrorType::Success if the reset reason was obtained
     * @returns ErrorType::NotImplemented if getting the reset reason is not implemented
    */
    virtual ErrorType getResetReason(OperatingSystemConfig::ResetReason &resetReason) = 0;
    /**
     * @brief Perform a soft reset of the processor.
     * @returns ErrorType::Success if the processor was reset
     * @returns ErrorType::Failure if the processor could not be reset
     * @returns ErrorType::NotImplemented if resetting the processor is not implemented
     * @returns ErrorType::NotAvailable if resetting the processor is not available
    */
    virtual ErrorType reset() = 0;
    /**
     * @brief setTimeOfDay
     * @param[in] utc The time of day in unix time.
     * @param[in] timeZoneDifferenceUtc The time zone difference.
     * @returns ErrorType::Success if the time of day was set.
     * @returns ErrorType::NotImplemented if setting the time of day is not implemented.
     * @returns ErrorType::NotAvailable if setting the time of day is not available on the host system.
     * @returns ErrorType::Failure if an error occurred while setting the time of day.
    */
    virtual ErrorType setTimeOfDay(UnixTime utc, Seconds timeZoneDifferenceUtc) = 0;
};

#endif //__OPERATING_SYSTEM_ABSTRACTION_HPP__