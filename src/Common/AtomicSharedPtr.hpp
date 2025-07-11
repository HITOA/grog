#pragma once

#include <atomic>
#include <memory>


namespace Grog {

    template<typename T>
    struct DefaultDeleter {
        void operator()(T* ptr) {
            delete ptr;
        }
    };

    template<typename T, typename Deleter = DefaultDeleter<T>>
    class AtomicSharedPtr {
    public:
        AtomicSharedPtr() : ptr{ nullptr }, counter{ new std::atomic_uint32_t{} } {
            counter->fetch_add(1);
        }
        AtomicSharedPtr(T* ptr) : ptr{ ptr }, counter{ new std::atomic_uint32_t{} } {
            counter->fetch_add(1);
        }
        AtomicSharedPtr(const AtomicSharedPtr& v) : ptr{ v.ptr }, counter{ v.counter } {
            counter->fetch_add(1);
        }
        AtomicSharedPtr(AtomicSharedPtr&& v) : ptr{ v.ptr }, counter{ v.counter } {
            counter->fetch_add(1);
        }
        ~AtomicSharedPtr() {
            uint32_t c = counter->fetch_sub(1);
            if (c == 0)
                Deleter{}(ptr);
        }

        AtomicSharedPtr& operator=(const AtomicSharedPtr& v) {
            uint32_t c = counter->fetch_sub(1);
            if (c == 0)
                Deleter{}(ptr);
            ptr = v.ptr;
            counter = v.counter;
            counter->fetch_add(1);
        }
        AtomicSharedPtr& operator=(AtomicSharedPtr&& v) {
            uint32_t c = counter->fetch_sub(1);
            if (c == 0)
                Deleter{}(ptr);
            ptr = v.ptr;
            counter = v.counter;
            counter->fetch_add(1);
        }

        T* operator->() {
            return ptr;
        }
        
    private:
        T* ptr;
        std::atomic_uint32_t* counter;
    };

    template<typename T, typename... Args>
    AtomicSharedPtr<T> MakeAtomicSharedPtr(Args&&... args) {
        return AtomicSharedPtr{ new T{ std::forward<Args>(args)... } };
    }

}