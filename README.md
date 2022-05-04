# BitmapCompare
compute the scroll distance of the picture by compareing the bitmaps
Java_com_package_BitmapUtils_nativeCompareBitmap (JNIEnv *env, jclass bitmapUtilClass,jobject bitmap1, jobject bitmap2) 
bitmap1向上滑动之后变成bitmap2
算法思路：
从Bitmap1底部取200px高度像素带作为样本，从bitmap2.height开始向上移动到bitmap的顶部，找到最匹配的2行像素，通过simaple验证，能比较出95%以上的图片
java_package_bitmapUtils_nativeCompareBitmap (JNIEnv *env, jclass bitmapUtilClass,jobject bitmap1, jobject bitmap2) {
AndroidBitmapInfo bitmapInfo;
        if ((AndroidBitmap_getInfo(env, bitmap1, &bitmapInfo)) < 0) {
        return 0;
        }
        void *bitmapPixels;
        if ((AndroidBitmap_lockPixels(env, bitmap1, &bitmapPixels)) < 0) {
        return 0;
        }
        AndroidBitmapInfo bitmapInfo2;
        if ((AndroidBitmap_getInfo(env, bitmap2, &bitmapInfo2)) < 0) {
        AndroidBitmap_unlockPixels(env, bitmap1);
        return 0;
        }
        void *bitmapPixels2;
        if ((AndroidBitmap_lockPixels(env, bitmap2, &bitmapPixels2)) < 0) {
        AndroidBitmap_unlockPixels(env, bitmap1);
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
        AndroidBitmap_unlockPixels(env, bitmap1);
        AndroidBitmap_unlockPixels(env, bitmap2);
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
        AndroidBitmap_unlockPixels(env, bitmap1);
        AndroidBitmap_unlockPixels(env, bitmap2);
        return distance;
        }
后续优化中......
