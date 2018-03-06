//
//  NSObject+getColor.h
//  BLE-RGB
//
//  Created by Yuki.Ma on 2018/3/5.
//  Copyright © 2018年 Yuki.Ma. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSObject ()
void *bitmapData; //内存空间的指针，该内存空间的大小等于图像使用RGB通道所占用的字节数。

static CGContextRef CreateRGBABitmapContext (CGImageRef inImage)
{
    CGContextRef context = NULL;
    CGColorSpaceRef colorSpace;
    int bitmapByteCount;
    int bitmapBytesPerRow;
    
    size_t pixelsWide = CGImageGetWidth(inImage); //获取横向的像素点的个数
    size_t pixelsHigh = CGImageGetHeight(inImage);
    
    bitmapBytesPerRow   = (pixelsWide * 4); //每一行的像素点占用的字节数，每个像素点的ARGB四个通道各占8个bit(0-255)的空间
    bitmapByteCount = (bitmapBytesPerRow * pixelsHigh); //计算整张图占用的字节数
    
    colorSpace = CGColorSpaceCreateDeviceRGB();//创建依赖于设备的RGB通道
    //分配足够容纳图片字节数的内存空间
    bitmapData = malloc( bitmapByteCount );
    //创建CoreGraphic的图形上下文，该上下文描述了bitmaData指向的内存空间需要绘制的图像的一些绘制参数
    context = CGBitmapContextCreate (bitmapData,
                                     pixelsWide,
                                     pixelsHigh,
                                     8,
                                     bitmapBytesPerRow,
                                     colorSpace,
                                     kCGImageAlphaPremultipliedLast);
    //Core Foundation中通过含有Create、Alloc的方法名字创建的指针，需要使用CFRelease()函数释放
    CGColorSpaceRelease( colorSpace );
    return context;
}

// 返回一个指针，该指针指向一个数组，数组中的每四个元素都是图像上的一个像素点的RGBA的数值(0-255)，用无符号的char是因为它正好的取值范围就是0-255
static unsigned char *RequestImagePixelData(UIImage *inImage)
{
    CGImageRef img = [inImage CGImage];
    CGSize size = [inImage size];
    //使用上面的函数创建上下文
    CGContextRef cgctx = CreateRGBABitmapContext(img);
    CGRect rect = {{0,0},{size.width, size.height}};
    //将目标图像绘制到指定的上下文，实际为上下文内的bitmapData。
    CGContextDrawImage(cgctx, rect, img);
    unsigned char *data = CGBitmapContextGetData (cgctx);
    //释放上面的函数创建的上下文
    CGContextRelease(cgctx);
    return data;
}

//设置背景原图片,即取色所用的图片
- (void)setSourceImage:(NSString *)sourceImage ImageWidth:(int)_width ImageHeight:(int)_height {
    //生成指定大小的背景图
    UIImage *im = [UIImage imageNamed:sourceImage];
    UIImage *newImage;
    UIImageView *view = [[UIImageView alloc] initWithImage:im];
    view.frame = CGRectMake(0, 0, _width, _height);
    UIGraphicsBeginImageContext(CGSizeMake(_width, _height)); //size 为CGSize类型，即你所需要的图片尺寸
    [im drawInRect:CGRectMake(0, 0, _width, _height)]; //newImageRect指定了图片绘制区域
    newImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    width = newImage.size.width;
    height = newImage.size.height;
    //将解析背景图为像素，供取色用
    imgPixel = RequestImagePixelData(newImage);
}

//计算颜色
-(UIColor*)calColor:(CGPoint)aPoint {
    int i = 4 * width * round(aPoint.y+imageView.frame.size.height/2) + 4 * round(aPoint.x+imageView.frame.size.width/2);
    int _r = (unsigned char)imgPixel[i];
    int _g = (unsigned char)imgPixel[i+1];
    int _b = (unsigned char)imgPixel[i+2];
    NSLog(@"(%f,%f)",aPoint.x,aPoint.y);
    NSLog(@"Red : %f   Green: %f   Blue: %f",_r/255.0,_g/255.0,_b/255.0);
    return [UIColor colorWithRed:_r/255.0f green:_g/255.0f blue:_b/255.0f alpha:1.0];
}

- (void)changColor:(UIColor *)color{
    int width_;
    if (![Util isIpad]) {
        width_ = 30;
    } else {
        width_ = 70;
    }
    
    UIGraphicsBeginImageContext(CGSizeMake(width_, width_));
    CGContextRef ctx = UIGraphicsGetCurrentContext();
    CGContextMoveToPoint(ctx, 20, 20);
    CGContextSetFillColorWithColor(ctx, color.CGColor);
    if (![Util isIpad]) {
        CGContextAddArc(ctx, width_/2, width_/2, 14.5, 0, 6.3, 0);
    } else {
        CGContextAddArc(ctx, width_/2+0.5, width_/2, 31.3, 0, 6.3, 0);
    }
    CGContextFillPath(ctx);
    self->pickedColorImageView.image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
}
//该片段来自于http://outofmemory.cn
@end
