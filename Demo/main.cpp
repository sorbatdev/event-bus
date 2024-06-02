#include <string>
#include <iostream>

#include <event_bus.hpp>

struct print_event : eventbus::Event<print_event>
{
    explicit print_event(std::string t_msg) : m_msg{std::move(t_msg)} { }

    std::string m_msg;
};

void PrintMessage(print_event& t_event)
{
    std::cout << t_event.m_msg << std::endl;
}

int main()
{
    eventbus::CreateEventbus();

    SUB_TO_EVENT_FREE_FUN(print_event, PrintMessage);

    eventbus::EventBus::SubscribeToEvent(eventbus::Delegate<void(print_event&)>::From(
        [](print_event& t_event)
        {
            std::cout << t_event.m_msg << " A LAMBDA!" << std::endl;
        }
        )
    );

    int a = 3;
    eventbus::EventBus::SubscribeToEvent(eventbus::Delegate<void(print_event&)>::From(
        [&a](print_event& t_event)
        {
            std::cout << t_event.m_msg << " A LAMBDA! " << a << std::endl;
        }
        )
    );

    eventbus::EventBus::RaiseEvent<print_event>("Hello from eventbus!");

    // Capturing lambdas also work with references.
    a = 4;
    eventbus::EventBus::RaiseEvent<print_event>("Hello from eventbus!");
}