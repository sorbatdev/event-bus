# EventBus

A simple event bus with type-safety provided by C++20' s concepts.

- Currently not thread-safe. WIP for future versions.

Before calling any code related to EventBus, the EventBus itself must be created with:
```cpp
int main()
{
    eventbus::CreateEventbus();
    // No return value, since EventBus can be used statically.
    ...
}
```


To create an event that will be raised:

```cpp
struct print_event : eventbus::Event<print_event>
{
    explicit print_event(std::string t_msg) : m_msg{std::move(t_msg)} { }

    std::string m_msg;
};
```
All events should be created in public headers to use in other compilation units.

- `event_bus.hpp` header provides 4 macros for ease of use. These macros are for making
the process of creating `Delegate`s easier and provide a straight-forward API.

After an initial setup, events can be subscribed to and raised like:
```cpp

void PrintMessage(print_event& t_event)
{
    std::cout << t_event.m_msg << std::endl;
}

int main()
{
    // Instantiate EventBus in the internal namespace
    eventbus::CreateEventbus();

    // Notice the usage of this macro is pretty straight-forward.
    // Event type as the first parameter, the function name as the second.
    SUB_TO_EVENT_FREE_FUN(print_event, PrintMessage);

    // If we were the constructor of a class. This would instead be used as:
    // -> SUB_TO_EVENT_MEM_FUN(print_event, PrintMessage);
    // Where the PrintMessage would be a member function. No additional information
    // about the class is needed.

    // We call all the subscribers of the event whether they are member functions or free functions. 
    eventbus::EventBus::RaiseEvent<print_event>("Hello from eventbus!");
}
```
