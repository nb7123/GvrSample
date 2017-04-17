//
// Created by michael on 17-4-17.
//
#include <jni.h>
#include <android/asset_manager_jni.h>

#include "gl/GLHelper.h"
#include "Renderer.h"

static GLHelper *glHelper;
static Renderer *renderer;

extern "C" {

JNIEXPORT void JNICALL
Java_com_example_michael_gvrsamples_renderer_NativeRenderer_nativeOnSurfaceCreated(JNIEnv *env,
                                                                                   jobject instance) {
    renderer->InitializeGL();
    glHelper->Init();
    renderer->InitProgram(glHelper->CreateGLProgram("vss.glsl", "fss.glsl"));
}

JNIEXPORT void JNICALL
Java_com_example_michael_gvrsamples_renderer_NativeRenderer_nativeOnDraw(JNIEnv *env,
                                                                         jobject instance) {
    renderer->DrawFrame();
}

JNIEXPORT void JNICALL
Java_com_example_michael_gvrsamples_renderer_NativeRenderer_nativeShutdown(JNIEnv *env,
                                                                           jobject instance) {

    if (nullptr != renderer) delete renderer;
    if (nullptr != glHelper) delete glHelper;
}


JNIEXPORT void JNICALL
Java_com_example_michael_gvrsamples_renderer_NativeRenderer_nativeOnCreate(JNIEnv *env,
                                                                           jobject instance,
                                                                           jlong gvr_ctx,
                                                                           jobject amg) {
    glHelper = new GLHelper(AAssetManager_fromJava(env, amg));
    renderer = new Renderer(reinterpret_cast<gvr_context_ *> (gvr_ctx));
}

JNIEXPORT void JNICALL
Java_com_example_michael_gvrsamples_renderer_NativeRenderer_nativeOnResume(JNIEnv *env,
                                                                           jobject instance) {

    renderer->OnResume();

}

JNIEXPORT void JNICALL
Java_com_example_michael_gvrsamples_renderer_NativeRenderer_nativeOnPause(JNIEnv *env,
                                                                          jobject instance) {

    renderer->OnPause();

}
}