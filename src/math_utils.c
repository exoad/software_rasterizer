#include <assert.h>
#include <stdio.h>

#include "def.h"
#include "utils.h"

F32 jm_clampf(const F32 value, const F32 min, const F32 max)
{
    assert(min <= max && "jm_clampf: min value cannot be greater than max value!");
    if(min > max)
    {
        printerr("%s", "Invalid bounds with min greater than max");
        return value;
    }
    return value < min ? min : value > max ? max : value;
}
