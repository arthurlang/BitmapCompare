# BitmapCompare
compute the scroll distance of the picture by compareing the bitmaps
BitmapCompare
compute the scroll distance of the picture by compareing the bitmaps Java_com_package_BitmapUtils_nativeCompareBitmap (JNIEnv *env, jclass bitmapUtilClass,jobject bitmap1, jobject bitmap2) bitmap1向上滑动之后变成bitmap2 算法思路： 从Bitmap1底部取200px高度像素带作为样本，从bitmap2.height开始向上移动到bitmap的顶部，找到最匹配的2行像素，通过simaple验证，能比较出95%以上的图片

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
    AndroidBitmap_unlockPixels(env, bitmap1);
    AndroidBitmap_unlockPixels(env, bitmap2);
    return distance;
    }

持续优化中......
