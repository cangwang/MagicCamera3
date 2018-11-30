#include "MagicCalmFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"

#define LOG_TAG "MagicCalmFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

/**
 * cool滤镜
 */
MagicCalmFilter::MagicCalmFilter(){

}

MagicCalmFilter::MagicCalmFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"default_vertex.glsl"), readShaderFromAsset(assetManager,"calm.glsl")),
      mToneCurveTexture(0),
      mMaskGery1TextureId(0),
      mMaskGery2TextureId(0){

}

MagicCalmFilter::~MagicCalmFilter() {

}

void MagicCalmFilter::onDestroy() {
    glDeleteTextures(1, &mToneCurveTexture);
    glDeleteTextures(1, &mMaskGery1TextureId);
    glDeleteTextures(1, &mMaskGery2TextureId);
}

void MagicCalmFilter::onDrawArraysPre() {
    if(this->mToneCurveTexture !=0){
        glActiveTexture(GL_TEXTURE3);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,mToneCurveTexture);
        glUniform1i(mToneCurveTextureUniformLocation,3);
//        GLuint error= glGetError();
//        if (error !=GL_NO_ERROR){
//            ALOGE("mToneCurveTextureUniformLocation error");
//        }
    }

    if(this->mMaskGery1TextureId !=0){
        glActiveTexture(GL_TEXTURE4);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,mMaskGery1TextureId);
        glUniform1i(mMaskGery1UniformLocation,4);
//        GLuint error= glGetError();
//        if (error !=GL_NO_ERROR){
//            ALOGE("mMaskGery1UniformLocation error");
//        }
    }

    if(this->mMaskGery2TextureId !=0){
        glActiveTexture(GL_TEXTURE5);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,mMaskGery2TextureId);
        glUniform1i(mMaskGery2UniformLocation,5);
//        GLuint error= glGetError();
//        if (error !=GL_NO_ERROR){
//            ALOGE("mMaskGery2UniformLocation error");
//        }
    }
}

void MagicCalmFilter::onDrawArraysAfter() {

    if (this->mToneCurveTexture != 0){
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,mToneCurveTexture);
        glActiveTexture(GL_TEXTURE0);
    }
    if (this->mMaskGery1TextureId != 0){
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D,mMaskGery1TextureId);
        glActiveTexture(GL_TEXTURE0);
    }
    if (this->mMaskGery2TextureId != 0){
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D,mMaskGery2TextureId);
        glActiveTexture(GL_TEXTURE0);
    }
}

void MagicCalmFilter::onInit() {
    GPUImageFilter::onInit();
    mToneCurveTextureUniformLocation = glGetUniformLocation(mGLProgId,"curve");
    mMaskGery1UniformLocation = glGetUniformLocation(mGLProgId,"grey1Frame");
    mMaskGery2UniformLocation = glGetUniformLocation(mGLProgId,"grey2Frame");
}

void MagicCalmFilter::onInitialized() {
    GPUImageFilter::onInitialized();

    glGenTextures(1, &mToneCurveTexture);
    glBindTexture(GL_TEXTURE_2D,mToneCurveTexture);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    //c++中没有byte类型，用unsigned char代替
    unsigned char* arrayOfByte = (unsigned char*)malloc(sizeof(char)*3072);
    unsigned char arrayOfInt1[] = { 38, 39, 40, 41, 41, 42, 43, 44, 45, 46, 47, 47, 48, 49, 50, 51, 52, 52, 53, 54, 55, 56, 57, 58, 58, 59, 60, 61, 62, 63, 64, 64, 65, 66, 67, 68, 69, 69, 70, 71, 72, 73, 74, 75, 75, 76, 77, 78, 79, 80, 81, 81, 82, 83, 84, 85, 86, 87, 87, 88, 89, 90, 91, 92, 92, 93, 94, 95, 96, 97, 98, 98, 99, 100, 101, 102, 103, 104, 104, 105, 106, 107, 108, 109, 109, 110, 111, 112, 113, 114, 115, 115, 116, 117, 118, 119, 120, 121, 121, 122, 123, 124, 125, 126, 127, 127, 128, 129, 130, 131, 132, 132, 133, 134, 135, 136, 137, 138, 138, 139, 140, 141, 142, 143, 144, 144, 145, 146, 147, 148, 149, 149, 150, 151, 152, 153, 154, 155, 155, 156, 157, 158, 159, 160, 161, 161, 162, 163, 164, 165, 166, 166, 167, 168, 169, 170, 171, 172, 172, 173, 174, 175, 176, 177, 178, 178, 179, 180, 181, 182, 183, 184, 184, 185, 186, 187, 188, 189, 189, 190, 191, 192, 193, 194, 195, 195, 196, 197, 198, 199, 200, 201, 201, 202, 203, 204, 205, 206, 206, 207, 208, 209, 210, 211, 212, 212, 213, 214, 215, 216, 217, 218, 218, 219, 220, 221, 222, 223, 224, 224, 225, 226, 227, 228, 229, 229, 230, 231, 232, 233, 234, 235, 235, 236, 237, 238, 239, 240, 241, 241, 242, 243, 244, 245, 246, 246, 247, 248, 249, 250, 251, 252, 252, 253, 254, 255 };
    for (int i = 0; i < 256; i++){
        arrayOfByte[(i * 4)] = arrayOfInt1[i];
        arrayOfByte[(1 + i * 4)] = arrayOfInt1[i];
        arrayOfByte[(2 + i * 4)] = arrayOfInt1[i];
        arrayOfByte[(3 + i * 4)] = static_cast<unsigned char>(-1);
    }
    unsigned char arrayOfInt2[] = { 0, 1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 24, 25, 26, 27, 29, 30, 31, 32, 33, 34, 35, 36, 37, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 93, 94, 95, 96, 97, 98, 99, 99, 100, 101, 102, 103, 104, 104, 105, 106, 107, 108, 109, 109, 110, 111, 112, 113, 114, 114, 115, 116, 117, 118, 118, 119, 120, 121, 122, 123, 123, 124, 125, 126, 127, 127, 128, 129, 130, 131, 131, 132, 133, 134, 135, 135, 136, 137, 138, 139, 140, 140, 141, 142, 143, 144, 145, 145, 146, 147, 148, 149, 150, 150, 151, 152, 153, 154, 155, 156, 157, 157, 158, 159, 160, 161, 162, 163, 164, 165, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 196, 197, 198, 199, 200, 201, 202, 203, 204, 206, 207, 208, 209, 210, 211, 213, 214, 215, 216, 217, 218, 220, 221, 222, 223, 224, 226, 227, 228, 229, 230, 232, 233, 234, 235, 237, 238, 239, 240, 241, 243, 244, 245, 246, 248, 249, 250, 251, 253, 254, 255 };
    unsigned char arrayOfInt3[] = { 0, 1, 1, 2, 3, 3, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11, 11, 12, 13, 14, 15, 15, 16, 17, 18, 19, 20, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 36, 37, 38, 39, 40, 42, 43, 44, 46, 47, 48, 50, 51, 53, 54, 56, 57, 59, 60, 62, 64, 65, 67, 69, 70, 72, 74, 75, 77, 79, 80, 82, 84, 85, 87, 89, 91, 92, 94, 96, 97, 99, 101, 102, 104, 106, 107, 109, 111, 112, 114, 115, 117, 118, 120, 121, 123, 124, 126, 127, 129, 130, 132, 133, 134, 136, 137, 138, 140, 141, 142, 144, 145, 146, 147, 149, 150, 151, 152, 153, 155, 156, 157, 158, 159, 160, 161, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 184, 185, 186, 187, 188, 189, 190, 191, 191, 192, 193, 194, 195, 196, 196, 197, 198, 199, 200, 200, 201, 202, 203, 203, 204, 205, 206, 206, 207, 208, 209, 209, 210, 211, 211, 212, 213, 214, 214, 215, 216, 216, 217, 218, 218, 219, 220, 220, 221, 222, 222, 223, 224, 224, 225, 226, 226, 227, 227, 228, 229, 229, 230, 231, 231, 232, 233, 233, 234, 234, 235, 236, 236, 237, 237, 238, 239, 239, 240, 240, 241, 242, 242, 243, 243, 244, 245, 245, 246, 246, 247, 247, 248, 249, 249, 250, 250, 251, 252, 252, 253, 253, 254, 254, 255 };
    for (int j = 0; j < 256; j++){
        arrayOfByte[(1024 + j * 4)] = arrayOfInt3[j];
        arrayOfByte[(1 + (1024 + j * 4))] = arrayOfInt3[j];
        arrayOfByte[(2 + (1024 + j * 4))] = arrayOfInt2[j];
        arrayOfByte[(3 + (1024 + j * 4))] = static_cast<unsigned char>(-1);
    }
    unsigned char arrayOfInt4[] = { 0, 1, 3, 4, 5, 7, 8, 10, 11, 12, 14, 15, 16, 18, 19, 20, 22, 23, 24, 26, 27, 28, 30, 31, 33, 34, 35, 37, 38, 39, 41, 42, 43, 45, 46, 47, 49, 50, 51, 53, 54, 55, 57, 58, 59, 61, 62, 63, 64, 66, 67, 68, 70, 71, 72, 74, 75, 76, 77, 79, 80, 81, 83, 84, 85, 86, 88, 89, 90, 91, 93, 94, 95, 96, 98, 99, 100, 101, 103, 104, 105, 106, 108, 109, 110, 111, 112, 114, 115, 116, 117, 118, 119, 121, 122, 123, 124, 125, 126, 128, 129, 130, 131, 132, 133, 134, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 175, 176, 177, 178, 179, 180, 181, 182, 183, 183, 184, 185, 186, 187, 188, 189, 189, 190, 191, 192, 193, 194, 194, 195, 196, 197, 198, 198, 199, 200, 201, 202, 202, 203, 204, 205, 205, 206, 207, 208, 208, 209, 210, 211, 211, 212, 213, 214, 214, 215, 216, 216, 217, 218, 218, 219, 220, 221, 221, 222, 223, 223, 224, 225, 225, 226, 227, 227, 228, 229, 229, 230, 231, 231, 232, 233, 233, 234, 235, 235, 236, 237, 237, 238, 239, 239, 240, 240, 241, 242, 242, 243, 244, 244, 245, 246, 246, 247, 247, 248, 249, 249, 250, 251, 251, 252, 252, 253, 254, 254, 255 };
    unsigned char arrayOfInt5[] = { 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 20, 20, 21, 21, 21, 22, 22, 23, 23, 24, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31, 32, 33, 33, 34, 34, 35, 35, 36, 37, 37, 38, 38, 39, 40, 40, 41, 42, 42, 43, 44, 44, 45, 46, 47, 47, 48, 49, 49, 50, 51, 52, 53, 53, 54, 55, 56, 57, 57, 58, 59, 60, 61, 62, 63, 64, 65, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 81, 82, 83, 84, 85, 86, 87, 88, 90, 91, 92, 93, 94, 96, 97, 98, 99, 101, 102, 103, 105, 106, 107, 109, 110, 111, 113, 114, 115, 117, 118, 120, 121, 123, 124, 126, 127, 129, 130, 132, 133, 135, 137, 138, 140, 142, 143, 145, 147, 148, 150, 152, 153, 155, 157, 159, 161, 162, 164, 166, 168, 170, 172, 174, 175, 177, 179, 181, 183, 185, 187, 189, 191, 193, 195, 197, 199, 201, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221, 224, 226, 228, 230, 232, 234, 236, 238, 240, 242, 244, 247, 249, 251, 253, 255 };
    unsigned char arrayOfInt6[] = { 0, 0, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 8, 8, 8, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 20, 20, 20, 21, 21, 21, 22, 22, 23, 23, 23, 24, 24, 24, 25, 25, 25, 25, 26, 26, 27, 27, 28, 28, 28, 28, 29, 29, 30, 29, 31, 31, 31, 31, 32, 32, 33, 33, 34, 34, 34, 34, 35, 35, 36, 36, 37, 37, 37, 38, 38, 39, 39, 39, 40, 40, 40, 41, 42, 42, 43, 43, 44, 44, 45, 45, 45, 46, 47, 47, 48, 48, 49, 50, 51, 51, 52, 52, 53, 53, 54, 55, 55, 56, 57, 57, 58, 59, 60, 60, 61, 62, 63, 63, 64, 65, 66, 67, 68, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 88, 89, 90, 91, 93, 94, 95, 96, 97, 98, 100, 101, 103, 104, 105, 107, 108, 110, 111, 113, 115, 116, 118, 119, 120, 122, 123, 125, 127, 128, 130, 132, 134, 135, 137, 139, 141, 143, 144, 146, 148, 150, 152, 154, 156, 158, 160, 163, 165, 167, 169, 171, 173, 175, 178, 180, 182, 185, 187, 189, 192, 194, 197, 199, 201, 204, 206, 209, 211, 214, 216, 219, 221, 224, 226, 229, 232, 234, 236, 239, 241, 245, 247, 250, 252, 255 };

    for (int k = 0; k < 256; k++){
        arrayOfByte[(2048 + k * 4)] = arrayOfInt4[k];
        arrayOfByte[(1 + (2048 + k * 4))] = arrayOfInt5[k];
        arrayOfByte[(2 + (2048 + k * 4))] = arrayOfInt6[k];
        arrayOfByte[(3 + (2048 + k * 4))] = static_cast<unsigned char>(-1);
    }
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,256,3,0,GL_RGBA,GL_UNSIGNED_BYTE,arrayOfByte);
    mMaskGery1TextureId=loadTextureFromAssets(mAssetManager,"calm_mask1.jpg");
    mMaskGery2TextureId=loadTextureFromAssets(mAssetManager,"calm_mask2.jpg");
}
