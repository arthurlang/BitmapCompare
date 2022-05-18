# BitmapCompare 
自研BitmapCompare算法计算2张图片滑动前后滑动距离
bitmap1向上滑动之后变成bitmap2 （已在千万DAU的App上线使用，稳定可靠）
compute the scroll distance of the picture by compareing the bitmaps
BitmapCompare
compute the scroll distance of the picture by compareing the bitmap
算法思路： 从Bitmap1底部取200px高度像素带作为样本，从bitmap2.height开始向上移动到bitmap的顶部，找到最匹配的2行像素，通过simaple验证，能比较出95%以上的图片
正在解决正在播放的视频无法计算滑动距离的业界难题，更强大功能持续优化中......
