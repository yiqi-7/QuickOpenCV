// ============================================================================
// 练习 01：图像读写
// 对应手册：docs/动手学OpenCV_自学手册.md 第 2 课
//
// 本课目标
//   1. 有命令行参数就用 argv[1] 作路径，否则用 assets/sample.jpg
//   2. imread 之后立刻检查 empty()，为空则报错返回 1
//   3. 打印 cols / rows / channels() / depth() / type()
//   4. 另存为 output/01_image_copy.png（记得先创建 output 目录）
//   5. 再用 IMREAD_GRAYSCALE 读一次，打印通道数并对比
//   6. 故意传一个不存在的路径，确认你的错误处理生效
//
// 验收
//   彩色图 channels()==3，灰度图 channels()==1
//   output/01_image_copy.png 与原图一致
//   错误路径时打印提示、返回码 1、不崩溃
// ============================================================================

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    // TODO: 按上面的 1~6 步自己实现。卡住了就翻 docs/动手学OpenCV_自学手册.md 对应那一课（含完整源码与逐段讲解）。
    (void)argc;
    (void)argv;
    return 0;
}
