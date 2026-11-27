// ============================================================================
// 练习 07：ORB 特征检测与匹配
// 对应手册：docs/动手学OpenCV_自学手册.md 第 8 课
//
// 本课目标
//   1. 读 assets/box.png 与 assets/box_in_scene.png
//   2. 都转成灰度（ORB 只接受单通道）
//   3. ORB::create(1000) 后对两张图各 detectAndCompute 一次
//   4. 打印两边关键点数；描述子为空要提前返回
//   5. BFMatcher(NORM_HAMMING, true) + match 得到匹配
//   6. drawMatches 可视化，存 output/07_orb_matches.png 并显示
//   7. 打印匹配数；再试 crossCheck=false + knnMatch + ratio test 比较效果
//
// 验收
//   匹配连线大体上是同一批角点，不是随机乱连
//   能解释为什么用汉明距离
//   能解释 crossCheck 做了什么
// ============================================================================
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <filesystem>
#include <iostream>
#include <vector>

int main()
{
    // 1.创建输出目录 
    std::filesystem::create_directories("output");

    //  2.输入转灰度
    cv::Mat img1 = cv::imread("assets/box.png", cv::IMREAD_GRAYSCALE);
    cv::Mat img2 = cv::imread("assets/box_in_scene.png", cv::IMREAD_GRAYSCALE);
    if (img1.empty() || img2.empty()) {
        std::cerr << "图片打开错误" << std::endl;
        return 1; 
    }

    // 3.关键点 + 描述子

    // 创建ORB, 保留最大关键点数是1000, 不一定是1000个关键点
    const cv::Ptr<cv::ORB> orb = cv::ORB::create(1000);
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;

    // 一次完成关键点检测和二进制描述子计算: noArray 表示不限制区域 
    orb->detectAndCompute(img1, cv::noArray(), keypoints1, descriptors1);
    orb->detectAndCompute(img2, cv::noArray(), keypoints2, descriptors2);

    // 4.打印点数 + 判空: 低纹理可能0个点
    std::cout << "Keypoints image 1: " << keypoints1.size() << std::endl;
    std::cout << "Keypoints image 2: " << keypoints2.size() << std::endl;

    if (descriptors1.empty() || descriptors2.empty()) {
        std::cerr << "无描述子" << std::endl; 
        return 1;
    }

    // 5.ORB的描述子是二进制描述子，用汉明码（二进制必须用）
    // 第二个参数代表采用双向验证
    cv::BFMatcher matcher(cv::NORM_HAMMING, true);
    std::vector<cv::DMatch> matches; 
    // 为每个查询描述子找最近的训练描述子
    //            查询描述子     训练描述子    输出匹配结果
    // 每个DMatch中记录第一张图中描述子索引、第二张图中描述子索引、两个描述子之间的距离
    matcher.match(descriptors1, descriptors2, matches);
    std::cout << "Matches: " << matches.size() << std::endl;

    // 6.匹配可视化
    cv::Mat matchImage;
    //两图并排拼接，并绘制匹配关键点之间的连线
    cv::drawMatches(img1, keypoints1, img2, keypoints2, matches, matchImage);

    if (!cv::imwrite("output/07_orb_matches.png", matchImage)) {
        std::cerr << "图片保存失败" << std::endl;
        return -1;
    }
    cv::imshow("ORB feature matching", matchImage);

    cv::waitKey(0);
    return 0;
}
