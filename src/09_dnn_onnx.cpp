// ============================================================================
// 练习 09：DNN 推理流程
// 对应手册：docs/动手学OpenCV_自学手册.md 第 10 课
//
// 本课目标
//   1. 参数不足 3 个时打印用法并 return 0（不算错误）
//   2. 读图，检查 empty()
//   3. readNetFromONNX 包在 try/catch 里；再用 net.empty() 判一次
//   4. blobFromImage 做预处理（尺寸、缩放、swapRB 必须与模型一致）
//   5. setPreferableBackend/Target -> setInput -> forward 到 vector<cv::Mat>
//   6. 遍历输出，打印 dims / total() / type() 与每一维 size[d]
//
// 说明
//   本课程不附带 ONNX 模型。你可以自己导出一个，或者只验证“无参数/错误路径”的行为。
//
// 验收
//   无参数运行时打印用法、退出码 0
//   模型路径错误时打印清晰错误、退出码 1、不崩溃
// ============================================================================
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/dnn.hpp>

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    if (argc < 3) {
        std::cout << "Usage: 09_dnn_onnx <model.onnx> <image> \n" << "需要按照上面的格式来启动程序" << std::endl;
        return 0;
    }

    // 保存路径 
    const std::string modelPath = argv[1];
    const std::string imagePath = argv[2];

    // DNN输入通常从彩色开始，后续再做统一blob预处理 
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "无法读取图片: " << imagePath << std::endl;
        return 1;
    }

    cv::dnn::Net net;
    try {
        // 读取ONNX
        net = cv::dnn::readNetFromONNX(modelPath);
    } catch(const cv::Exception& e) {
        std::cerr << "无法加载模型: " << e.what() << std::endl;
        return 1;
    }

    if (net.empty()) {
        std::cerr << "模型为空"<< std::endl;
        return 1;
    }

    // 开始使用模型 常见大小，缩放到[0, 1], 交换B/R
    // 将普通的图片转化为神经网络需要的输入张量
    cv::Mat blob = cv::dnn::blobFromImage(
        //     像素缩放到[0, 1]  跳转图像尺寸
        image, 1.0 / 255.0, cv::Size(640, 640),
        // 不设额外均值    BGR转RGB     不进行中心裁剪
        cv::Scalar(),      true,       false);
    
    // 选择推理后端
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    // 选择执行设备
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    // 使用处理后的bolb设置当作输入
    net.setInput(blob);

    try {
        std::vector<cv::Mat> outputs;
        // 执行向前推理，并收集所有输出层的结果
        net.forward(outputs, net.getUnconnectedOutLayersNames());
        std::cout << "输出向量个数: " << outputs.size() << std::endl;

        for (std::size_t i = 0; i < outputs.size(); ++i) {

            // 输出张量数量
            std::cout << "dims=" << outputs[i].dims << std::endl; 
            // 张量包含元素总数
            std::cout << "total=" << outputs[i].total() << std::endl;
            // 张量的数据类型以及通道信息
            std::cout << "type=" << outputs[i].type() << std::endl;

            
            std::cout << "Shape :";
            for (int d = 0; d < outputs[i].dims; ++d) {
                // 	第d个维度的大小
               std::cout << "第d个维度的大小" << outputs[i].size[d] << std::endl;
            }
        }
    
    } catch (const cv::Exception& e) {
        std::cerr << "推理失败: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "推理完成" << std::endl;
    return 0;
}
