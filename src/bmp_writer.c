#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp_writer.h"

// the implementation here primarily used the specifications defined by this website:
// https://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm

U0 jm_bmp_write(const JM_BMP_Write_ArgDesc *args)
{
    ASSERT_NOT_NULL(args);
    const I32 bytesPerRow = args->width * 3;
    const U8 pad[3] = { 0, 0, 0 };
    const I32 padding = (4 - (bytesPerRow) % 4) % 4;
    FILE *out = fopen(args->fileName, "wb");
    if(out == NULL)
    {
        printerr("Failed to open the output BMP file");
        return;
    }
    const I32 fileSize = 54 + (bytesPerRow + padding * args->height);
    const U8 header[14] = {
        'B',                      // <-- Signature (2)
        'M',                      // |
        (U8) fileSize,         // <-- File Size (4)
        (U8) (fileSize >> 8),  // |
        (U8) (fileSize >> 16), // |
        (U8) (fileSize >> 24), // |
        0,                        // <-- Reserved (4)
        0,                        // |
        0,                        // |
        0,                        // |
        54,                       // <-- Data Offset (4)
        0,                        // |
        0,                        // |
        0                         // |
    };
    U8 infoHeader[40] = { 0 };
    infoHeader[0] = 40;
    infoHeader[4] = (U8) args->width;
    infoHeader[5] = (U8) (args->width >> 8);
    infoHeader[6] = (U8) (args->width >> 16);
    infoHeader[7] = (U8) (args->width >> 24);
    infoHeader[8] = (U8) args->height;
    infoHeader[9] = (U8) (args->height >> 8);
    infoHeader[10] = (U8) (args->height >> 16);
    infoHeader[11] = (U8) (args->height >> 24);
    infoHeader[12] = 1;
    infoHeader[14] = args->bitsPerPixel;
    fwrite(header, 1, 14, out);
    fwrite(infoHeader, 1, 40, out);
    for(I32 i = 0;i < args->height; i++)
    {
        fwrite(args->data + (i * bytesPerRow), 3, (Sz) args->width, out);
        fwrite(pad, 1, (Sz) padding, out);
    }
    fclose(out);
}

U8 *jm_raster_buffer_to_bmp_data(const JM_RasterBuffer *raster)
{
    ensure(raster);
    ensure(raster->data);
    assert(raster->width > 0 && "jm_raster_buffer_to_bmp_data: Raster 'width' must be positive!");
    assert(raster->height > 0 && "jm_raster_buffer_to_bmp_data: Raster 'height' must be positive!");
    const Sz area = (Sz) (raster->width * raster->height);
    const Sz dataSize = area * 3;
    U8 *res = (U8*) malloc(dataSize);
    if(res == NULL)
    {
        printerr("%s", "Failed to allocate memory.");
        return NULL;
    }
    for(I32 i = 0;i < raster->height;i++)
    {
        for(I32 j = 0;j < raster->width;j++)
        {
            const JM_Color *fragment = &raster->data[i * raster->width + j];
            const Sz index = (Sz) ((i * raster->width + j) * 3);
            res[index] = fragment->b;
            res[index + 1] = fragment->g;
            res[index + 2] = fragment->r;
        }
    }
    return res;
}
