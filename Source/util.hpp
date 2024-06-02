#include <cstdint>
#include <string>
#include <typeinfo>

#define EB_VOID_CAST(Variable) static_cast<void>(Variable)

#define EB_CONCAT_IMPL(x, y) x ## y
/// Concatenates the passed parameters together.
#define EB_CONCAT(x, y)  EB_CONCAT_IMPL(x, y)

/// Creates a unique name based on a line
#define EB_ANON_NAME(name) EB_CONCAT(name, __LINE__)

namespace eventbus
{
    inline constexpr int64_t StrHash64(char const* t_str)
    {
        int64_t const p = 31;
        int64_t const m = 1'000'000'009;
        int64_t power_of_p = 1;
        int64_t hash_val = 0;

        // Loop to calculate the hash value
        // by iterating over the elements of string
        char c = 0;
        for (size_t i = 0; t_str[i] != '\0'; ++i)
        {
            c = t_str[i];
            hash_val = (hash_val + static_cast<int64_t>(c) * power_of_p) % m;
            power_of_p = (power_of_p * p) % m;
        }

        return (hash_val % m + m) % m;
    }

    template<typename T>
    int64_t TypeHash()
    {
        std::string const name = typeid(T).name();
        int64_t     const id   = StrHash64(name.c_str());

        return id;
    }
}