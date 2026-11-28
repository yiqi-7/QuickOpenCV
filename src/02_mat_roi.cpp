// ============================================================================
// 练习 02：Mat 的内存、ROI 与深浅拷贝
// 对应手册：docs/动手学OpenCV_自学手册.md 第 3 课
//
// 本课目标
//   1. 创建 640x480 三通道图，画一个边框，让 ROI 位置看得出来
//   2. 打印原图 rows / cols / step[0] / elemSize() / isContinuous()
//   3. 取 Rect(100, 50, 200, 120) 的 ROI（浅拷贝）并 setTo 成红色
//   4. 同区域再 clone() 一份（深拷贝）并 setTo 成绿色
//   5. 打印 ROI 的 step[0] / cols*elemSize() / isContinuous()，与原图对比
//   6. 保存 output/02_mat_roi_shared.png 与 output/02_mat_roi_clone.png
//   7. 再试 Rect(0, 50, 640, 120)（整行宽度），观察 isContinuous() 是否变化
//
// 验收
//   原图里那块区域变红 -> 证明共享内存
//   clone 出的图是独立绿色块，原图不受影响
//   能解释 ROI 的 step[0] 为什么等于原图 step[0] 而不是 200*3
// ============================================================================

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <filesystem>
#include <iostream>

int main(int argc, char** argv)
{
    // TODO: 按上面的 1~7 步自己实现。卡住了就翻 docs/动手学OpenCV_自学手册.md 对应那一课（含完整源码与逐段讲解）。
    (void)argc;
    (void)argv;
    return 0;
}
