package com.lj.bitmapcompare

import android.graphics.Bitmap
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import com.lj.bitmapcompare.databinding.ActivityMainBinding

class BitmapUtils {
    private val TAG = "BitmapUtils"
    external fun nativeCompareBitmapSimple(
        bitmapPre: Bitmap?,
        bitmapBack: Bitmap?,
        cropDistance: Int
    ): Int
}