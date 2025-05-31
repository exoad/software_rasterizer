#ifndef __PAINTING_H__
#define __PAINTING_H__

#include "def.h"


/// @brief stores a color in 0-255 format as RGBA
typedef struct
{
    /// @brief red channel
    uint8 r;
    /// @brief green channel
    uint8 g;
    /// @brief blue channel
    uint8 b;
    /// @brief alpha channel
    uint8 a;
} JM_Color;
/// @brief returns a random color with the alpha channel locked at 255
/// @return color
JM_Color jm_color_random();

extern const JM_Color JM_COLOR_BLACK;
extern const JM_Color JM_COLOR_WHITE;
extern const JM_Color JM_COLOR_RED;
extern const JM_Color JM_COLOR_GREEN;
extern const JM_Color JM_COLOR_BLUE;
extern const JM_Color JM_COLOR_YELLOW;
extern const JM_Color JM_COLOR_MAGENTA;
extern const JM_Color JM_COLOR_CYAN;
extern const JM_Color JM_COLOR_GRAY;
extern const JM_Color JM_COLOR_LIGHT_GRAY;
extern const JM_Color JM_COLOR_DARK_GRAY;

#endif