#include <math.h>
#include "vector.h"

Vec3 add(Vec3 v1, Vec3 v2) {
	return (Vec3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};;
}

Vec3 subtract(Vec3 v1, Vec3 v2) {
	return (Vec3){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};;
}

Vec3 scalarMultiply(float s, Vec3 v) {
	return (Vec3){v.x * s, v.y * s, v.z * s};
}

Vec3 scalarDivide(Vec3 v, float d) {
	return (Vec3){v.x / d, v.y / d, v.z / d};
}

float dot(Vec3 v1, Vec3 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float length2(Vec3 v) {
	return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

float length(Vec3 v) {
	return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

float distance2(Vec3 v1, Vec3 v2) {
    return ((v1.x - v2.x) * (v1.x - v2.x)) + ((v1.y - v2.y) * (v1.y - v2.y)) + ((v1.z - v2.z) * (v1.z - v2.z));
}
float distance(Vec3 v1, Vec3 v2) {
    return sqrt((v1.x - v2.x) * (v1.x - v2.x) +(v1.y - v2.y) * (v1.y - v2.y) +(v1.z - v2.z) * (v1.z - v2.z));
}

Vec3 normalize(Vec3 v) {
	return (Vec3){v.x / length(v), v.y / length(v), v.z / length(v)};
}
