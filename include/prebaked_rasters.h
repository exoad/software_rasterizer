#ifndef __PREBAKED_RASTERS_H__
#define __PREBAKED_RASTERS_H__

#include "renderer.h"

// ** THIS FILE CONTAINS DEFINITIONS FOR SOME BUILTIN RASTER BUFFERS WITH DATA **

/// @brief Creates a uv texture coordinate where the red channel is mapped to the x coordinate and the green coordinate onto the y coordinate
/// @param width width of the buffer data
/// @param height height of the buffer data
/// @return the raster buffer with the data
JM_RasterBuffer* jm_prebaked_uvmap(I32 width, I32 height);
/// @brief Creates a raster buffer with a singular color (good starting point as opposed to using the raw create raster buffer because there might be extra data from C's memory allocation
/// @param color the color to set to
/// @param width width of the buffer data
/// @param height height of the buffer data
/// @return the raster buffer with the filled color
JM_RasterBuffer* jm_prebaked_filled(JM_Color color, I32 width, I32 height);

#endif