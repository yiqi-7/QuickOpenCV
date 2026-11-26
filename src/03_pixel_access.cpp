// ============================================================================
// 练习 03：三种像素遍历方式
// 对应手册：docs/动手学OpenCV_自学手册.md 第 4 课
//
// 本课目标
//   1. 生成 1280x720 的三通道随机噪声图（cv::randu）
//   2. 用 at()、ptr()、迭代器 三种方式各做一次“亮度乘 1.2”（用 saturate_cast）
//   3. 用 convertTo(dst, -1, 1.2, 0.0) 做第四种
//   4. 用 cv::norm(a, b, cv::NORM_INF) 两两比较，打印最大差值（应接近 0）
//   5. 用 std::chrono::steady_clock 分别计时并打印
//   6. 保存 output/03_pixel_ptr_result.png
//
// 验收
//   三种手写结果与 convertTo 的最大差值 <= 1
//   能解释 convertTo 为什么通常最快
//   能解释 at() 在 Debug 有断言、Release 没有
// ============================================================================
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <iostream>

// 生成图片
static cv::Mat makeInput()
{
    cv::Mat image(720, 1280, CV_8UC3);
    cv::randu(image, cv::Scalar::all(0), cv::Scalar::all(180));
    return image;
}

/*
    下面执行三种访问 亮度乘上1.2
*/

// At访问 
static void processWithAt(const cv::Mat& src, cv::Mat& dst)
{
    dst.create(src.size(), src.type());

    for (int y = 0; y < src.rows; ++y) {
        for (int x = 0; x < src.cols; ++x) {
            const cv::Vec3b pixel = src.at<cv::Vec3b>(y, x);
            cv::Vec3b& out = dst.at<cv::Vec3b>(y, x);
            for (int c = 0; c < 3; ++c) {
                out[c] = cv::saturate_cast<uchar>(pixel[c] * 1.2);
            }
        }
    }
}

static void processWithPtr(const cv::Mat& src, cv::Mat& dst)
{
    dst.create(src.size(), src.type());

    for (int y = 0; y < src.rows; ++y) {
        const cv::Vec3b* srcRow = src.ptr<cv::Vec3b>(y);
        cv::Vec3b* dstRow = dst.ptr<cv::Vec3b>(y);
        for (int x = 0; x < src.cols; ++x) {
            for (int c = 0; c < 3; ++c) {
                dstRow[x][c] = cv::saturate_cast<uchar>(srcRow[x][c]*1.2);
            }
        }
    }
}

static void processIterator(const cv::Mat& src, cv::Mat& dst)
{
    dst.create(src.size(), src.type());

    auto in = src.begin<cv::Vec3b>();
    auto inEnd = src.end<cv::Vec3b>();
    auto out = dst.begin<cv::Vec3b>();
    auto outEnd = dst.end<cv::Vec3b>();

    for (; in != inEnd; ++in, ++out) {
        for (int c = 0; c < 3; ++c) {
            (*out)[c] = cv::saturate_cast<uchar>((*in)[c] * 1.2);
        }
    }
}

int main()
{
    
    
    return 0;
}
