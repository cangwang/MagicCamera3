#include <stdio.h>

void scaleM(float* m,int mOffset, float x, float y, float z);
void setIdentityM(float* sm,int smOffset);
bool orthoM(float* m, int mOffset, float left, float right, float bottom, float top, float near, float far);
