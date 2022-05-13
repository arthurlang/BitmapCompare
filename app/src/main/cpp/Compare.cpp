#include <jni.h>
#include <string>
#include <android/bitmap.h>
#include <android/log.h>
#include <vector>
using namespace std;

#define TAG "BitmapUtils-jni"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型

extern "C"
JNIEXPORT jint JNICALL Java_com_lj_bitmapcompare_BitmapUtils_nativeCompareBitmapSimple
        (JNIEnv *env, jclass bitmapUtilClass,jobject bitmapPre, jobject bitmapBack, jint crop_bottom) {
    AndroidBitmapInfo bitmapInfo;
    if ((AndroidBitmap_getInfo(env, bitmapPre, &bitmapInfo)) < 0) {
        return 0;
    }
    void *bitmapPixels;
    if ((AndroidBitmap_lockPixels(env, bitmapPre, &bitmapPixels)) < 0) {
        return 0;
    }
    AndroidBitmapInfo bitmapInfo2;
    if ((AndroidBitmap_getInfo(env, bitmapBack, &bitmapInfo2)) < 0) {
        AndroidBitmap_unlockPixels(env, bitmapPre);
        return 0;
    }
    void *bitmapPixels2;
    if ((AndroidBitmap_lockPixels(env, bitmapBack, &bitmapPixels2)) < 0) {
        AndroidBitmap_unlockPixels(env, bitmapPre);
        return 0;
    }
    jint width = bitmapInfo.width;
    jint height = bitmapInfo.height;
    jint width2 = bitmapInfo2.width;
    jint height2 = bitmapInfo2.height;
    jint stride = bitmapInfo.stride;
    jint stride2 = bitmapInfo2.stride;
    if (height != height2 || width != width2) {
        LOGE("error : height != height2 || width != width2  height: %d  height2: %d width: %d width2: %d", height, height2, width, width2);
        AndroidBitmap_unlockPixels(env, bitmapPre);
        AndroidBitmap_unlockPixels(env, bitmapBack);
        return 0;
    }
    long minResult = LONG_MAX;
    jint minLine = -1;
    jint sample_size = 20;
    jint sample_height = 200;
    jint startLinePre = height - crop_bottom;
    jint startLineBack;
    jint lineEnd = crop_bottom + sample_height + 1;
    for (int line = height; line > lineEnd; line--) {
        long sumNotSame = 0;
        startLineBack = line - crop_bottom;
        for (int i = 0; i < sample_height; i++) {
            jint linePre = startLinePre - i;
            jint lineBack = startLineBack - i;

            jint *linePrePixels = (jint *) ((jbyte *) bitmapPixels + linePre * stride);
            jint *lineBackPixels = (jint *) ((jbyte *) bitmapPixels2 + lineBack * stride2);

            if (linePre < 0 || lineBack < 0) {
                LOGI("end: linePre %d lineBack %d", linePre, lineBack);
            }
            for (int col = 0; col < stride; col = col + sample_size) {
                jint pixelPre = *(linePrePixels + col);
                jint pixelBack = *(lineBackPixels + col);

                if (pixelBack != pixelPre) {
                    sumNotSame = sumNotSame + sample_size;
                }

                if (sumNotSame > minResult) {
                    break;
                }
            }
            if (sumNotSame > minResult) {
                break;
            }
        }

        if (sumNotSame < minResult) {
            minResult = sumNotSame;
            minLine = startLineBack;
        }
    }

    jint distance = startLinePre - minLine;
    LOGE("end: height=%d minResult=%ld crop_bottom=%d minLine=%d distance=%d", height, minResult, crop_bottom, minLine, distance);
    AndroidBitmap_unlockPixels(env, bitmapPre);
    AndroidBitmap_unlockPixels(env, bitmapBack);
    return distance;
}