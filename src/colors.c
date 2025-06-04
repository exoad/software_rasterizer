#include "painting.h"

#include <stdlib.h>

// ** PRE-DEFINED COLORS HERE **

JM_Color jm_color_random()
{
    return (JM_Color) {
        .r = (U8) rand(), // NOLINT(cert-msc30-c, cert-msc50-cpp)
        .g = (U8) rand(), // NOLINT(cert-msc30-c, cert-msc50-cpp)
        .b = (U8) rand(), // NOLINT(cert-msc30-c, cert-msc50-cpp)
        .a = 255
    };
}

const JM_Color JM_COLOR_BLACK             = { .r = 0  , .g = 0  , .b = 0  , .a = 255 };
const JM_Color JM_COLOR_WHITE             = { .r = 255, .g = 255, .b = 255, .a = 255 };
const JM_Color JM_COLOR_RED               = { .r = 255, .g = 0  , .b = 0  , .a = 255 };
const JM_Color JM_COLOR_GREEN             = { .r = 0  , .g = 255, .b = 0  , .a = 255 };
const JM_Color JM_COLOR_BLUE              = { .r = 0  , .g = 0  , .b = 255, .a = 255 };
const JM_Color JM_COLOR_YELLOW            = { .r = 255, .g = 255, .b = 0  , .a = 255 };
const JM_Color JM_COLOR_MAGENTA           = { .r = 255, .g = 0  , .b = 255, .a = 255 };
const JM_Color JM_COLOR_CYAN              = { .r = 0  , .g = 255, .b = 255, .a = 255 };
const JM_Color JM_COLOR_GRAY              = { .r = 128, .g = 128, .b = 128, .a = 255 };
const JM_Color JM_COLOR_LIGHT_GRAY        = { .r = 192, .g = 192, .b = 192, .a = 255 };
const JM_Color JM_COLOR_DARK_GRAY         = { .r = 64 , .g = 64 , .b = 64 , .a = 255 };
const JM_Color JM_COLOR_TRANSPARENT_BLACK = { .r = 0  , .g = 0  , .b = 0  , .a = 0   } ;