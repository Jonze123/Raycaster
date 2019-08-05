#include <stdio.h>
#include <malloc.h>
#include <stdint.h>
#include <float.h>

#define internal static
#define persistent static
#define global static
#define BYTES_PER_PIXEL 4
#define R32_MIN -FLT_MAX
#define R32_MAX FLT_MAX

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef unsigned char byte;

typedef u32 b32;

typedef float r32;
typedef double r64;

#include "ray_math.h"
#include "bmp.h"

internal void
Render(image_t *image)
{
    u32 *pixel = image->pixels;
    for(u32 y = 0; y < image->height; y++)
    {
        for(u32 x = 0; x < image->width; x++)
        {
            if(y < 32)
            {
                *pixel++ = 0xFFFF0000;
            }
            else
            {
                *pixel++ = 0xFF0000FF;
            }
        }
    }
}

internal void
SaveBMP(const image_t *image, char *filename)
{
    bmp_t header = {0};
    header.file_type = 0x4d42;
    header.file_size = sizeof(header) + image->num_bytes;
    header.bmp_offset = sizeof(header);
    header.size = sizeof(header) - 14;
    header.width = image->width;
    header.height = image->height;
    header.planes = 1;
    header.bpp = 32;
    header.compression = 0;
    header.size_of_bmp = image->num_bytes;
    header.horz_res = 0;
    header.vert_hes = 0;
    header.colors_used = 0;
    header.colors_important = 0;
    
    FILE *bmp_output_file = fopen(filename, "wb");
    
    if(bmp_output_file)
    {
        fwrite(&header, sizeof(header), 1, bmp_output_file);
        fwrite(image->pixels, image->num_bytes, 1, bmp_output_file);
        fclose(bmp_output_file);
    }
    else
    {
        fprintf(stderr, "Unable to write output file %s", filename);
    }
}

internal image_t
CreateImage(u32 width, u32 height, void* (*allocator)(u32 num_bytes))
{
image_t result = {0};
result.width = width;
result.height = height;
result.num_bytes = result.width*result.height*BYTES_PER_PIXEL;
if(!allocator)
{
result.pixels = (u32 *)malloc(result.num_bytes);
}
else
{
result.pixels = (u32 *)allocator(result.num_bytes);
}

return result;
}

int 
main(int argc, char **argv)
{
image_t bmp_image = CreateImage(1280, 720, NULL);
Render(&bmp_image);
SaveBMP(&bmp_image, "test.bmp");

return 0;
}