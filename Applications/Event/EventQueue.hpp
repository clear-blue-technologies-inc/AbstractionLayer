/**************************************************************************//**
* @author Ben Haubrich                                        
* @file   EventQueue.hpp
* @details \b Synopsis: \n Synchronization for calling function members of the
           base class.
* @ingroup AbstractionLayer
*******************************************************************************/
#ifndef __EVENT_QUEUE_HPP__
#define __EVENT_QUEUE_HPP__

//AbstractionLayer utilities
#include "Error.hpp"
#include "Types.hpp"
//C++
#include <vector>
#include <tuple>
#include <functional>
#include <memory>
#include <string>

/**
 * @class EventAbstraction
 * @brief A user level interface for running queued events.
*/
class EventAbstraction {
    public:
    EventAbstraction() = default;
    virtual ~EventAbstraction() = default;

    /**
     * @brief Run the next event in the event queue.
     * @returns The error code of the callback that the event runs.
    */
    virtual ErrorType run() = 0;
};

/**
 * @class EventQueue
 * @details Queue is FIFO
 * @brief Provides an interface for synchronizing calls to base classes.
*/
class EventQueue {
    
    public:
    EventQueue();
    ~EventQueue() = default;

    /**
     * @brief Adds an event to the to the queue.
     * @param[in] event The event to add.
     * @post The event is added to a FIFO queue and will be executed when it reaches the first position in the queue and this thread
     *       is running.
     * @post Ownership of the event is transferred to the queue if, and only if, ErrorType::Success is returned.
     * @returns ErrorType::Success
     * @returns ErrorType::LimitReached if the maximum number of events has been reached.
     * @returns ErrorType::Timeout if the semaphore could not be obtained in time
    */
    ErrorType addEvent(std::unique_ptr<EventAbstraction> &event);

    /**
     * @class Event
     * @brief Runs the function and parameters passed to it by the constructor
     * @tparam T The class which contains the function member to call
     * @tparam Args Optional arguments types that the function needs passed to it.
    */
    template <class T, class ...Args> class Event : public EventAbstraction { 

        public:
        /**
         * @brief Constructor.
         * @tparam T The class which contains the function member or lambda expression callback to call
         * @tparam Args The arguments that the callback function needs.
         * @param eventCallback The function member to call.
         * @param params The parameters to pass to the function member.
         * @post The eventCallback is not called.
         * @sa run
         * @code //Bind a function member. Can have any signature, any number of args.
         * ErrorType Class::functionMember(uint8_t arg1, uint32_t arg2, ErrorType &arg3) {
         *   return ErrorType::Success;
         * }
         * 
         * ErrorType Class::bindFunctionMember() {
         *   std::bind(&Class::functionMember, this, arg1, arg2, arg3, ...argn);
         * 
         *   return ErrorType::Success;
         * }
         * @endcode
         * @code //Bind a lambda expression. Can have any signature, any number of args.
         * ErrorType Class::bindFunctionMember(uint8_t arg1, uint32_t arg2, ErrorType &arg3, std::shared_ptr<uint8_t> ptr) {
         *   auto lambdaFunc = [](uint8_t arg1, uint32_t arg2, ErrorType &arg3, std::shared_ptr<uint8_t> ptr) -> ErrorType {};
         *   
         *   //Set it to a function pointer.
         *   std::function<ErrorType(uint8_t arg1, uint32_t arg2, ErrorType &arg3, std::shared_ptr<uint8_t> ptr)> lambdaCallback = lambdaFunc;
         *
         *   //Pass it off as a parameter to Event. Do not need to pass `this` as the first argument as we did in function members.
         *   std::unique_ptr<EventAbstraction> event = std::bind(lambdaFunc, arg1, arg2, arg3, ptr);
         * 
         *   return ErrorType::Success;
         * }
         * @endcode
        */
        Event(std::function<ErrorType(Args ...)> eventCallback, Args ...params) : EventAbstraction(), _eventCallback(eventCallback), _params(std::forward_as_tuple(params...)) {}
        ~Event() = default;

        /**
         * @brief Calls the function member with the parameters that were passed to the constructor.
        */
        ErrorType run() override {
            return _run(_params, std::index_sequence_for<Args...>());

        }

        private:
        /// @brief _params Tuple for forwarding parameter packs.
        std::tuple<Args...> _params;
        /// @brief The callback function of this event.
        std::function<ErrorType(Args...)> _eventCallback;

        /**
         * @brief Calls the callback with the parameters set in the constructor.
         * @returns The error code of the function pointed to by the callback.
        */
        template <std::size_t... IndexSequence> ErrorType _run(std::tuple<Args...> &params, std::index_sequence<IndexSequence...>) const {
            return (_eventCallback)(std::get<IndexSequence>(params)...);
        }
    };
    
    /**
     * @brief The main loop for the eventQueue which can be used to continually check for and run events.
     * @sa runNextEvent
     * @returns ErrorType::NoData if there are no events to process.
     * @returns The error codes of any functions called by runNextEvent.
    */
    virtual ErrorType mainLoop() { return ErrorType::NotImplemented; }

    /**
     * @fn runNextEvent
     * @brief Runs the next event in the queue.
     * @returns ErrorType::NoData if the queue is empty.
     * @returns ErrorType::Timeout if the semaphore could not be obtained in time
     * @returns The error code of the callback function pointed to by the Event.
    */
    ErrorType runNextEvent();

    /// @brief Get the number of events available in the queue.
    /// @return The number of events available in the queue.
    Count eventsAvailable() const { return _maxEvents - events.size(); }

    private:
    /// @brief The maximum number of events that can be queued.
    static constexpr Count _maxEvents = 10;
    static constexpr Milliseconds SemaphoreTimeout = 0;
    static int semaphoreCount;
    /// @brief The queue of events to run.
    std::vector<std::unique_ptr<EventAbstraction>> events;
    std::string binarySemaphore;
};

#endif //__EVENT_QUEUE_HPP__
