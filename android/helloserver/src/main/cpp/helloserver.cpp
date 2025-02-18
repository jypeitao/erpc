#include <jni.h>
#include <hello_world_server.h>
#include<android/log.h>
#include "JNIThreadAttacher.h"


#define LOG_TAG "JNI"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


static jobject g_obj;
static JavaVM *g_jvm;

extern "C"
JNIEXPORT void JNICALL
Java_com_dreamsmart_helloserver_MainActivity_destroy(JNIEnv *env, jobject thiz) {
    hello_world_destroy_server();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_dreamsmart_helloserver_MainActivity_starServer(JNIEnv *env, jobject thiz) {
    hello_world_start_server();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_dreamsmart_helloserver_MainActivity_init(JNIEnv *env, jobject thiz, jboolean tcp) {
    if(g_obj!= nullptr) {
        env->DeleteGlobalRef(g_obj);
    }
    g_obj = env->NewGlobalRef(thiz);

    hello_world_init_server(tcp, [](const char *text) {
        JNIThreadAttacher attacher(g_jvm);
        auto env = attacher.getEnv();

        jstring jText = env->NewStringUTF(text);
        if (jText == nullptr) {
            LOGE("Failed to create jstring");
            return;
        }
        jclass clazz = env->GetObjectClass(g_obj);
        if (clazz == nullptr) {
            LOGE("Failed to find class");
            env->DeleteLocalRef(jText);
            return;
        }
        jmethodID methodID = env->GetMethodID(clazz, "onPrintTextCall", "(Ljava/lang/String;)V");
        if (methodID == nullptr) {
            env->DeleteLocalRef(jText);
            env->DeleteLocalRef(clazz);
            return;
        }

        env->CallVoidMethod(g_obj, methodID, jText);

        env->DeleteLocalRef(jText);
        env->DeleteLocalRef(clazz);
    });

    return true;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_dreamsmart_helloserver_MainActivity_stopServer(JNIEnv *env, jobject thiz) {
    hello_world_stop_server();
}

extern "C" JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    g_jvm = vm;

    return JNI_VERSION_1_6;
}