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
    // TODO: 按上面的 1~7 步自己实现。卡住了就翻 docs/动手学OpenCV_自学手册.md 对应那一课（含完整源码与逐段讲解）。
    return 0;
}
