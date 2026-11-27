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
    // 选摄像头
    const int cameraIndex = (argc >= 2) ? std::stoi(argv[1]) : 0;
    cv::VideoCapture capture(cameraIndex);

    // 检测设备是否打卡
    if (!capture.isOpened()) {
        std::cerr << "无法打开摄像头，索引为: " << cameraIndex << std::endl;
        return 1;
    }

    std::cout << "Camera opened. Press G to toggle grayscale, S to save, ESC to quit."
              << std::endl;

    bool garyscale = false; 
    cv::Mat frame, output;

    // 创建输出目录 
    std::filesystem::create_directories("output");

    while (true) {
        if (!capture.read(frame) || frame.empty()) {
            std::cerr << "捕获帧失败" << std::endl;
            break;
        }

        if (garyscale) {
            cv::cvtColor(frame, output, cv::COLOR_BGR2GRAY);
        }
        else {
            // Mat浅拷贝
            output = frame; 
        }

        // Windows HighGUI 对 UTF-8 中文窗口标题的支持不稳定，使用 ASCII 标题避免乱码。
        cv::imshow("Camera Input", frame);
        cv::imshow("Camera Output", output);

        // 等待窗口刷新和键盘输入 
        const int key = cv::waitKey(1) & 0xFF;
        if (key == 27) { // ESC
            break; 
        }
        else if (key == 'g' || key == 'G') {
            garyscale = !garyscale;
        }
        else if (key == 's' || key == 'S') {
            const std::string path = garyscale ? "output/05_camera_gray.png" : "output/05_camera_color.png";

            if (cv::imwrite(path, output)) {
                std::cout << "保存成功: " << path << std::endl;
            } 
            else {
                std::cerr << "保存失败: " << path << std::endl;
            }
        }
    }

    // 释放资源 
    capture.release();
    cv::destroyAllWindows();

    return 0;
}
