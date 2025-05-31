#include "utils.h"

#include <stdio.h>
#include <assert.h>

float jm_clampf(const float value, const float min, const float max)
{
    assert(min <= max && "jm_clampf: min value cannot be greater than max value!");
    if (min > max)
    {
        fprintf(stderr, "[W] jm_clampf: Invalid bounds with min greater than max");
        return value;
    }
    return value < min ? min : value > max ? max : value;
}