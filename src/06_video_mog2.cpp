// ============================================================================
// 练习 06：MOG2 背景建模
// 对应手册：docs/动手学OpenCV_自学手册.md 第 7 课
//
// 本课目标
//   1. 打开视频源：有参数当文件路径，否则用摄像头 0
//   2. 用 createBackgroundSubtractorMOG2() 创建背景减除器
//   3. 循环读帧：apply(frame, fgmask) 得到单通道掩码
//   4. 同时显示原图与掩码，waitKey(1)，ESC 退出
//   5. 调 setVarThreshold / setHistory，观察掩码变化
//
// 验收
//   画面静止时掩码基本全黑（背景学习完成）
//   有物体移动时出现白色前景块
//   能解释为什么前几十帧掩码很乱
// ============================================================================
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    cv::VideoCapture capture;
    if (argc >= 2) {
        capture.open(argv[1]);
    }
    else {
        capture.open(0);
    }

    if (!capture.isOpened()) {
        std::cerr << u8"打开失败" << std::endl; 
        return 1;
    }

    cv::Ptr<cv::BackgroundSubtractor> mog2 = 
    cv::createBackgroundSubtractorKNN();
    // cv::createBackgroundSubtractorMOG2();

    cv::Mat frame, foreground;
    std::cout << u8"MOG2 背景提取, 按ESC退出" << std::endl; 

    while (capture.read(frame)) {
        if (frame.empty()) break;

        // 更新背景模型 生成单通道前景掩码 
        mog2->apply(frame, foreground);
        cv::imshow("Input video", frame);
        // cv::imshow("MOG2 foreground mask", foreground);
        cv::imshow("KNN foreground mask", foreground);

        if ((cv::waitKey(1) & 0xFF) == 27) break;
    }

    // 释放资源
    capture.release();
    cv::destroyAllWindows(); // 关闭所有窗口
    
    return 0;
}

