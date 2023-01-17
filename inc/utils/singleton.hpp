#pragma once

template<typename T>
class Singleton {
    public:
    static T* Get() {
        static T instance;
        return &instance;
    }

    protected:
    Singleton() {}
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;
};

