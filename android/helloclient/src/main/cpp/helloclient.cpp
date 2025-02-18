
#include <jni.h>
#include <hello_world_client.h>

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_dreamsmart_helloclient_MainActivity_callPrintText(JNIEnv *env, jobject thiz,
                                                           jstring text) {
    auto ret = false;

    const char *nativeString = env->GetStringUTFChars(text, nullptr);
    if (nativeString == nullptr) {
        return ret;
    }

    ret = hello_world_call_printText(nativeString);

    env->ReleaseStringUTFChars(text, nativeString);

    return ret;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_dreamsmart_helloclient_MainActivity_init(JNIEnv *env, jobject thiz, jboolean tcp,
                                                  jstring address, jint port) {
    auto ret = false;
    const char *nativeAddress = env->GetStringUTFChars(address, nullptr);
    if (nativeAddress == nullptr) {
        return ret;
    }

    ret = hello_world_init_client(tcp, nativeAddress, port);
    env->ReleaseStringUTFChars(address, nativeAddress);

    return ret;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_dreamsmart_helloclient_MainActivity_destroy(JNIEnv *env, jobject thiz) {
    hello_world_destroy_client();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_dreamsmart_helloclient_MainActivity_callStopServer(JNIEnv *env, jobject thiz) {
    hello_world_call_stopServer();
}