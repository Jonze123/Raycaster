#include <stdio.h>
#include <malloc.h>
#include <stdint.h>

#define internal static
#define persistent static
#define global static
#define BYTES_PER_PIXEL 4

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


#pragma pack(push, 1)
typedef struct bmp_t
{
u16 file_type;
u32 file_size;
u16 reserved1;
u16 reserved2;
u32 bmp_offset;
u32 size;
s32 width;
s32 height;
u16 planes;
u16 bpp;
u32 compression;
u32 size_of_bmp;
s32 horz_res;
s32 vert_hes;
u32 colors_used;
u32 colors_important;
} bmp_t;
#pragma pack(pop)

typedef struct image_t
{
u32 width;
u32 height;
u32 num_bytes;
u32 *pixels;
} image_t;

int main(int argc, char **argv)
{
image_t bmp_image = {0};
bmp_image.width = 1280;
bmp_image.height = 720;
bmp_image.num_bytes = bmp_image.width*bmp_image.height*BYTES_PER_PIXEL;
bmp_image.pixels = (u32 *)malloc(bmp_image.num_bytes);

u32 *pixel = bmp_image.pixels;
for(u32 y = 0; y < bmp_image.height; y++)
{
for(u32 x = 0; x < bmp_image.width; x++)
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
bmp_t header = {0};
header.file_type = 0x4d42;
header.file_size = sizeof(header) + bmp_image.num_bytes;
header.bmp_offset = sizeof(header);
header.size = sizeof(header) - 14;
header.width = bmp_image.width;
header.height = bmp_image.height;
header.planes = 1;
header.bpp = 32;
header.compression = 0;
header.size_of_bmp = bmp_image.num_bytes;
header.horz_res = 0;
header.vert_hes = 0;
header.colors_used = 0;
header.colors_important = 0;

char *bmp_output_file_name = "test.bmp";
FILE *bmp_output_file = fopen(bmp_output_file_name, "wb");

if(bmp_output_file)
{
fwrite(&header, sizeof(header), 1, bmp_output_file);
fwrite(bmp_image.pixels, bmp_image.num_bytes, 1, bmp_output_file);
fclose(bmp_output_file);
}
else
{
fprintf(stderr, "Unable to write output file %s", bmp_output_file_name);
}

return 0;
}