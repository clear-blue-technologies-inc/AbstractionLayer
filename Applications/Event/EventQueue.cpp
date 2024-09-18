#include "EventQueue.hpp"
#include "OperatingSystemModule.hpp"

int EventQueue::semaphoreCount = 0;
EventQueue::EventQueue() {
    semaphoreCount++;
    binarySemaphore = std::string("eventQueueBinarySemaphore").append(std::to_string(semaphoreCount));
    ErrorType error = OperatingSystem::Instance().createSemaphore(1, 1, binarySemaphore);
    assert(ErrorType::Success == error);
}

ErrorType EventQueue::addEvent(std::unique_ptr<EventAbstraction> &event) {

    ErrorType error = OperatingSystem::Instance().waitSemaphore(binarySemaphore, SemaphoreTimeout);
    if (ErrorType::Success != error) {
        return ErrorType::Timeout;
    }

    if (events.size() >= _maxEvents) {
        error = OperatingSystem::Instance().incrementSemaphore(binarySemaphore);
        assert(ErrorType::Success == error);
        return ErrorType::LimitReached;
    }

    events.push_back(std::move(event));

    error = OperatingSystem::Instance().incrementSemaphore(binarySemaphore);
    assert(ErrorType::Success == error);

    return ErrorType::Success;
}

ErrorType EventQueue::runNextEvent() {

    ErrorType error = OperatingSystem::Instance().waitSemaphore(binarySemaphore, SemaphoreTimeout);
    if (ErrorType::Success != error) {
        return ErrorType::Timeout;
    }

    if (0 == events.size()) {
        error = OperatingSystem::Instance().incrementSemaphore(binarySemaphore);
        assert(ErrorType::Success == error);
        return ErrorType::NoData;
    }

    auto event = std::move(events.front());
    assert(nullptr != event.get());
    events.erase(events.begin());

    error = OperatingSystem::Instance().incrementSemaphore(binarySemaphore);
    assert(ErrorType::Success == error);

    //This needs to be run last, in case the event needs to add more events to the queue or run an event.
    error = event->run();

    return ErrorType::Success;
}