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
    const int cameraIndex = (argc >= 2) ? std::stoi(argv[1]) : 0;
    cv::VideoCapture camera(cameraIndex);
    if (!camera.isOpened()) {
        std::cerr << "无法打开摄像头" << std::endl;
        return 1;
    }

    std::filesystem::create_directories("output");
    int mode = 1;
    cv::Mat frame, output, gray, blurred;

    std::cout << "1: Original | 2: Gray | 3: Gaussian | 4: Canny" << std::endl << "S: Save Current result | ESC: Quit" << std::endl; 

    while (camera.read(frame)) {
        if (frame.empty()) continue;

        switch (mode)
        {
        case 1:
            output = frame;
            break;
        case 2: // 转灰度
            cv::cvtColor(frame, output, cv::COLOR_BGR2GRAY);
            break;
        case 3: // 高斯滤波
            cv::GaussianBlur(frame, output, cv::Size(5, 5), 1.0);
            break;
        case 4:
            // Canny 只接受单通道：先灰度，再平滑，最后检测边缘
            cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
            cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 1.0);
            // 低/高阈值为 50/150
            cv::Canny(blurred, output, 50, 150);
            break;

        default:
            output = frame;
            break;
        }

        cv::imshow("Camera", frame);
        cv::imshow("Processing result", output);

        const int key = cv::waitKey(1) & 0xFF;
        if (key == 27) break;
        if (key >= '1' && key <= '4') {
            mode = key - '0';
        }
        else if (key == 's' || key == 'S') {
            const std::string filename = "output/10_mode_" + std::to_string(mode) + ".png";
            if(!cv::imwrite(filename, output)) {
                std::cerr << "保存失败: " << filename << std::endl;
            }
            else {
                std::cout << "保存成功: " << filename << std::endl;
            }
        }
    }

    // 释放摄像头和全部窗口
    camera.release();
    cv::destroyAllWindows();

    return 0;
}
