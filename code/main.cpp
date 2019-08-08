#include <stdio.h>
#include <malloc.h>
#include <stdint.h>
#include <float.h>
#include <assert.h>

#define internal static
#define persistent static
#define global static
#define BYTES_PER_PIXEL 4
#define R32_MIN -FLT_MAX
#define R32_MAX FLT_MAX
#define ERROR assert(0);

#define ArrayCount(arr) (sizeof(arr)/sizeof(arr[0]))

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

// ray_origin is in Worldspace
internal vec3_t
RayCast(world_t *world, vec3_t ray_origin, vec3_t ray_dir)
{
vec3_t result = world->materials[0].color;
r32 hit_distance = R32_MAX;

r32 tolerance = 0.00001f;
for(u32 i = 0; i < world->num_planes; i++)
{
plane_t plane = world->planes[i];

r32 denom = Dot3(plane.n, ray_dir);
if((denom < -tolerance) || (denom > tolerance))
{
r32 t = (-plane.d - Dot3(plane.n, ray_origin)) / denom;
if((t < hit_distance) && (t > 0))
{
hit_distance = t;
result = world->materials[plane.mat_index].color;
}
}
}

for(u32 i = 0; i < world->num_spheres; i++)
{
sphere_t sphere = world->spheres[i];
        
vec3_t sphere_to_origin = ray_origin - sphere.p;   // Translate the ray origin so that the sphere is at 0,0,0
r32 a = Dot3(ray_dir, ray_dir);
r32 b = 2.0f*Dot3(ray_dir, sphere_to_origin);
r32 c = Dot3(sphere_to_origin, sphere_to_origin) - sphere.r*sphere.r;

r32 denom = 2.0f*a;
r32 root_term = (r32)sqrt(b*b - 4.0f*a*c);
if(root_term > tolerance)
{
r32 tp = (-b + root_term) / denom;
r32 tn = (-b - root_term) / denom;

r32 t = tp;
if((tn > 0) && (tn < tp))
{
t = tn;
}
if((t > 0) && (t < hit_distance))
{
hit_distance = t;
result = world->materials[sphere.mat_index].color;
}
}
}

return result;
}

// Image plane is -1 to 1 inclusive on x and y
internal void
RayTrace(world_t *world, image_t *image, viewpoint_t *eye)
{
    u32 *pixel = image->pixels;
    u32 image_width = image->width;
    u32 image_height = image->height;

r32 eye_dist = 1.0f;
vec3_t eye_look_at = eye->pos - eye_dist*eye->local_z;
vec3_t look_at_min = eye_look_at - (eye->local_x + eye->local_y);

for(u32 y = 0; y < image->height; y++)
    {
r32 image_y = ((2.0f / (r32)(image_height-1))*(y - 0)) - 1;
for(u32 x = 0; x < image->width; x++)
        {
            r32 image_x = ((2.0f / (r32)(image_width-1))*(x - 0)) - 1;
            
vec3_t point_in_plane = look_at_min + (image_x+1.0f)*eye->local_x + (image_y+1.0f)*eye->local_y;

vec3_t ray_origin = eye->pos;   // in Worldspace
vec3_t ray_dir = Vec3Norm(point_in_plane - ray_origin);

vec3_t temp_color = RayCast(world, ray_origin, ray_dir);
vec4_t final_color = {temp_color.r, temp_color.g, temp_color.b, 255.0f};
            *pixel++ = PackRGBA(final_color);
        }
    }
}

internal void
SaveBMP(image_t *image, char *filename)
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
ERROR;
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
if(!result.pixels)
{
fprintf(stderr, "Error allocating memory for image!\n");
ERROR;
}

return result;
}

int 
main(int argc, char **argv)
{
printf("Ray casting...\n");
image_t bmp_image = CreateImage(800, 600, NULL);

material_t materials[4];
materials[0] = {0.1f, 0.2f, 0.6f};
materials[1] = {1.0f, 0.0f, 0.0f};
materials[2] = {1.0f, 1.0f, 1.0f};
materials[3] = {0.3f, 0.3f, 0.3f};

// Worldspace: x goes to right, y goes forward, z goes up
plane_t planes[1] = {};
planes[0].n = {0.0f, 0.0f, 1.0f};
planes[0].d = 0;
planes[0].mat_index = 3;

    sphere_t spheres[3] = {};
spheres[0].p = {0.0f, 0.0f, 70.0f};
spheres[0].r = 8.0f;
spheres[0].mat_index = 1;

    spheres[1].p = {50.0f, 0.0f, 10.0f};
    spheres[1].r = 11.0f;
    spheres[1].mat_index = 2;

    spheres[2].p = {-30.0f, -50.0f, -2.0f};
    spheres[2].r = 11.0f;
    spheres[2].mat_index = 1;
    
world_t world = {};
world.num_materials = ArrayCount(materials);
world.materials = materials;
world.num_planes = ArrayCount(planes);
world.planes = planes;
world.num_spheres = ArrayCount(spheres);
world.spheres = spheres;

vec3_t eye_pos = {0.0f, -100.0f, 10.0f};
vec3_t eye_z = Vec3Norm(eye_pos);
vec3_t eye_x = Vec3Norm(Cross3(vec3_t{0.0f, 0.0f, 1.0f}, eye_z));
vec3_t eye_y = Vec3Norm(Cross3(eye_z, eye_x));

viewpoint_t eye = {};
eye.pos = eye_pos;
eye.local_x = eye_x;
eye.local_y = eye_y;
eye.local_z = eye_z;

RayTrace(&world, &bmp_image, &eye);
SaveBMP(&bmp_image, "test.bmp");

printf("Done!\n");

return 0;
}