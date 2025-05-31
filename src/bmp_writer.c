#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp_writer.h"

// the implementation here primarily used the specifications defined by this website:
// https://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm

void jm_bmp_write(const JM_BMP_Write_ArgDesc* data)
{
    ASSERT_NOT_NULL(data);
    const int bytesPerRow = data->width * 3;
    uint8 pad[3] = { 0, 0, 0 };
    const int padding = (4 - (bytesPerRow) % 4) % 4;
    FILE* out = fopen(data->fileName, "wb");
    if(out == NULL)
    {
        perror("Failed to open the output BMP file");
        return;
    }
    const int fileSize = 54 + (bytesPerRow + padding * data->height);
    uint8 header[14] = {
           'B',                      // <-- Signature (2)
           'M',                      // |
           (uint8) fileSize,         // <-- File Size (4)
           (uint8) (fileSize >> 8),  // |
           (uint8) (fileSize >> 16), // |
           (uint8) (fileSize >> 24), // |
           0,                        // <-- Reserved (4)
           0,                        // |
           0,                        // |
           0,                        // |
           54,                       // <-- Data Offset (4)
           0,                        // |
           0,                        // |
           0                         // |
    };
    uint8 infoHeader[40];
    memset(infoHeader, 0, 40);
    infoHeader[0] = 40;
    infoHeader[4] = (uint8) data->width;
    infoHeader[5] = (uint8) (data->width >> 8);
    infoHeader[6] = (uint8) (data->width >> 16);
    infoHeader[7] = (uint8) (data->width >> 24);
    infoHeader[8] = (uint8) data->height;
    infoHeader[9] = (uint8) (data->height >> 8);
    infoHeader[10] = (uint8) (data->height >> 16);
    infoHeader[11] = (uint8) (data->height >> 24);
    infoHeader[12] = 1;
    infoHeader[14] = data->bitsPerPixel;
    fwrite(header, 1, 14, out);
    fwrite(infoHeader, 1, 40, out);
    for(int i = 0; i < data->height; i++)
    {
        fwrite(data->data + (i * bytesPerRow), 3, (usize) data->width, out);
        fwrite(pad, 1, (usize) padding, out);
    }
    fclose(out);
}

uint8* jm_raster_buffer_to_bmp_data(const JM_RasterBuffer* raster)
{
    ASSERT_NOT_NULL(raster);
    ASSERT_NOT_NULL(raster->data);
    assert(raster->width > 0 && "jm_raster_buffer_to_bmp_data: Raster 'width' must be positive!");
    assert(raster->height > 0 && "jm_raster_buffer_to_bmp_data: Raster 'height' must be positive!");
    if(raster == NULL || raster->data == NULL ||
      raster->width <= 0 || raster->height <= 0)
    {
        fprintf(stderr, "ERROR: jm_raster_buffer_to_bmp_data: Invalid raster buffer provided.\n");
        return NULL;
    }
    usize area = (usize) (raster->width * raster->height);
    usize dataSize = area * 3;
    uint8* res = (uint8*) malloc(dataSize);
    if(res == NULL)
    {
        perror("ERROR: jm_raster_buffer_to_bmp_data: Failed to allocate memory.");
        return NULL;
    }
    for(int i = 0; i < raster->height; i++)
    {
        for(int j = 0; j < raster->width; j++)
        {
            const JM_Color* fragment = &raster->data[i * raster->width + j];
            usize index = (usize) ((i * raster->width + j) * 3);
            res[index] = fragment->b;
            res[index + 1] = fragment->g;
            res[index + 2] = fragment->r;
        }
    }
    return res;
}