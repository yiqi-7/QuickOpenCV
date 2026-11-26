// ============================================================================
// 练习 00：第一个 OpenCV 程序
// 对应手册：docs/动手学OpenCV_自学手册.md 第 1 课
//
// 本课目标
//   1. 打印 OpenCV 版本号（CV_VERSION）
//   2. 创建一张 640x360 的三通道 8 位图像，填充深色背景
//   3. 用 putText 写一行 "Hello OpenCV"，字号 1.5、线宽 3、颜色与背景可区分
//   4. 保存到 output/00_hello_opencv.png，并检查 imwrite 的返回值
//   5. 打印 cols / rows / channels()
//   6. imshow + waitKey(0) 显示窗口
//
// 自己查文档时重点看
//   cv::Mat 构造函数参数顺序（先 rows 后 cols）
//   cv::Scalar 是 BGR 顺序
//   cv::putText 的 org 是文字左下角坐标
//
// 验收
//   控制台打印 4.13.0 与 640 x 360 / 3
//   output/00_hello_opencv.png 生成且能看到文字
//   窗口弹出，按键后进程返回 0
// ============================================================================
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

int main()
{
    // TODO: 按上面的 1~6 步自己实现。卡住了就翻 docs/动手学OpenCV_自学手册.md 对应那一课（含完整源码与逐段讲解）。
    std::cout << "Opencv version:" << CV_VERSION << '\n';
    //  创建图片        BGR
    cv::Mat canvas(360, 640, CV_8UC3, cv::Scalar(80, 40, 20));
    //  在图片上输出文字（FONT_HERSHEY_SIMPLEX） 坐标为文字的左下角像素坐标
    cv::putText(canvas, "Hello OpenCV", cv::Point(90, 180),
                cv::FONT_HERSHEY_SIMPLEX, 1.5, 
            cv::Scalar(0, 255, 255), 3, cv::LINE_AA);

    if (!cv::imwrite("output/00_hello_openve.png", canvas)) {
        std::cerr << "Failed to save png" << std::endl;
        return 1;
    }
    
    std::cout << "Image saved !" << std::endl;
    std::cout << "Size:"  << canvas.cols << " x " << canvas.rows 
              << ", channels:" << canvas.channels() << std::endl;

    cv::imshow("Fisrt OpenCV Program", canvas);
    cv::waitKey(0);

    return 0;
}
