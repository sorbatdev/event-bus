#include <cstdint>
#include <concepts>

#include "util.hpp"

namespace eventbus
{
    template<typename T>
    int64_t g_uniqueEventId = -1;

    class BaseEvent
    {
    public:
        virtual ~BaseEvent() = default;
    protected:
        virtual int64_t id() = 0;
    };

    template<typename T>
    class Event : public BaseEvent
    {
    private:
        int64_t id() override
        {
            return g_uniqueEventId<T>;
        }
    };

    template<typename Event>
    int64_t assignId()
    {
        return g_uniqueEventId<Event> = TypeHash<Event>();
    }
}