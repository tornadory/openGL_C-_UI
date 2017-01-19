#include "common.h"
#include "native_context.h"


extern "C"
void
Java_com_example_cheng_testndk_MainActivity_onPointerDown(JNIEnv *env, jobject instance,
                                                          jint i_point_id, jfloat i_x, jfloat i_y,
                                                          jlong _native) {

    ((NativeContext *) _native)->onPointerDown(i_point_id,i_x,i_y);

}

extern "C"
void
Java_com_example_cheng_testndk_MainActivity_onPointerUp(JNIEnv *env, jobject instance, jfloat i_x,
                                                        jfloat i_y, jlong _native) {

    ((NativeContext *) _native)->onPointerUp(i_x,i_y);
}

extern "C"
void
Java_com_example_cheng_testndk_MainActivity_onPointerMoved(JNIEnv *env, jobject instance,
                                                           jint i_point_id, jfloat i_x, jfloat i_y,
                                                           jlong _native) {
    ((NativeContext *) _native)->onPointerMoved(i_point_id, i_x, i_y);

}

extern "C"
void
Java_com_example_cheng_testndk_MainActivity_destorySurface(JNIEnv *env, jobject instance,

                                                           jlong _native) {

    ((NativeContext *) _native)->onDestorySurface();
}



extern "C"
void
Java_com_example_cheng_testndk_MainActivity_onPauseActivity(JNIEnv *env, jobject instance,long _native) {

    ((NativeContext *)_native)->onPauseActivity();
}

extern "C"
void
Java_com_example_cheng_testndk_MainActivity_onResumeActivity(JNIEnv *env, jobject instance,long _native) {

    ((NativeContext *)_native)->onResumeAcitity();
}

extern "C"
void
Java_com_example_cheng_testndk_MainActivity_changeSurface(JNIEnv *env, jobject instance,
                                                          jint i_width, jint i_height,
                                                          jobject i_surface, jlong _native) {

    ((NativeContext *)_native)->changeSurface(env,i_surface,i_width,i_height);

}
//
//extern "C"
//void
//Java_com_example_cheng_testndk_MainActivity_changeSurface(JNIEnv *env,
//                                                          jint i_width, jint i_height,
//                                                          jobject i_surface, jlong _native) {
//
//    // TODO
//    ((NativeContext *)_native)->changeSurface(env,i_surface,i_width,i_height);
//}

extern "C"
void
Java_com_example_cheng_testndk_MainActivity_renderThread(JNIEnv *env, jobject instance,
                                                         jlong _native) {
    ((NativeContext *)_native)->renderThread();
}

extern "C"
jlong
Java_com_example_cheng_testndk_MainActivity_createNativeContext(JNIEnv *env, jobject instance) {

    return (jlong)new NativeContext();
}

extern "C"
jstring
Java_com_example_cheng_testndk_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C+++++++";
    return env->NewStringUTF(hello.c_str());
}
