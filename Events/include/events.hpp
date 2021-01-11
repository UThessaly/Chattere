#include <functional>
#include <string>
#include <any>
#include <map>

namespace chattere::events {
    class EventEmitter
    {
    public:
        template <typename T>
        void on(std::string event, std::function<void(T)> listener)
        {
            listeners[event].push_back(listener);
        }

        template <typename T>
        void emit(std::string event, T data)
        {
            for (auto &listener : listeners[event])
                listeners(data);
        }

    private:
        std::map<std::string, std::vector<std::function<std::any>>> listeners;
    };

    class Event
    {
    };
}