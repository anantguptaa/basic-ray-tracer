#include "assg.h"

Camera camera;    // Global definition
Viewport viewport; // Global definition

//https://stackoverflow.com/questions/5157337/reading-command-line-parameters
int main(int argc, char const *argv[]) {
    if (argc != 3) {
        printf("Incorrect number of arguments.\n");
        return 1;
    }

    const char *input_path = argv[1];
    const char *output_path = argv[2];

    //https://users.cs.utah.edu/~germain/PPS/Topics/C_Language/file_IO.html
    FILE *input_file = fopen(input_path, "r");
    if (input_file == NULL) {
        printf("Error! Could not open file\n");
        return 1;
    }

    int image_width, image_height, num_colors, background_index, num_spheres;
    float viewport_height, focal_length, viewport_width;
    LightSource light_source;
    World world;
    worldInit(&world);

    if (fscanf(input_file, "%d %d", &image_width, &image_height) != 2) {
        fprintf(stderr, "Error: Failed to read input\n");
        return 1;
    }
    if (fscanf(input_file, "%f", &viewport_height) != 1) {
        fprintf(stderr, "Error: Failed to read input\n");
        return 1;
    }
    if (fscanf(input_file, "%f", &focal_length) != 1) {
        fprintf(stderr, "Error: Failed to read input\n");
        return 1;
    }
    if (fscanf(input_file, "%f %f %f %f", &light_source.position.x, &light_source.position.y, &light_source.position.z, &light_source.brightness) != 4) {
        fprintf(stderr, "Error: Failed to read input\n");
        return 1;
    }
    if (fscanf(input_file, "%d", &num_colors) != 1) {
        fprintf(stderr, "Error: Failed to read input\n");
        return 1;
    }

    // Creating an array for colors in HEX
    // https://stackoverflow.com/questions/1396732/how-do-i-read-hex-numbers-into-an-unsigned-int-in-c
    unsigned int colors[num_colors];
    for (int i = 0; i < num_colors; i++) {
        if (fscanf(input_file, "%x", &colors[i]) != 1) {
            fprintf(stderr, "Error: Failed to read input\n");
            return 1;
        }
    }

    #ifdef FS
    // https://en.cppreference.com/w/c/algorithm/qsort
    qsort(colors, num_colors, sizeof(unsigned int), compareColor);
    #endif

    if (fscanf(input_file, "%d", &background_index) != 1) {
        fprintf(stderr, "Error: Failed to read input\n");
        return 1;
    }
     if (fscanf(input_file, "%d", &num_spheres) != 1) {
        fprintf(stderr, "Error: Failed to read input\n");
        return 1;
     }

    for (int i = 0; i < num_spheres; i++) {
        Vec3 position;
        float radius;
        int color_index;
        if (fscanf(input_file, "%f %f %f %f %d", &position.x, &position.y, &position.z, &radius, &color_index) != 5) {
            fprintf(stderr, "Error: Failed to read input\n");
            return 1;
        }

        Vec3 sphere_color = (Vec3){1, 1, 1};
        #ifdef FS
        sphere_color = unpackRGB(colors[color_index]);
        #endif


        Sphere *sphere = createSphere(radius, position, sphere_color);
        addSphere(&world, sphere);
    }

    fclose(input_file);

    initialize_camera();
    initialize_viewport(viewport_height, focal_length, image_height, image_width, &viewport_width);

    #ifdef FS
    Vec3 background_colour = unpackRGB(colors[background_index]);
    #endif

    FILE *output_file = fopen(output_path, "w");
    if (!output_file) {
        perror("Failed to open output file");
        fclose(input_file);
        return 1;
    }
    
    #ifdef FS
    fprintf(output_file,"P3\n%d %d\n255\n", image_width, image_height);
    rendering_pipeline_aliasing(viewport, image_height, image_width, &world, background_colour, light_source, output_file);
    #endif
    
    #ifdef MS2
    // Background color for milestone 1 and 2 is black (0, 0, 0)
    Vec3 background_colour = (Vec3){0, 0, 0};
    // PPM header
    fprintf(output_file,"P3\n%d %d\n255\n", image_width, image_height);
    rendering_pipeline(viewport, image_height, image_width, &world, background_colour, light_source, output_file);
    
    #endif
    
    // Output calculations for MS1
    #ifdef MS1
    // Background color for milestone 1 and 2 is black (0, 0, 0)
    Vec3 background_colour = (Vec3){0, 0, 0};

    printVec3(output_file, background_colour);
    fprintf(output_file, " + ");
    printVec3(output_file, light_source.position);
    fprintf(output_file, " = ");
    printVec3(output_file, add(background_colour, light_source.position));
    fprintf(output_file, "\n");

    printVec3(output_file, background_colour);
    fprintf(output_file, " - ");
    printVec3(output_file, light_source.position);
    fprintf(output_file, " = ");
    printVec3(output_file, subtract(background_colour, light_source.position));
    fprintf(output_file, "\n");

    fprintf(output_file, "%.1f * ", viewport.width);
    printVec3(output_file, light_source.position);
    fprintf(output_file, " = ");
    printVec3(output_file, scalarMultiply(viewport.width, light_source.position));
    fprintf(output_file, "\n");

    fprintf(output_file, "normalize");
    printVec3(output_file, light_source.position);
    fprintf(output_file, " = ");
    printVec3(output_file, normalize(light_source.position));
    fprintf(output_file, "\n");

    for (int i = 0; i < world.size; i++) {
        Sphere *sphere = world.spheres[i];

        // New line before each sphere
        fprintf(output_file, "\n");

        // Sphere color / sphere radius
        printVec3(output_file, sphere->color);
        fprintf(output_file, " / ");
        fprintf(output_file, "%.1f = ", sphere->r);
        printVec3(output_file, scalarDivide(sphere->color, sphere->r));
        fprintf(output_file, "\n");

        // Dot product
        fprintf(output_file, "dot(");
        printVec3(output_file, light_source.position);
        fprintf(output_file, ", ");
        printVec3(output_file, sphere->pos);
        fprintf(output_file, ")");
        fprintf(output_file, " = %.1f", dot(light_source.position, sphere->pos));
        fprintf(output_file, "\n");

        // Distance
        fprintf(output_file, "distance(");
        printVec3(output_file, light_source.position);
        fprintf(output_file, ", ");
        printVec3(output_file, sphere->pos);
        fprintf(output_file, ")");
        fprintf(output_file, " = %.1f", distance(light_source.position, sphere->pos));
        fprintf(output_file, "\n");

        // Length
        fprintf(output_file, "length");
        printVec3(output_file, sphere->pos);
        fprintf(output_file, " = %.1f", length(sphere->pos));
        fprintf(output_file, "\n");
    }
    #endif

    fclose(output_file);
    freeWorld(&world);

    return 0;
}


void initialize_camera() {
    camera.position = (Vec3){0, 0, 0};
    camera.direction = (Vec3){0, 0, -1};  // assuming -z direction
}

void initialize_viewport(float viewport_height, float focal_length, int image_height, int image_width, float *viewport_width) {
    viewport.height = viewport_height;
    viewport.width = viewport_height * ((float) image_width / image_height);
    *viewport_width = viewport.width;
    viewport.focal_length = focal_length;
    viewport.position = (Vec3){0, 0, -focal_length};
}

Vec3 scale_pixel_to_world(int pixel_x, int pixel_y, int image_width, int image_height) {
    float world_x = ((pixel_x + 0.5) - image_width / 2.0) * (viewport.width / image_width);
    float world_y = ((pixel_y + 0.5) - image_height / 2.0) * (viewport.height / image_height);

    Vec3 world_vector = {world_x, world_y, -viewport.focal_length};
    return world_vector;
}

Ray produce_ray(Vec3 pixel_position) {
    Ray ray;
    ray.position_vector = (Vec3){0,0,0};
    ray.direction_vector = normalize(subtract(pixel_position, ray.position_vector));
    return ray;
}

void printVec3(FILE *file ,Vec3 vec) {
    fprintf(file, "(%.1f, %.1f, %.1f)", vec.x, vec.y, vec.z);
}

Ray produce_shadow_ray(Vec3 intersectionPoint, Vec3 normal, LightSource light_source){
    Vec3 adjusted_position_vector =  add(intersectionPoint, scalarMultiply(0.001f,normal));     //p = i + 0.001 n
    Vec3 light_direction = normalize(subtract(light_source.position, intersectionPoint));       // finding the light direction becuase the shadow ray has to be pointed in that direction.

    Ray shadow_ray;
    shadow_ray.position_vector = adjusted_position_vector;
    shadow_ray.direction_vector = light_direction;

    return shadow_ray;
}

Vec3 calculate_final_colour(Sphere *sphere, LightSource light_source, Vec3 intersectionPoint, Vec3 normal, World *world) {
    // Calculate normalized light direction
    Vec3 d = normalize(subtract(light_source.position, intersectionPoint));
    float dot_product = fmax(dot(d, normal), 0.0);

    // Calculate distance squared
    // float distance_squared = dot(subtract(light_source.position, intersectionPoint), subtract(light_source.position, intersectionPoint));
    float distance_squared = distance2(light_source.position, intersectionPoint);

    // Calculate intensity
    float intensity = fmin(light_source.brightness * (dot_product / distance_squared), 1.0);

    Vec3 final_colour = scalarMultiply(intensity, sphere->color);

    // Check for shadows
    Ray shadow_ray = produce_shadow_ray(intersectionPoint, normal, light_source);
    float distance_to_light = distance2(light_source.position, add(intersectionPoint, scalarMultiply(0.001f,normal)));

    for (int i = 0; i < world->size; i++) {
        if (doesIntersect(world->spheres[i], shadow_ray.position_vector, shadow_ray.direction_vector, &shadow_ray.t_scalar)) {
            // Check if the intersection is between the point and the light source
            if (shadow_ray.t_scalar > 0 && distance_to_light > (shadow_ray.t_scalar * shadow_ray.t_scalar)) {
                return scalarMultiply(SHADOW_FACTOR, final_colour);
            }
        }
    }

    return final_colour;
}

void rendering_pipeline(Viewport viewport, int image_height, int image_width, World *world, Vec3 background_colour, LightSource light_source, FILE *output_file) {
    // Writing to the image in order from (in image coordinates) 0,0, to imgW-1,imgH-1 will result in an upside down image.
    // So writing it the opposite way, from top to bottom and left to right
    for(int y = image_height - 1; y >= 0; y--) {
        for (int x = 0; x < image_width; x++) {
            // Scaling each pixel to match their respective world coordinates
            // Vec3 pixel_position = {x, y, viewport.focal_length};
            Vec3 scaled_pixel_position = scale_pixel_to_world(x, y, image_width, image_height);

            // Generating a ray for each pixel
            Ray ray = produce_ray(scaled_pixel_position);

            // the final colour for the pixel is the backgorund colour if there are no intersections
            Vec3 final_pixel_colour = background_colour;
            //https://stackoverflow.com/questions/4786663/limits-for-floating-point-types
            float closest_t = __FLT_MAX__;

            for (int i = 0; i < world->size; i++) {
                if (doesIntersect(world->spheres[i], ray.position_vector, ray.direction_vector, &ray.t_scalar)) {

                    // Updating the colour if this is closer than the previous one
                    if (ray.t_scalar < closest_t) {
                        closest_t = ray.t_scalar;

                        // finding the normal and intersection point for the sphere
                        Vec3 intersectionPoint = add(ray.position_vector, scalarMultiply(ray.t_scalar, ray.direction_vector));
                        Vec3 normal = normalize(subtract(intersectionPoint, world->spheres[i]->pos));
                        //find the final colour for the pixel
                        final_pixel_colour = calculate_final_colour(world->spheres[i], light_source, intersectionPoint, normal, world);
                    }
                }
            }
            writeColour(output_file, final_pixel_colour);
            
        }
    }
}

void rendering_pipeline_aliasing(Viewport viewport, int image_height, int image_width, World *world, Vec3 background_colour, LightSource light_source, FILE *output_file) {
    // Offsets for the 3x3 grid sampling
    float offsets[3] = {-1.0/3, 0.0, 1.0/3};

    // Writing to the image in order from (in image coordinates) 0,0, to imgW-1,imgH-1 will result in an upside down image.
    // So writing it the opposite way, from top to bottom and left to right
    for(int y = image_height - 1; y >= 0; y--) {
        for (int x = 0; x < image_width; x++) {
            Vec3 final_pixel_colour = {0.0f, 0.0f, 0.0f};

            // Sample each pixel 9 times
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    // Scaling each pixel to match their respective world coordinates
                    Vec3 scaled_pixel_position = scale_pixel_to_world(x + offsets[i], y + offsets[j], image_width, image_height);

                    // Generating a ray for each sample
                    Ray ray = produce_ray(scaled_pixel_position);

                    // The final colour for the sample is the background colour if there are no intersections
                    Vec3 sample_colour = background_colour;
                    float closest_t = __FLT_MAX__;

                    for (int k = 0; k < world->size; k++) {
                        if (doesIntersect(world->spheres[k], ray.position_vector, ray.direction_vector, &ray.t_scalar)) {
                            // Updating the colour if this is closer than the previous one
                            if (ray.t_scalar < closest_t) {
                                closest_t = ray.t_scalar;

                                // Finding the normal and intersection point for the sphere
                                Vec3 intersectionPoint = add(ray.position_vector, scalarMultiply(ray.t_scalar, ray.direction_vector));
                                Vec3 normal = normalize(subtract(intersectionPoint, world->spheres[k]->pos));
                                // Find the final colour for the sample
                                sample_colour = calculate_final_colour(world->spheres[k], light_source, intersectionPoint, normal, world);
                            }
                        }
                    }
                    // Accumulate the sample colour
                    final_pixel_colour = add(final_pixel_colour, sample_colour);
                }
            }
            // Average the accumulated colour
            final_pixel_colour = scalarDivide(final_pixel_colour, 9.0f);

            // Write the final averaged colour to the output file
            writeColour(output_file, final_pixel_colour);
        }
    }
}
