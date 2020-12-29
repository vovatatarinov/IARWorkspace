typedef struct vector3d {
    float x,y,z;
    float (*getX) (struct vector3d*);
    float (*getY) (struct vector3d*);
    float (*getZ) (struct vector3d*);
    void (*setX) (struct vector3d*, float);
    void (*setY) (struct vector3d*, float);
    void (*setZ) (struct vector3d*, float);
    float (*abs) (struct vector3d*);
    void (*add) (struct vector3d*, struct vector3d*, struct vector3d*);
    void (*sub) (struct vector3d*, struct vector3d*, struct vector3d*);
    float (*scalarmul) (struct vector3d*, struct vector3d*);
    void (*vectormul) (struct vector3d*, struct vector3d*, struct vector3d*);
    void (*mul) (struct vector3d*, struct vector3d*, float);
    void (*div) (struct vector3d*, struct vector3d*, float);

} vector3d;

void vector3dInit(vector3d* ptr);