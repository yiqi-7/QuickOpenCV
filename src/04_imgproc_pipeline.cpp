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

namespace fs = std::filesystem;

static cv::Mat makeDemoImage()
{  
    cv::Mat image(480, 640, CV_8UC3, cv::Scalar(235, 235, 235));
    cv::rectangle(image, cv::Rect(80, 70, 180, 250), cv::Scalar(30, 80, 220), 01);
    cv::circle(image, cv::Point(430, 170), 85, cv::Scalar(40, 180, 70), -1);
    cv::line(image, cv::Point(50, 390), cv::Point(590, 390), cv::Scalar(200, 60, 20), 12);
    cv::putText(image, "Image Processing", cv::Point(120, 455), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(20, 20, 20), 2, cv::LINE_AA);

    return image;
}

int main(int argc, char** argv)
{
    fs::create_directories("output");

    cv::Mat src = (argc >= 2) ? cv::imread(argv[1], cv::IMREAD_COLOR) : makeDemoImage(); 

    if (src.empty()) {
        std::cerr << "输入图像为空， 检测路径" << std::endl;
        return 1;
    }

    cv::Mat gray, gaussian, median, binary, edges, morphology;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY); // 彩色转化为灰度图
    cv::GaussianBlur(gray, gaussian, cv::Size(5, 5), 1.2); // 亮度1.2，且使用高斯滤波
    cv::medianBlur(gray, median, 5); // 核大小为5
    cv::threshold(gray, binary, 127, 255, cv::THRESH_BINARY);  // 固定阈值二值化
    cv::Canny(gaussian, edges, 50, 150);

    const cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    // 做开运算
    cv::morphologyEx(binary, morphology, cv::MORPH_OPEN, kernel); 
    
    cv::imwrite("output/04_source.png", src);
    cv::imwrite("output/04_gray.png", gray);
    cv::imwrite("output/04_gaussian.png", gaussian);
    cv::imwrite("output/04_median.png", median);
    cv::imwrite("output/04_binary.png", binary);
    cv::imwrite("output/04_canny.png", edges);
    cv::imwrite("output/04_morphology_open.png", morphology);
    
    cv::imshow("Source", src);
    cv::imshow("Gray", gray);
    cv::imshow("Gaussion", gaussian);
    cv::imshow("Canny", edges);
    cv::waitKey(0);
    
    return 0;
}
