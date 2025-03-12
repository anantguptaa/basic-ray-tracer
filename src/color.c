#include "vector.h"
#include "color.h"

Vec3 unpackRGB(unsigned int packedRGB) {
	//https://stackoverflow.com/questions/3723846/convert-from-hex-color-to-rgb-struct-in-c
	float red = ((packedRGB >> 16) & 0xFF) / 255.0;
    float green = ((packedRGB >> 8) & 0xFF) / 255.0;
    float blue = (packedRGB & 0xFF) / 255.0;

    Vec3 rgb_vector = {red, green, blue};
    return rgb_vector;
}

int compareColor(const void *a, const void *b)
{
    int a1 = 0, b1 = 0;
    for (int i = 0; i < sizeof(int); i++)
    {
        a1 |= (*((unsigned char*)a + i) & 0x0F) << (i * 8);
        b1 |= (*((unsigned char*)b + i) & 0x0F) << (i * 8);
    }
    
    return (a1 < b1) ? -1 : (b1 < a1) ? 1 : (*((int*)a) < *((int*)b)) ? -1 : (*((int*)a) > *((int*)b)) ? 1 : 0;
}

void writeColour(FILE *ppmFile, Vec3 color) {
    int r = color.x * 255;
    int g = color.y * 255;
    int b = color.z * 255;

    fprintf(ppmFile, "%d %d %d ", r, g, b);
}