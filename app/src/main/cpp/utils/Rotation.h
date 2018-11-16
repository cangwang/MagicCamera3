#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>

enum Rotation {
    NORMAL, ROTATION_90, ROTATION_180, ROTATION_270
};

class RotationUtil{
public:
    static int asInt(Rotation rotation);
    static Rotation fromInt(int roation);
};


