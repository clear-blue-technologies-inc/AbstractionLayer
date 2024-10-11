#ifndef __OPERATING_SYSTEM_MODULE_HPP__
#define __OPERATING_SYSTEM_MODULE_HPP__

//AbstractionLayer
#include "OperatingSystemAbstraction.hpp"
//Common
#include "Global.hpp"
//C
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
//C++
#include <cassert>
#include <map>

class OperatingSystem : public Global<OperatingSystem>, public OperatingSystemAbstraction {

    public:
    ErrorType delay(Milliseconds delay) override;
    ErrorType createThread(OperatingSystemConfig::Priority priority, std::string name, void * arguments, Bytes stackSize, void *(*startFunction)(void *), Id &number) override;
    ErrorType deleteThread(std::string name) override;
    ErrorType joinThread(std::string name) override;
    ErrorType threadId(std::string name, Id &thread) override;
    ErrorType isDeleted(std::string &name) override;
    ErrorType createSemaphore(Count max, Count initial, std::string name) override;
    ErrorType deleteSemaphore(std::string name) override;
    ErrorType waitSemaphore(std::string &name, Milliseconds timeout) override;
    ErrorType incrementSemaphore(std::string &name) override;
    ErrorType decrementSemaphore(std::string name) override;
    ErrorType createTimer(Id &timer, Milliseconds period, bool autoReload, std::function<void(void)> callback) override;
    ErrorType startTimer(Id timer, Milliseconds timeout) override;
    ErrorType stopTimer(Id timer, Milliseconds timeout) override;
    ErrorType getSystemTime(UnixTime &currentSystemUnixTime) override;
    ErrorType getSystemTick(Ticks &currentSystemTicks) override;
    ErrorType ticksToMilliseconds(Ticks ticks, Milliseconds &timeInMilliseconds) override;
    ErrorType getSoftwareVersion(std::string &softwareVersion) override;
    ErrorType getResetReason(OperatingSystemConfig::ResetReason &resetReason) override;
    ErrorType reset() override;
    ErrorType setTimeOfDay(UnixTime utc, Seconds timeZoneDifferenceUtc) override;
    ErrorType idlePercentage(Percent &idlePercent) override;

    int toPosixPriority(OperatingSystemConfig::Priority priority) {
        assert(sched_get_priority_max(SCHED_FIFO) / 2 > 4);

        switch (priority) {
            case OperatingSystemConfig::Priority::Highest:
                return sched_get_priority_max(SCHED_FIFO) / 2;
                break;
            case OperatingSystemConfig::Priority::High:
                return (sched_get_priority_max(SCHED_FIFO) / 2) - 1;
                break;
            case OperatingSystemConfig::Priority::Normal:
                return (sched_get_priority_max(SCHED_FIFO) / 2) - 2;
                break;
            case OperatingSystemConfig::Priority::Low:
                return (sched_get_priority_max(SCHED_FIFO) / 2) - 3;
                break;
            case OperatingSystemConfig::Priority::Lowest:
                return (sched_get_priority_min(SCHED_FIFO) / 2) - 4;
                break;
            default:
                assert(false);
        }
    }

    private:

    struct Thread {
        pthread_t posixThreadId;
        std::string name;
        Id fndThreadId;
    };

    std::map<std::string, Thread> threads;
    std::map<std::string, sem_t *> semaphores;
};

#endif // __OPERATING_SYSTEM_HPP__
