#include <jni.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <iostream>
#include "CydiaSubstrate.h"
#include <android/log.h>

#define LOG_TAG "Fate"
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,LOG_TAG,__VA_ARGS__)
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

using namespace std;


bool isHooked = false;


jint
(*initialize)(JNIEnv *a1, void *clazz, jstring a3, jstring a4, jstring a5, jstring a6, jstring a7,
              jstring a8);

jint
$initialize(JNIEnv *a1, void *clazz, jstring a3, jstring a4, jstring a5, jstring a6, jstring a7,
            jstring a8) {


    const char *newPath = "/sdcard/ftmgp.apk";//虚假地址,不用真实存在此文件
    jstring pJstring = a1->NewStringUTF(newPath);
    initialize(a1, clazz, a3, a4, a5, pJstring, a7, a8);
    return 0;
}


jint (*RegiserNatives)(JNIEnv *env, jclass clazz, const JNINativeMethod *methods,
                       jint nMethods);

jint $RegiserNatives(JNIEnv *env, jclass clazz, const JNINativeMethod *methods, jint nMethods) {

    for (int i = 0; i < nMethods; i++) {
        if (strcmp(methods[i].name, "initialize") == 0) {
            void *symbol = (void *) ((intptr_t) methods[i].fnPtr);
            MSHookFunction(symbol, (void *) $initialize, (void **) &initialize);
            break;
        }
    }
    return RegiserNatives(env, clazz, methods, nMethods);
}


JNIEXPORT jint

JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    jint result = JNI_ERR;
    jint version = 0;
    JNIEnv *env = 0;

    do {
        if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_OK) {
            version = JNI_VERSION_1_6;
        } else if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) == JNI_OK) {
            version = JNI_VERSION_1_4;
        } else if (vm->GetEnv((void **) &env, JNI_VERSION_1_2) == JNI_OK) {
            version = JNI_VERSION_1_2;
        } else if (vm->GetEnv((void **) &env, JNI_VERSION_1_1) == JNI_OK) {
            version = JNI_VERSION_1_1;
        } else {

            break;
        }
        result = version;

    } while (0);
    LOGD("version=%d", result);


    if (!isHooked) {
        const JNINativeInterface *pInterface = env->functions;
        MSHookFunction((void *) pInterface->RegisterNatives, (void *) $RegiserNatives,
                       (void **) &RegiserNatives);
        isHooked=true;
    }

    return result;
}
