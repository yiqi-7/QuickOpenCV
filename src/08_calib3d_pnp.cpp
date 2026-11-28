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
// 相机标定与三维重建
#include <opencv2/calib3d.hpp>


#include <iostream>
#include <vector>

int main()
{
    // 1.相机内参矩阵初始化
    // 正对角线前两个是 fx/fy(像素焦距), 第三列前两个是cx/cy(主点坐标)
    const cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) << 
        800.0, 0.0, 320.0,
        0.0,  800.0, 240.0,
        0.0,  0.0,   1.0);
    // 五个畸变系数全零，假设当前镜头没有畸变  
    const cv::Mat distCoeffs = cv::Mat::zeros(5, 1, CV_64F);

    // 2.构造三维物体
    const std::vector<cv::Point3f> objectPoints = {
      {0,0,0}, {1,0,0}, {1,1,0}, {0,1,0},
      {0,0,1}, {1,0,1}, {1,1,1}, {0,1,1}
    };

    // 3.设置真实位姿
    const cv::Mat trueRvec = (cv::Mat_<double>(3,1) << 0.1, -0.15, 0.05);
    const cv::Mat trueTvec = (cv::Mat_<double>(3,1) << 0.0, 0.0, 6.0);
    // projectPoints 的输出必须是 CV_32F（Point2f），传 Point2d 会触发 create 类型断言
    std::vector<cv::Point2f> imagePoints;

    // 4.三位点投影
    cv::projectPoints(objectPoints, trueRvec, trueTvec, cameraMatrix, distCoeffs, imagePoints);

    // 5.位姿反向估计 
    // slovePnPg 根据3D-2D对应关系估计物体对于相机的位姿
    cv::Mat estimatedRvec, estimatedTvec; 
    // 根据3D-2D对应点、内参和畸变反求位姿
    const bool success = cv::solvePnP(
        objectPoints, imagePoints, 
        cameraMatrix, distCoeffs, 
        estimatedRvec, estimatedTvec,
        false, cv::SOLVEPNP_ITERATIVE
    );

    // 6.输出估计结果 
    if (!success) {
        std::cerr << "slovePnp failed" << std::endl;
        return 1;
    }

    std::cout << "Estimated rotation vector: \n" << estimatedRvec << std::endl;
    std::cout << "Estimated translation vector: \n" << estimatedTvec << std::endl;

    std::cout << "\n" << "-----------------------------------" << "\n";

    std::cout << "rvec error: " << cv::norm(estimatedRvec - trueRvec) << '\n';
    std::cout << "tvec error: " << cv::norm(estimatedTvec - trueTvec) << '\n';

    std::cout << "\n" << "-----------------------------------" << "\n";

    cv::Rect box = cv::boundingRect(imagePoints);
std::cout << "projected bbox: " << box << '\n';

    return 0;


}
