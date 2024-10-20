#include "tests.h"

// 练习1，实现库函数strlen
#include <stddef.h> // 为了使用 size_t 类型，这是一种适合表示数组大小和循环计数器的无符号类型（可选）  
  
int my_strlen(char *str) {  
    int length = 0;  
  
    
    if (str == NULL) {  
        return 0;  
    }  
  
   
    while (str[length] != '\0') {  
        length++; 
    }  
  
    return length;  
}


// 练习2，实现库函数strcat
#include <stddef.h> 
void my_strcat(char *str_1, char *str_2) {  
    if (str_1 == NULL || str_2 == NULL) {  
        return; 
    }  
  
    while (*str_1 != '\0') {  
        str_1++;  
    }  
    while (*str_2 != '\0') {  
        *str_1 = *str_2;  
        str_1++;  
        str_2++;  
    }  
  
    *str_1 = '\0';  
}

// 练习3，实现库函数strstr
char* my_strstr(char *s, char *p) {  
    
    if (*p == '\0') {  
        return s;  
    }  
  

    while (*s != '\0') {  
       
        char *s_original = s;  
        char *p_copy = p;  
  
       
        while (*s != '\0' && *p_copy != '\0' && *s == *p_copy) {  
            s++;  
            p_copy++;  
        }  
  
     
        if (*p_copy == '\0') {  
            return s_original;  
        }  
  
     
    }  
  
  
    return 0;  
}

/**
 * ================================= 背景知识 ==================================
 * 1. 什么是图片？
 *    图片可以看做是由像素组成的一个方阵，图片分成两种，一种是灰度图片，颜色只有黑白
 * 两种，一种是彩色图片。
 *    灰度图片的每一个像素是一个范围在[0, 255]内的数值，数值越高就越接近白色。
 *    彩色图片的每一个像素都是一个长度为3的数组，数组中的每一个元素都是[0, 255]内的
 * 数值，3个数的含义分别是Red, Green, Blue，表示这个像素所在的位置这3种颜色的强度，
 * 举个例子，纯红色对应的像素就是{255, 0, 0}。这3个数称为图片的3个通道，这种像素表示
 * 方式也称之为图片的RGB表示方式。
 *
 * 2. 图片的属性？
 *    图片是一个方阵，所以具有宽、高两个属性，除此之外还有一个通道数，比如说灰度图片
 * 的通道数为1，RGB彩色图片的通道数为3。图片的宽高也称为图片的分辨率。
 *
 * 3. 图片在计算机中如何存储？
 *   灰度图片可以看成是二维数组，为方便计算，数据类型使用float。举个例子：
 *   float a[2][3] = {
 *      {121, 231, 35},
 *      {47, 52, 63}
 *   };
 *   就可以看做是一个高为2，宽为3的灰度图片。实际上图片的宽高都比较大，比如说500。
 *
 *   彩色图片也可以看作是由像素构成的二维数组，举个例子，一张高为2，宽为3彩色图片在
 * 内存中的存储为：
 *
 *   低地址 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - >>> 高地址
 *
 *    R    G    B    R    G    B    R    G    B    R    G    B    R    G    B    R    G    B
 *   111, 123, 215, 233, 123, 142, 156, 233, 267, 223, 234, 175, 123, 117, 168, 123, 156, 156
 *   |< -------- >| |< -------- >| |< -------- >| |< -------- >| |< -------- >| |< -------- >|
 *     第一个像素      第二个像素      第三个像素       第四个像素     第五个像素     第六个像素
 *   |< -------------------------------------- >| |< -------------------------------------- >|
 *                    第一行像素                                   第二行像素
 *
 *   理解了图片的存储之后，再开始编写代码。
 */


// 练习4，将彩色图片(rgb)转化为灰度图片
void rgb2gray(float *in, float *out, int h, int w) {
    // 遍历图片的每个像素
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            // 计算当前像素在彩色图片中的地址
            int index = (i * w + j) * 3; // 每个像素有3个分量，所以乘以3
            // 读取当前像素的RGB分量
            float r = in[index];
            float g = in[index + 1];
            float b = in[index + 2];
            // 计算灰度值
            float v = 0.1140 * b + 0.5870 * g + 0.2989 * r;
            // 将计算出的灰度值写入灰度图片对应的位置
            out[i * w + j] = v;
        }
    }
}

// 练习5，实现图像处理算法 resize：缩小或放大图像
void resize(float *in, float *out, int h, int w, int c, float scale) {
    int new_h = (int)(h * scale);
    int new_w = (int)(w * scale);

    // 遍历目标图像的每个像素
    for (int y = 0; y < new_h; ++y) {
        for (int x = 0; x < new_w; ++x) {
            // 计算目标像素在原图中的坐标
            float x0 = x / scale;
            float y0 = y / scale;

            // 计算原图中的四个邻居点的坐标
            int x1 = (int)x0;
            int y1 = (int)y0;
            int x2 = (x1 < w - 1) ? x1 + 1 : x1;
            int y2 = (y1 < h - 1) ? y1 + 1 : y1;

            // 计算四个邻居点的插值权重
            float dx = x0 - x1;
            float dy = y0 - y1;

            // 遍历每个颜色通道
            for (int i = 0; i < c; ++i) {
                // 计算目标像素的值
                float value = 0;
                value += (1 - dx) * (1 - dy) * in[(y1 * w + x1) * c + i];
                value += dx * (1 - dy) * in[(y1 * w + x2) * c + i];
                value += (1 - dx) * dy * in[(y2 * w + x1) * c + i];
                value += dx * dy * in[(y2 * w + x2) * c + i];

                // 将计算出的值写入目标图像
                out[(y * new_w + x) * c + i] = value;
            }
        }
    }}


// 练习6，实现图像处理算法：直方图均衡化
#include <math.h>
#include <stdlib.h>

void hist_eq(float *in, float *out, int h, int w) {
    // 计算直方图
    int hist[256] = {0};
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            hist[(int)in[i * w + j]]++;
        }
    }

    // 计算累积直方图
    float cum_hist[256] = {0};
    float sum = 0;
    for (int i = 0; i < 256; ++i) {
        sum += hist[i];
        cum_hist[i] = sum / (h * w);
    }

    // 映射原图像的像素值
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int idx = (int)in[i * w + j];
            // 累积直方图值映射到[0, 255]区间
            out[i * w + j] = cum_hist[idx] * 255;
        }
    }
}