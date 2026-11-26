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

int main()
{
    std::filesystem::create_directories("output");
    cv::Mat image(480, 640, CV_8UC3, cv::Scalar(30, 30, 30));
    cv::rectangle(image, cv::Rect(0, 0, 640, 480), cv::Scalar(70, 70, 70), 2);

    std::cout << "Original rows = " << image.rows 
    << ", cols=" << image.cols 
    << ", elemSize=" << image.elemSize() 
    << ", step[0]=" << image.step[0] 
    << ", continuous=" << image.isContinuous() << std::endl; 

    const cv::Rect roiRect(100, 50, 200, 100);
    
    cv::Mat roi = image(roiRect); // 浅拷贝
    cv::Mat roiCopy = image(roiRect).clone(); // 深拷贝

    std::cout << "Original rows = " << roi.rows 
    << ", cols=" << roi.cols 
    << ", elemSize=" << roi.elemSize() 
    << ", step[0]=" << roi.step[0] 
    << ", continuous=" << roi.isContinuous() << std::endl; 

    // 浅拷贝影响原来的
    roi.setTo(cv::Scalar(0,0,255));
    // 深拷贝不影响
    roiCopy.setTo(cv::Scalar(0,255,0));

    cv::rectangle(image, roiRect, cv::Scalar(255, 255, 255), 2);
    cv::imwrite("output/02_mat_roi_shared.png", image);
    cv::imwrite("output/02_mat_roi_clone.png", roiCopy);

    cv::imshow("原始图片", image);
    cv::imshow("深拷贝图片", roiCopy);
    cv::waitKey(0);

    return 0;
}
