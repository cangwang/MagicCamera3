#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>
#include "gpuimagefilter.h"


GPUImageFilter::GPUImageFilter(){

}

GPUImageFilter::GPUImageFilter(char *vertexShader, char *fragmentShader) {
    this->vertexShader = vertexShader;
    this->fragmentShader = fragmentShader;
}

void GPUImageFilter::init() {
    onInit();
    onInitialized();
}

void GPUImageFilter::onInit() {

}