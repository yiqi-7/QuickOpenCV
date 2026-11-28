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

int main(int argc, char** argv)
{
    // TODO: 按上面的 1~6 步自己实现。卡住了就翻 docs/动手学OpenCV_自学手册.md 对应那一课（含完整源码与逐段讲解）。
    (void)argc;
    (void)argv;
    return 0;
}
