#ifndef __BMP_WRITER_H__
#define __BMP_WRITER_H__

#include "def.h"
#include "renderer.h"

// ** IT IS VERY IMPORTANT TO NOTE THAT BMP IS STRUCTURED IN BLUE GREEN RED ORDER **

/// @brief BMP function data
typedef struct
{
    const I8* fileName;
    I32 width;
    I32 height;
    U8* data;
    /// @brief 1 = monochrome, 4bit, 8bit, 16bit, 24bit
    U8 bitsPerPixel;
} JM_BMP_Write_ArgDesc;

/// @brief  Writes a BMP file given the proper information.
/// @param args creation struct to use with named construction
U0 jm_bmp_write(const JM_BMP_Write_ArgDesc* args);

/// @brief Converts a raster buffer into a new 24-bit BGR format for usage with bmp writing.
/// @param raster buffer
/// @return pointer to the buffer, NULL if creation failed softly.
U8* jm_raster_buffer_to_bmp_data(const JM_RasterBuffer* raster);

#endif