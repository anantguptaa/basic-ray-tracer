#include "spheres.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


//https://stackoverflow.com/questions/3536153/c-dynamically-growing-array
//https://en.cppreference.com/w/c/memory/malloc

void worldInit(World *world) {
    // Initially there arent any spheres and keeping the capacity = 1
    world->capacity = 1;
    world->size = 0;

    // Allocating space for this dynamic array of spheres
    world->spheres = malloc(world->capacity * sizeof(Sphere));
    if (world->spheres == NULL) {
        printf("Memory not allocated for World (dynamic array of spheres)");
        exit(1);
    }
}

// Free memory allocated for the World structure
void freeWorld(World *world) {
    for (int i = 0; i < world->size; i++) {
        free(world->spheres[i]);  // Free each Sphere
    }
    free(world->spheres);         // Free the array itself
    world->spheres = NULL;
    world->size = world->capacity = 0;
}

// Add a Sphere to the World structure, resizing if necessary
void addSphere(World *world, Sphere *sphere) {
    if (world->size >= world->capacity) {
        // Increase capacity by doubling it
        world->capacity *= 2;
        Sphere **newSpheres = realloc(world->spheres, world->capacity * sizeof(Sphere *)); // reallocating the memory to increase space
        if (!newSpheres) {
            printf("Failed to reallocate sphere array");
            freeWorld(world);  // Clean up in case of failure
            exit(1);
        }
        world->spheres = newSpheres;
    }
    world->spheres[world->size] = sphere;  // Add the new sphere
    world->size++;
}

// Create a new Sphere with the given properties
Sphere *createSphere(float radius, Vec3 position, Vec3 color) {
    Sphere *sphere = malloc(sizeof(Sphere));
    if (!sphere) {
        printf("Failed to allocate new sphere");
        exit(1);
    }
    sphere->r = radius;
    sphere->pos = position;
    sphere->color = color;
    
    return sphere;
}

int doesIntersect(const Sphere *sphere, Vec3 rayPos, Vec3 rayDir, float *t){
    Vec3 V = subtract(rayPos, sphere -> pos);

    float a = dot(rayDir, rayDir);
    float b = 2 * dot(rayDir, V);
    float c = dot(V, V) - ((sphere -> r) * (sphere -> r));

    float discriminant = b * b - 4 * a * c; //b^2 - 4ac

    if (discriminant < 0) { //checking discriminant 
        return 0;
    } else {

        float t1, t2;
        float sqrt_d = sqrt(discriminant);
        
        t1 = (-b -sqrt_d)/(2 * a);
        t2 = (-b +sqrt_d)/(2 * a);

        if (t1 > 0 && t2 > 0) { //both intersection in front

            if (t1 > t2){ //find the nearest intersection
                *t = t2;
            } else {
                *t = t1;
            }

        }  else if (t1 > 0) {
            *t = t1;

        } else if (t2 > 0) {
            *t = t2;
            
        } else { //both intersection behind camera
            return 0;
        }

        return 1;
    }
}