// ============================================================================
// 练习 08：相机几何与 solvePnP
// 对应手册：docs/动手学OpenCV_自学手册.md 第 9 课
//
// 本课目标
//   1. 构造边长 1 的立方体 8 个 cv::Point3f 角点
//   2. 构造内参 K（fx=fy=800, cx=320, cy=240）与全零畸变系数
//   3. 指定真值 rvec / tvec，用 projectPoints 得到 2D 点
//   4. 用 solvePnP 从 3D-2D 对应关系反求位姿，检查返回值
//   5. 打印估计值与真值，用 cv::norm 算差值（合成数据应接近 0）
//   6. 把 tvec 的 z 从 6 改成 3，观察 2D 投影点分布变宽
//
// 拓展（可选）
//   用 assets/chessboard.png + findChessboardCorners + cornerSubPix 找角点，
//   理解真实标定的输入长什么样。
//
// 验收
//   估计 tvec 与真值差在 1e-6 量级
//   能解释 solvePnP 需要几个点、为什么内参必须已知
// ============================================================================
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>

#include <iostream>
#include <vector>

int main()
{
    // TODO: 按上面的 1~6 步自己实现。卡住了就翻 docs/动手学OpenCV_自学手册.md 对应那一课（含完整源码与逐段讲解）。
    return 0;
}
