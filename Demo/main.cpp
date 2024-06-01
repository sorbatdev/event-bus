#include <string>
#include <iostream>

#include <event_bus.hpp>

struct print_event final : eventbus::Event<print_event>
{
    print_event() = default;
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

    eventbus::EventBus::RaiseEvent<print_event>("Hello from eventbus!");
}