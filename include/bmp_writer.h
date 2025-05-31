#ifndef __BMP_WRITER_H__
#define __BMP_WRITER_H__

#include "def.h"
#include "renderer.h"

// ** IT IS VERY IMPORTANT TO NOTE THAT BMP IS STRUCTURED IN BLUE GREEN RED ORDER **

/// @brief BMP function data
typedef struct
{
    const char* fileName;
    int width;
    int height;
    uint8* data;
    /// @brief 1 = monochrome, 4bit, 8bit, 16bit, 24bit
    uint8 bitsPerPixel;
} JM_BMP_Write_ArgDesc;

/// @brief  Writes a BMP file given the proper information.
/// @param args creation struct to use with named construction
void jm_bmp_write(const JM_BMP_Write_ArgDesc* args);

/// @brief Converts a raster buffer into a new 24-bit BGR format for usage with bmp writing.
/// @param raster buffer
/// @return pointer to the buffer, NULL if creation failed softly.
uint8* jm_raster_buffert_to_bmp_data(const JM_RasterBuffer* raster);

#endif