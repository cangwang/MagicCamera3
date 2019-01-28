#include <stdio.h>
#include <math.h>

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

bool orthoM(float* m, int mOffset,
                          float left, float right, float bottom, float top,
                          float near, float far) {
    if (left == right) {
        return false;
    }
    if (bottom == top) {
        return false;
    }
    if (near == far) {
        return false;
    }

    float r_width  = 1.0f / (right - left);
    float r_height = 1.0f / (top - bottom);
    float r_depth  = 1.0f / (far - near);
    float x =  2.0f * (r_width);
    float y =  2.0f * (r_height);
    float z = -2.0f * (r_depth);
    float tx = -(right + left) * r_width;
    float ty = -(top + bottom) * r_height;
    float tz = -(far + near) * r_depth;
    m[mOffset + 0] = x;
    m[mOffset + 5] = y;
    m[mOffset +10] = z;
    m[mOffset +12] = tx;
    m[mOffset +13] = ty;
    m[mOffset +14] = tz;
    m[mOffset +15] = 1.0f;
    m[mOffset + 1] = 0.0f;
    m[mOffset + 2] = 0.0f;
    m[mOffset + 3] = 0.0f;
    m[mOffset + 4] = 0.0f;
    m[mOffset + 6] = 0.0f;
    m[mOffset + 7] = 0.0f;
    m[mOffset + 8] = 0.0f;
    m[mOffset + 9] = 0.0f;
    m[mOffset + 11] = 0.0f;
    return true;
}

//void setLookAtM(float* rm, int rmOffset,
//                              float eyeX, float eyeY, float eyeZ,
//                              float centerX, float centerY, float centerZ, float upX, float upY,
//                              float upZ) {
//
//    // See the OpenGL GLUT documentation for gluLookAt for a description
//    // of the algorithm. We implement it in a straightforward way:
//
//    float fx = centerX - eyeX;
//    float fy = centerY - eyeY;
//    float fz = centerZ - eyeZ;
//
//    // Normalize f
//    float rlf = 1.0f / Matrix.length(fx, fy, fz);
//    fx *= rlf;
//    fy *= rlf;
//    fz *= rlf;
//
//    // compute s = f x up (x means "cross product")
//    float sx = fy * upZ - fz * upY;
//    float sy = fz * upX - fx * upZ;
//    float sz = fx * upY - fy * upX;
//
//    // and normalize s
//    float rls = 1.0f / Matrix.length(sx, sy, sz);
//    sx *= rls;
//    sy *= rls;
//    sz *= rls;
//
//    // compute u = s x f
//    float ux = sy * fz - sz * fy;
//    float uy = sz * fx - sx * fz;
//    float uz = sx * fy - sy * fx;
//
//    rm[rmOffset + 0] = sx;
//    rm[rmOffset + 1] = ux;
//    rm[rmOffset + 2] = -fx;
//    rm[rmOffset + 3] = 0.0f;
//
//    rm[rmOffset + 4] = sy;
//    rm[rmOffset + 5] = uy;
//    rm[rmOffset + 6] = -fy;
//    rm[rmOffset + 7] = 0.0f;
//
//    rm[rmOffset + 8] = sz;
//    rm[rmOffset + 9] = uz;
//    rm[rmOffset + 10] = -fz;
//    rm[rmOffset + 11] = 0.0f;
//
//    rm[rmOffset + 12] = 0.0f;
//    rm[rmOffset + 13] = 0.0f;
//    rm[rmOffset + 14] = 0.0f;
//    rm[rmOffset + 15] = 1.0f;
//
//    translateM(rm, rmOffset, -eyeX, -eyeY, -eyeZ);
//}
//
//void translateM(float* tm, int tmOffset,
//                              float* m, int mOffset,
//                              float x, float y, float z) {
//    for (int i=0 ; i<12 ; i++) {
//        tm[tmOffset + i] = m[mOffset + i];
//    }
//    for (int i=0 ; i<4 ; i++) {
//        int tmi = tmOffset + i;
//        int mi = mOffset + i;
//        tm[12 + tmi] = m[mi] * x + m[4 + mi] * y + m[8 + mi] * z +
//                       m[12 + mi];
//    }
//}
//
//float length(float x, float y, float z) {
//    return (float) sqrt(x * x + y * y + z * z);
//}
//
//void util_multiplyMM(float* result_ref, int resultOffset,
//                     float* lhs_ref, int lhsOffset,
//                     float* rhs_ref, int rhsOffset) {
//
//    FloatArrayHelper resultMat(env, result_ref, resultOffset, 16);
//    FloatArrayHelper lhs(env, lhs_ref, lhsOffset, 16);
//    FloatArrayHelper rhs(env, rhs_ref, rhsOffset, 16);
//
//    bool checkOK = resultMat.check() && lhs.check() && rhs.check();
//
//    if ( !checkOK ) {
//        return;
//    }
//
//    resultMat.bind();
//    lhs.bind();
//    rhs.bind();
//
//    multiplyMM(resultMat.mData, lhs.mData, rhs.mData);
//
//    resultMat.commitChanges();
//}
//
//void multiplyMM(float* r, const float* lhs, const float* rhs) {
//    for (int i=0 ; i<4 ; i++) {
//        const float rhs_i0 = rhs[ I(i,0) ];
//        float ri0 = lhs[ I(0,0) ] * rhs_i0;
//        float ri1 = lhs[ I(0,1) ] * rhs_i0;
//        float ri2 = lhs[ I(0,2) ] * rhs_i0;
//        float ri3 = lhs[ I(0,3) ] * rhs_i0;
//        for (int j=1 ; j<4 ; j++) {
//            const float rhs_ij = rhs[ I(i,j) ];
//            ri0 += lhs[ I(j,0) ] * rhs_ij;
//            ri1 += lhs[ I(j,1) ] * rhs_ij;
//            ri2 += lhs[ I(j,2) ] * rhs_ij;
//            ri3 += lhs[ I(j,3) ] * rhs_ij;
//        }
//        r[ I(i,0) ] = ri0;
//        r[ I(i,1) ] = ri1;
//        r[ I(i,2) ] = ri2;
//        r[ I(i,3) ] = ri3;
//    }
//}
