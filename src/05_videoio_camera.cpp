// ============================================================================
// 练习 05：摄像头与视频帧
// 对应手册：docs/动手学OpenCV_自学手册.md 第 6 课
//
// 本课目标
//   1. argv[1] 可选，作为摄像头索引，默认 0
//   2. 打开后检查 isOpened()，失败打印提示并返回 1
//   3. 循环：读帧 -> 判空 -> 按模式决定显示原图还是灰度 -> 显示 -> 读键盘
//   4. 按键：27 退出、g/G 切换灰度、s/S 保存当前输出到 output/05_camera_*.png
//   5. 退出前 release() 并 destroyAllWindows()
//
// 没有摄像头怎么办
//   把 VideoCapture(0) 换成 VideoCapture("assets/你的视频.mp4")，循环逻辑完全一样。
//
// 验收
//   窗口实时刷新，按键能切换灰度
//   按 S 后 output/ 出现图片
//   没有摄像头时给出明确提示而不是崩溃
// ============================================================================
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    // TODO: 按上面的 1~5 步自己实现。卡住了就翻 docs/动手学OpenCV_自学手册.md 对应那一课（含完整源码与逐段讲解）。
    (void)argc;
    (void)argv;
    return 0;
}
