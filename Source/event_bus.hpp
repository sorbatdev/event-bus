#pragma once

#include <cstddef>
#include <unordered_map>
#include <vector>
#include <new>
#include <memory>

#include "event.hpp"
#include "delegate.hpp"

namespace eventbus
{

    template<class T>
    concept EngineEvent = std::is_base_of_v<Event<T>, T>;

    class EventBus
    {
    public:
        template<EngineEvent EventType, typename... Args>
        requires std::constructible_from<EventType, Args...>
        static void RaiseEvent(Args&&... p_Args);

        template<EngineEvent EventType>
        static bool SubscribeToEvent(Delegate<void(EventType&)> t_callback);

        template<EngineEvent EventType>
        static bool UnsubscribeFromEvent(Delegate<void(EventType&)> t_callback);

    private:
        std::unordered_map<int64_t, std::vector<Delegate<void(BaseEvent&)>>> m_callbacks;
    };

    namespace internal_
    {
        std::shared_ptr<EventBus> g_instance;
    }

    void CreateEventbus()
    {
        if (nullptr == internal_::g_instance)
        {
            internal_::g_instance = std::make_shared<EventBus>();
        }
    }

    template<EngineEvent EventType, typename... Args>
    requires std::constructible_from<EventType, Args...>
    inline void EventBus::RaiseEvent(Args&&... p_Args)
    {
        auto& callbacks = internal_::g_instance->m_callbacks;
        if (callbacks.contains(g_uniqueEventId<EventType>))
        {
            EventType e(std::forward<Args>(p_Args)...);
            int64_t id = g_uniqueEventId<EventType>;
            for (auto const& subscriber_function : callbacks[id])
            {
                std::invoke(subscriber_function, e);
            }
        }
    }

    template<EngineEvent EventType>
    inline bool EventBus::SubscribeToEvent(Delegate<void(EventType&)> t_callback)
    {
        auto& callbacks = internal_::g_instance->m_callbacks;

        int64_t id = g_uniqueEventId<EventType>;
        // If the event to subscribe doesn't have an id yet, create it.
        if (id == -1)
        {
            id = assignId<EventType>();
        }
        callbacks.try_emplace(id)
            .first->second.push_back(
                std::move(
                   *std::launder(reinterpret_cast<Delegate<void(BaseEvent&)>*>(&t_callback))
                )
            );

        return true;
    }

    template<EngineEvent EventType>
    inline bool EventBus::UnsubscribeFromEvent(Delegate<void(EventType&)> t_callback)
    {
        int64_t id = g_uniqueEventId<EventType>;
        if (id == -1)
        {
            return false;
        }

        auto& callbacks = internal_::g_instance->m_callbacks;
        auto& event_list = callbacks.at(id);

        // Check for the same delegate in the subscribers, then delete it
        event_list.erase(
            std::remove(
            event_list.begin(),
            event_list.end(),
            *std::launder(reinterpret_cast<Delegate<void(BaseEvent&)>*>(&t_callback))
        ),
            event_list.end()
        );

        return true;
    }
} // namespace eventbus

#define SUB_TO_EVENT_MEM_FUN(EventType, MemFuncName)     bool EB_ANON_NAME(result) = ::eventbus::EventBus::SubscribeToEvent(::eventbus::Delegate<void(EventType&)>::template From<std::remove_cvref_t<decltype(*this)>,&std::remove_cvref_t<decltype(*this)>::MemFuncName>(this)); EB_VOID_CAST(EB_ANON_NAME(result))
#define SUB_TO_EVENT_FREE_FUN(EventType, FuncPtr)        bool EB_ANON_NAME(result) = ::eventbus::EventBus::SubscribeToEvent(::eventbus::Delegate<void(EventType&)>::template From<FuncPtr>()); EB_VOID_CAST(EB_ANON_NAME(result))

#define UNSUB_FROM_EVENT_MEM_FUN(EventType, MemFuncName) bool EB_ANON_NAME(result) = ::eventbus::EventBus::UnsubscribeFromEvent(::eventbus::Delegate<void(EventType&)>::template From<std::remove_cvref_t<decltype(*this)>,&std::remove_cvref_t<decltype(*this)>::MemFuncName>(this)); EB_VOID_CAST(EB_ANON_NAME(result));
#define UNSUB_FROM_EVENT_FREE_FUN(EventType, FuncPtr)    bool EB_ANON_NAME(result) = ::eventbus::EventBus::UnsubscribeFromEvent(::eventbus::Delegate<void(EventType&)>::template From<FuncPtr>()); EB_VOID_CAST(EB_ANON_NAME(result))
