package com.lj.bitmapcompare

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.Rect
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.lj.bitmapcompare.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        binding.sampleText.text = stringFromJNI()
        Thread {
            try {
                val bitmap1 = getImageFromAssetsFile("bitmap1.jpg")
                val bitmap2 = getImageFromAssetsFile("bitmap2.jpg")
                val value: Int = BitmapUtils.nativeCompareBitmapSimple(bitmap1, bitmap2)
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }.start()
    }

    open fun getImageFromAssetsFile(fileName: String): Bitmap {
        var image: Bitmap? = null
        val am = resources.assets
        val options = BitmapFactory.Options()
        options.inJustDecodeBounds = true
        //        BitmapFactory.decodeResource(getResources(), R.drawable.r5, options);
        val width = 1080
        val height = 2400
        try {
            val `is` = am.open(fileName)
            val rect = Rect(0, 0, width, height)
            val regionOptions = BitmapFactory.Options()
            image = BitmapFactory.decodeStream(`is`, rect, regionOptions)
            `is`.close()
        } catch (e: java.lang.Exception) {
            e.printStackTrace()
        }
        return image!!
    }

    /**
     * A native method that is implemented by the 'bitmapcompare' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {
        // Used to load the 'bitmapcompare' library on application startup.
        init {
            System.loadLibrary("bitmapcompare")
        }
    }
}