#include "Event.hpp"



Grog::EventHandle::EventHandle(std::shared_ptr<void> event, std::type_index typeIndex) : event{ event }, typeIndex{ typeIndex } {

}

void Grog::EventBus::Process() {
    std::scoped_lock guard{ mutex };

    while (!events.empty()) {
        EventHandle& event = events.front();
        for (size_t i = 0; i < listeners.size(); ++i) {
            if (listeners[i]->OnEvent(event))
                break;
        }
        events.pop();
    }
}

bool Grog::EventBus::Push(EventHandle& event) {
    std::scoped_lock guard{ mutex };
    events.emplace(event);
    return true;
}

bool Grog::EventBus::AddListener(EventListener* listener) {
    std::scoped_lock guard{ mutex };
    for (size_t i = 0; i < listeners.size(); ++i)
        if (listeners[i] == listener)
            return false;
    listeners.emplace_back(listener);
    return true;
}

void Grog::EventBus::RemoveListener(EventListener* listener) {
    std::scoped_lock guard{ mutex };
    for (size_t i = 0; i < listeners.size(); ++i) {
        if (listeners[i] == listener) {
            listeners.erase(listeners.begin() + i);
            return;
        }
    }
}