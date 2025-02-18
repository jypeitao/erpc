#pragma once

#include <jni.h>
#include <sys/prctl.h>
class JNIThreadAttacher {
public:
    explicit JNIThreadAttacher(JavaVM *vm) : vm_(vm), env_(nullptr) {
        status_ = vm_->GetEnv((void **)&env_, JNI_VERSION_1_6);

        if (status_ != JNI_OK && status_ != JNI_EDETACHED) {
            env_ = nullptr;
            return;
        }

        char thread_name[128] = {0};
        prctl(PR_GET_NAME, (char *)(thread_name));

        if (status_ == JNI_EDETACHED) {
            JavaVMAttachArgs args = {JNI_VERSION_1_6, thread_name, NULL};
            if (vm_->AttachCurrentThread(&env_, (void *)&args) != 0) {
                env_ = nullptr;
                return;
            }
        }
    }

    ~JNIThreadAttacher() {
        if (status_ == JNI_EDETACHED) vm_->DetachCurrentThread();
    }

    JNIEnv *getEnv() { return env_; }

private:
    JavaVM *vm_;
    JNIEnv *env_;
    jint status_;
};