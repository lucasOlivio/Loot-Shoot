#pragma once

namespace MyEngine
{
    template <typename T>
    class Singleton 
    {
    public:
        static std::shared_ptr<T> GetInstance()
        {
            static std::shared_ptr<T> instance(new T());
            return instance;
        }

        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;

    protected:
        Singleton() {}
        ~Singleton() {}
    };
}
