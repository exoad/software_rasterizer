#include <stdio.h>
#include <assert.h>

#include "utils.h"

float jm_clampf(const float value, const float min, const float max)
{
    assert(min <= max && "jm_clampf: min value cannot be greater than max value!");
    if (min > max)
    {
        printerr("%s", "Invalid bounds with min greater than max");
        return value;
    }
    return value < min ? min : value > max ? max : value;
}