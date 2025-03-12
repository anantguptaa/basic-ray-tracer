#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include "vector.h"
#include "color.h"
#include "spheres.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SHADOW_FACTOR 0.1f

// Camera structure
typedef struct {
    Vec3 position;
    Vec3 direction;
} Camera;

// Viewport structure
typedef struct {
    Vec3 position;
    float height;
    float width;
    float focal_length;
} Viewport;

// Ray structure
typedef struct {
    Vec3 position_vector;
    Vec3 direction_vector;
    float t_scalar;
} Ray;

// Light source structure
typedef struct {
    Vec3 position;
    float brightness;
} LightSource;

// Global variables
extern Camera camera;
extern Viewport viewport;

// Function prototypes
void initialize_camera(void);
void initialize_viewport(float viewport_height, float focal_length, int image_height, int image_width, float *viewport_width);
Vec3 scale_pixel_to_world(int pixel_x, int pixel_y, int image_width, int image_height);
Ray produce_ray(Vec3 pixel_position);
void printVec3(FILE *file, Vec3 vec);
void rendering_pipeline(Viewport viewport, int image_height, int image_width, World *world, Vec3 background_colour, LightSource light_source, FILE *output_file);
Vec3 calculate_final_colour(Sphere *sphere, LightSource light_source, Vec3 intersectionPoint, Vec3 normal, World *world);
void rendering_pipeline_aliasing(Viewport viewport, int image_height, int image_width, World *world, Vec3 background_colour, LightSource light_source, FILE *output_file);

#endif // RAY_TRACER_H
