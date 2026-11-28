// ============================================================================
// 练习 04：imgproc 处理流水线
// 对应手册：docs/动手学OpenCV_自学手册.md 第 5 课
//
// 本课目标
//   1. 读 assets/sample.jpg（可换成 assets/butterfly.jpg），检查 empty()
//   2. 灰度化 -> 存 output/04_gray.png
//   3. 高斯滤波 Size(5,5) sigma 1.2 -> 存 output/04_gaussian.png
//   4. 中值滤波 ksize 5 -> 存 output/04_median.png
//   5. 对灰度图固定阈值 127 二值化 -> 存 output/04_binary.png
//   6. 对高斯结果做 Canny(50, 150) -> 存 output/04_canny.png
//   7. 对二值图做开运算 -> 存 output/04_morphology_open.png
//   8. 显示原图 / 灰度 / Canny，waitKey(0)
//
// 验收
//   7 张 output/04_*.png 齐全
//   能解释 Canny 前为什么要先高斯滤波
//   把 Canny 改成 (200, 250) 后能解释边缘变少的原因
// ============================================================================
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <filesystem>
#include <iostream>

int main(int argc, char** argv)
{
    // TODO: 按上面的 1~8 步自己实现。卡住了就翻 docs/动手学OpenCV_自学手册.md 对应那一课（含完整源码与逐段讲解）。
    (void)argc;
    (void)argv;
    return 0;
}
