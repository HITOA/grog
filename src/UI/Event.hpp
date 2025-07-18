#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <typeindex>
#include <mutex>


namespace Grog {

    class EventHandle {
    public:
        EventHandle() = delete;
        EventHandle(std::shared_ptr<void> event, std::type_index typeIndex);

        template<typename T>
        std::shared_ptr<T> Get() {
            if (typeIndex != typeid(T))
                return nullptr;
            return std::static_pointer_cast<T, void>(event);
        }

    private:
        std::shared_ptr<void> event;
        std::type_index typeIndex;
    };

    class EventListener {
    public:
        virtual ~EventListener() = default;

        virtual bool OnEvent(EventHandle& handle) = 0;
    };

    class EventBus {
    public:
        /**
         * Process all event and call the listeners.
         * All the event listeners will run on the thread calling this function.
         */
        void Process();

        /**
         * Push a new event into the queue. This function is thread safe.
         */
        bool Push(EventHandle& event);
        template<typename T>
        bool Push(std::shared_ptr<T> event) {
            EventHandle handle{ event, typeid(T) };
            return Push(handle);
        }

        /**
         * Add a listener
         */
        bool AddListener(EventListener* listener);

        /**
         * Remove a listener
         */
        void RemoveListener(EventListener* listener);

    private:
        std::queue<EventHandle> events{};
        std::vector<EventListener*> listeners{};
        std::mutex mutex{};
    };

}