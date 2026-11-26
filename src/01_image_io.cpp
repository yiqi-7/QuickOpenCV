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

namespace fs = std::filesystem;

// 生成一个图片
static cv::Mat makeDemoImage()
{
    cv::Mat image(480, 640, CV_8UC3, cv::Scalar(235, 235, 235));
    // 画 矩形 圆 线 文字
    cv::rectangle(image, cv::Rect(70, 80, 210, 260), cv::Scalar(40, 80, 220), 5);
    cv::circle(image, cv::Point(430, 190), 90, cv::Scalar(40, 180,60), -1);
    cv::line(image, cv::Point(60, 410), cv::Point(580, 410),
            cv::Scalar(200, 70, 30), 8);
    cv::putText(image, "OpenCV image I/O", cv::Point(100, 455),
                    cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(20, 20, 20), 2, cv::LINE_AA);
    return image;
}

int main(int argc, char** argv)
{
    // TODO: 按上面的 1~6 步自己实现。卡住了就翻 docs/动手学OpenCV_自学手册.md 对应那一课（含完整源码与逐段讲解）。
    fs::create_directories("output");

    // 参数1 指定图片路径 
    cv::Mat image;
    if (argc >= 2) {
        image = cv::imread(argv[1], cv::IMREAD_COLOR);
        if (image.empty()) {
            std::cerr << "无法读取图片: " <<argv[1] << std::endl;
            return 1;
        }
    }
    else {
        image = makeDemoImage();
        cv::imwrite("output/01_generated_input.png", image);
    }  

    std::cout << "Width: " << image.cols << std::endl 
    << "Height: " << image.rows << std::endl
    << "Channels: " << image.channels() << std::endl
    << "Depth: " << image.depth() << std::endl
    << "Type code: " << image.type() << std::endl; 

    const std::string outputPath = "output/01_image_copy.jpg";
    if (!cv::imwrite(outputPath, image)) {
        std::cerr << "图片写入失败: " << outputPath << std::endl;
        return 1;
    }

    cv::imshow("Image I/O - input", image);
    cv::waitKey(0);
    
    return 0;
}
