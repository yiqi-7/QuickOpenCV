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
    // TODO: 按上面的 1~6 步自己实现。卡住了就翻 docs/动手学OpenCV_自学手册.md 对应那一课（含完整源码与逐段讲解）。
    (void)argc;
    (void)argv;
    return 0;
}
