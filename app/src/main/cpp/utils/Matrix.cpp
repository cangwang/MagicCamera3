#include <stdio.h>

void scaleM(float* m,int mOffset, float x, float y, float z){
    for (int i = 0; i <4 ; i++) {
        int mi = mOffset +i;
        m[mi]   *=x;
        m[4+mi] *= y;
        m[8+mi] *= z;
    }
}

void setIdentityM(float* sm,int smOffset){
    for(int i =0;i<16;i++){
        sm[smOffset+i]=0;
    }

    for (int i = 0; i < 16; i+=5) {
        sm[smOffset + i] = 1.0f;
    }
}
