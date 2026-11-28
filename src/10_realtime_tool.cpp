// ============================================================================
// 练习 10：综合实时图像处理工具
// 对应手册：docs/动手学OpenCV_自学手册.md 第 11 课
//
// 必须实现
//   1. 打开摄像头（索引可从 argv[1] 传入），失败提示并返回 1
//   2. 用 int mode 记录当前模式，默认 1
//   3. 主循环：读帧 -> switch(mode) 生成 output -> 显示原图与结果 -> waitKey(1)
//       1 原图 / 2 灰度 / 3 高斯滤波 / 4 Canny
//   4. 键盘：'1'~'4' 切模式（key - '0'）、s/S 保存、27 退出
//   5. 保存文件名带模式号，避免互相覆盖
//   6. 退出时 release() + destroyAllWindows()
//
// 拓展（必做项完成后）
//   HSV 颜色筛选 + inRange；形态学清理 + findContours 画外接矩形；左上角显示 FPS
//
// 验收
//   四个模式切换流畅
//   保存的文件名与当前模式对应
//   空帧、打不开摄像头、按 ESC 三种情况都不异常退出
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
    // TODO: 按上面的 1~6 步自己实现。卡住了就翻 docs/动手学OpenCV_自学手册.md 对应那一课（含完整源码与逐段讲解）。
    (void)argc;
    (void)argv;
    return 0;
}
