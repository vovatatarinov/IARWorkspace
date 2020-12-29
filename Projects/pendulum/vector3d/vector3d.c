#include "math.h"
#include "vector3d.h"

float vector3dGetX(vector3d* ptr) {
    return ptr->x;
}

float vector3dGetY(vector3d* ptr) {
    return ptr->y;
}

float vector3dGetZ(vector3d* ptr) {
    return ptr->z;
}

void vector3dSetX(vector3d* ptr, float x) {
    ptr->x = x;
}

void vector3dSetY(vector3d* ptr, float y) {
    ptr->y = y;
}

void vector3dSetZ(vector3d* ptr, float z) {
    ptr->z = z;
}

float vector3dabs(vector3d* ptr) {
    return sqrt(ptr->x * ptr-> x + ptr->y * ptr->y + ptr->z*ptr->z);
}

void vector3dadd(vector3d* res, vector3d* a, vector3d* b) {
    res->x = a->x + b->x;
    res->y = a->y + b->y;
    res->z = a->z + b->z;
}

void vector3dsub(vector3d* res, vector3d* a, vector3d* b) {
    res->x = a->x - b->x;
    res->y = a->y - b->y;
    res->z = a->z - b->z;
}

float vector3dscalarmul(vector3d* a, vector3d* b) {
    return (a->x * b->x + a->y * b->y + a->z * b->z);
}

void vector3dvectormul(vector3d* res, vector3d* a, vector3d* b) {
    res->x = a->y * b->z - a->z * b->y;
    res->y = a->z * b->x - a->x * b->z;
    res->z = a->x * b->y - a->y * b->x;
}

void vector3dmul(vector3d* res, vector3d* a, float k) {
    res->x = a->x * k;
    res->y = a->y * k;
    res->z = a->z * k;
}

void vector3ddiv(vector3d* res, vector3d* a, float k) {
    res->x = a->x / k;
    res->y = a->y / k;
    res->z = a->z / k;
}

void vector3dInit(vector3d* ptr) {
    ptr->getX = &vector3dGetX;
    ptr->getY = &vector3dGetY;
    ptr->getZ = &vector3dGetZ;
    ptr->setX = &vector3dSetX;
    ptr->setY = &vector3dSetY;
    ptr->setZ = &vector3dSetZ;
    ptr->abs = &vector3dabs;
    ptr->add = &vector3dadd;
    ptr->sub = &vector3dsub;
    ptr->scalarmul = &vector3dscalarmul;
    ptr->vectormul = &vector3dvectormul;
    ptr->mul = &vector3dmul;
    ptr->div = &vector3ddiv;	
}
