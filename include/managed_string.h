#ifndef __MANAGED_STRING_H__
#define __MANAGED_STRING_H__

#include <string.h>

#include "def.h"

typedef struct JM_ManagedString
{
    char* data;
    usize length;
} JM_ManagedString;
/// @brief Returns a view of a sequence of characters within a larger block
static inline JM_ManagedString jm_managed_string_span(char* beg, const char* end)
{
    JM_ManagedString res = { 0 };
    res.data = beg;
    res.length = beg ? end - beg : 0;
    return res;
}
/// @brief Check if the two strings are equal
static inline bool jm_managed_string_equals(const JM_ManagedString a, const JM_ManagedString b)
{
    return a.length == b.length && (!a.length || !memcmp(a.data, b.data, a.length));
}
/// @brief Removes the leading whitespace
static inline JM_ManagedString jm_managed_string_trimleft(JM_ManagedString a)
{
    for(; a.length && *a.data <= ' '; a.data++, a.length--)
    {
    }
    return a;
}
/// @brief Removes the trailing whitespace
static inline JM_ManagedString jm_managed_string_trimright(JM_ManagedString a)
{
    for(; a.length && a.data[a.length - 1] <= ' '; a.length--)
    {
    }
    return a;
}
/// @brief Returns a new managed string that represents a substring starting from an index within the given string.
static inline JM_ManagedString jm_managed_string_substr(JM_ManagedString a, usize i)
{
    if(i > a.length)
    {
        i = a.length;
    }
    a.data += i;
    a.length -= i;
    return a;
}

#endif