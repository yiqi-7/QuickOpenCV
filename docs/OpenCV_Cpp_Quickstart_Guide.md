# OpenCV C++ 快速入门讲义

**目标读者：** 掌握 C++ 基础语法、希望快速建立 OpenCV 整体认识的学生。  
**建议课时：** 8～10 小时。  
**开发环境：** VS Code、C++17、CMake 3.16+、OpenCV 4.x。  
**配套代码：** 本目录 `src/` 下的 11 个 C++ 程序；练习步骤与验收标准见 `动手学OpenCV_自学手册.md` 的各课和附录 D。

> 注意：`src/` 里的 11 个文件目前是**空壳**（只有目标、API 提示和 TODO），代码需要你自己写。
> 每个程序的完整源码与逐段讲解在 `动手学OpenCV_自学手册.md`，对照该手册完成 `src/` 里的实现。

> 学习方式：每读完一节，先打开对应 `.cpp` 文件，按文件头的步骤自己实现并运行，再完成对应练习。不要只阅读 API 名称。

---

## 学习地图

| 讲义章节 | 配套代码 | 核心产出 |
|---|---|---|
| 1. OpenCV 是什么 | `00_first_program.cpp` | 认识 OpenCV 和第一个 Mat |
| 2. 模块架构 | 全部示例 | 知道任务应该使用哪个模块 |
| 3. 环境配置 | 根目录 `CMakeLists.txt` | 能够构建整个项目 |
| 4. core 与 Mat | `02_mat_roi.cpp`、`03_pixel_access.cpp` | 理解图像数据与内存 |
| 5. 图像 I/O | `01_image_io.cpp` | 读、显示、保存图片 |
| 6. imgproc | `04_imgproc_pipeline.cpp` | 运行常用图像处理 |
| 7. videoio 与 video | `05_videoio_camera.cpp`、`06_video_mog2.cpp` | 采集和分析视频 |
| 8. features2d、calib3d | `07_features2d_orb.cpp`、`08_calib3d_pnp.cpp` | 了解传统视觉 |
| 9. dnn | `09_dnn_onnx.cpp` | 了解模型推理流程 |
| 10. 性能与工程问题 | `03_pixel_access.cpp` 及前述示例 | 建立基本性能意识 |
| 11. 综合项目 | `10_realtime_tool.cpp` | 完成实时图像处理工具 |

# 第一章 OpenCV 是什么

## 1.1 定义与定位

OpenCV（Open Source Computer Vision Library）是开源计算机视觉与图像处理库。它提供 C++ 等语言的 API，帮助程序完成图像读写、像素处理、滤波、几何变换、特征提取、视频分析、相机标定和模型推理等任务。

可以把计算机视觉应用拆成一条数据链：

```text
图像/视频来源 -> 图像数据 -> 预处理 -> 算法分析 -> 结果显示/保存/传输
```

OpenCV 在这条链中提供大量现成组件，但它不是完整的应用程序，也不替代 C++、GUI 框架、视频编解码系统或深度学习训练框架。

## 1.2 OpenCV 与相关技术的关系

| 技术 | 主要职责 | 典型任务 |
|---|---|---|
| C++ | 编程语言 | 组织程序、封装算法和系统逻辑 |
| OpenCV | 图像处理与计算机视觉 | 图像增强、特征提取、视频分析 |
| FFmpeg | 音视频编解码及流处理 | 视频解码、编码、转码 |
| OpenGL | 图形渲染 API | 2D/3D 图形显示、渲染管线 |
| CUDA | NVIDIA GPU 通用计算平台 | GPU 并行计算 |
| PyTorch | 深度学习开发与训练框架 | 网络训练、模型导出 |

它们可以组合使用。例如，C++ 负责应用主流程，OpenCV 负责图像预处理，PyTorch 训练模型并导出 ONNX，OpenCV DNN 或其他推理运行时负责部署模型。

## 1.3 OpenCV 能做什么

1. **图像处理：** 灰度转换、滤波、阈值分割、边缘检测、形态学处理、几何变换。
2. **视频处理：** 摄像头采集、视频读取、逐帧处理、运动分析、背景建模。
3. **传统视觉：** 特征检测与匹配、相机标定、位姿估计、图像拼接。
4. **深度学习推理：** 加载部分受支持的模型格式并执行前向计算。

## 1.4 学习目标

第一轮学习不要求背诵全部算法，而要求学生能回答：输入数据是什么、用哪个模块处理、输出是什么、数据怎样存储以及如何验证结果。

**对应代码：** `src/00_first_program.cpp`。先编译运行，观察版本号、图像尺寸和保存结果。

# 第二章 OpenCV 模块架构

OpenCV 按功能组织成多个模块。以下按 OpenCV 4.x 的常见主模块介绍；具体安装包是否包含某模块，取决于构建配置。

| 模块 | 核心职责 | 常见类/API | 典型任务 |
|---|---|---|---|
| `core` | 基础数据结构、矩阵运算、内存管理 | `Mat`、`Point`、`Rect`、`Scalar` | 图像数据管理 |
| `imgproc` | 图像处理 | `cvtColor`、`resize`、`GaussianBlur`、`Canny` | 预处理和传统图像算法 |
| `imgcodecs` | 图像文件读写与编解码 | `imread`、`imwrite`、`imdecode` | 图片加载、保存 |
| `highgui` | 简单窗口显示和交互 | `imshow`、`waitKey`、`createTrackbar` | 调试显示 |
| `videoio` | 视频源输入输出 | `VideoCapture`、`VideoWriter` | 摄像头、视频文件 |
| `video` | 视频分析 | 光流、背景建模、`KalmanFilter` | 帧间运动与状态分析 |
| `features2d` | 2D 特征框架 | `ORB`、`SIFT`、`BFMatcher` | 特征检测与匹配 |
| `calib3d` | 相机标定和三维重建 | `calibrateCamera`、`solvePnP` | 相机几何、位姿估计 |
| `objdetect` | 目标检测相关接口 | `CascadeClassifier`、`HOGDescriptor`、部分 DNN 检测接口 | 人脸、行人等检测 |
| `dnn` | 神经网络加载和前向推理 | `readNet`、`blobFromImage`、`forward` | 模型部署推理 |
| `photo` | 计算摄影 | 去噪、修复、HDR 等 | 图像增强 |
| `ml` | 传统机器学习 | SVM、KNN、随机森林 | 传统分类与回归 |
| `flann` | 高维近似最近邻搜索 | FLANN 匹配器 | 特征匹配加速 |
| `stitching` | 图像拼接 | 拼接流水线 | 全景图 |

### 2.1 最容易混淆的模块

- `imgcodecs`：把磁盘图片文件解码为 Mat，或把 Mat 编码写入文件。
- `highgui`：把 Mat 显示到窗口，并处理简单键盘/滑动条交互。
- `videoio`：从摄像头或视频文件获取帧，也可以把帧写成视频。
- `video`：分析视频帧之间的运动和变化。
- `imgproc`：处理图像本身，例如颜色变换、滤波、轮廓和形态学。
- `dnn`：网络推理基础设施；`objdetect` 中也有目标检测封装接口，二者不是完全互斥。

### 2.2 contrib 扩展模块

`opencv_contrib` 是 OpenCV 的扩展模块集合，包含一些不属于标准主模块集的额外算法。使用时需要确认 OpenCV 构建时包含相应 contrib 模块。不要假设所有预编译包都带有它们。

**本章练习：** 看一遍 `src/` 目录，根据文件内容为每个程序标注其使用的 OpenCV 模块。

# 第三章 环境配置与项目构建

## 3.1 工具链各自负责什么

- VS Code：代码编辑、调试入口和任务管理。
- C++ 编译器：把源代码编译成目标文件和可执行程序。
- CMake：描述项目结构、查找依赖并生成构建文件。
- OpenCV：提供头文件、库文件和算法实现。

VS Code 本身不是编译器。必须安装 GCC/Clang/MSVC，并配置 OpenCV 开发包。

## 3.2 项目结构

```text
opencv_cpp_quickstart/
├── CMakeLists.txt
├── README.md
├── docs/
├── src/
├── assets/
└── output/
```

根目录 `CMakeLists.txt` 会为每个示例创建独立可执行目标。这样学生可以单独运行某一课，不必把所有代码写进一个 main 函数。

## 3.3 CMake 构建

Linux：

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

Windows（Visual Studio）：

```powershell
cmake -S . -B build -A x64 -DOpenCV_DIR="C:/opencv/build"
cmake --build build --config Release
```

如果无法找到 OpenCV，`OpenCV_DIR` 应指向包含 `OpenCVConfig.cmake` 的目录。具体安装目录会因安装方式而不同。

## 3.4 第一个程序

见 `src/00_first_program.cpp`。它创建 Mat、绘制文字、保存图像并显示窗口。

编译运行成功，说明编译器、CMake、OpenCV 头文件和库链接基本配置正确。

# 第四章 core：Mat 与图像内存

## 4.1 图像为什么能用矩阵表示

数字图像由像素组成。灰度图的每个像素通常是一个数值；彩色图的每个像素由多个通道组成。例如常见的 8 位三通道图像使用 B、G、R 三个分量。

Mat 是 OpenCV 的通用多维数组容器。它不只表示图像，也可以表示普通矩阵、张量或其他数值数据。

## 4.2 创建 Mat

```cpp
cv::Mat gray(480, 640, CV_8UC1, cv::Scalar(0));
cv::Mat color(480, 640, CV_8UC3, cv::Scalar(0, 0, 0));
cv::Mat depth(480, 640, CV_32FC1, cv::Scalar(0));
```

构造参数通常包括行数、列数、类型和初始值。

`CV_8UC3` 可拆成：

- `8U`：每个通道使用 8 位无符号整数。
- `C3`：三个通道。

一个像素占用 `elemSize() = 3` 字节；单通道元素占用 `elemSize1() = 1` 字节。

## 4.3 Mat 常用属性

| 属性/方法 | 含义 |
|---|---|
| `rows` | 行数，通常对应图像高度 |
| `cols` | 列数，通常对应图像宽度 |
| `channels()` | 通道数 |
| `depth()` | 单通道数据深度 |
| `type()` | 数据深度与通道数组合形成的类型编码 |
| `elemSize()` | 一个矩阵元素/像素占用的字节数 |
| `elemSize1()` | 一个通道占用的字节数 |
| `data` | 数据起始地址 |
| `step[0]` | 相邻两行起始地址之间的字节跨度 |
| `total()` | 所有维度元素数量的乘积 |

注意 `rows` 是高度、`cols` 是宽度；访问二维 Mat 时通常写作 `at(y, x)`，先行后列。

## 4.4 Mat 的头部与像素数据

可把 Mat 理解为“描述数据的头部 + 实际像素内存”。头部保存尺寸、类型、行跨度、数据指针和引用计数等信息。

普通 Mat 拷贝时，常见操作是浅拷贝：复制头部并共享像素数据。引用计数用于管理共享数据的生命周期。

```cpp
cv::Mat a = cv::imread("sample.jpg");
cv::Mat b = a;          // 浅拷贝，共享像素数据
cv::Mat c = a.clone();  // 深拷贝，创建独立像素数据
```

`copyTo()` 也可以将数据复制到另一个 Mat：

```cpp
cv::Mat d;
a.copyTo(d);
```

## 4.5 ROI 的实现与使用

ROI（Region of Interest）表示感兴趣区域。`Rect(x, y, width, height)` 的四个参数分别表示左上角横坐标、纵坐标、宽度和高度。

```cpp
cv::Rect rect(100, 50, 200, 120);
cv::Mat roi = image(rect);
```

通常，ROI 创建一个新的 Mat 头部，`data` 指向原图 ROI 左上角，`rows/cols` 变成 ROI 尺寸，而底层像素内存仍与原图共享。因此修改 ROI 会影响原图对应区域。需要独立数据时使用 `clone()`。

ROI 必须位于原图有效范围内。构造 Rect 时应保证 `x >= 0`、`y >= 0`、`x + width <= cols`、`y + height <= rows`。

## 4.6 step 与 isContinuous()

`step[0]` 是行跨度，单位是字节，不是列数。普通连续 `CV_8UC3` 图像的一行有效字节数是 `cols * 3`。ROI 如果只截取部分列，仍可能继承原图行跨度，所以 ROI 可能不连续。

`isContinuous()` 用于判断数据能否作为一段连续内存处理。不要仅根据 `cols` 推断连续性。

不连续 Mat 逐行访问的正确思路：

```cpp
for (int y = 0; y < roi.rows; ++y) {
    cv::Vec3b* row = roi.ptr<cv::Vec3b>(y);
    for (int x = 0; x < roi.cols; ++x) {
        // 访问 row[x]
    }
}
```

`ptr(y)` 会根据行跨度定位到当前行。若下游接口只接受连续数组，可以 `clone()`；或者按照每行有效字节数复制。不要对非连续 ROI 直接做一次覆盖整个 `total() * elemSize()` 的单次 `memcpy`。

## 4.7 像素访问方式

### at()

```cpp
cv::Vec3b pixel = image.at<cv::Vec3b>(y, x);
```

按行列坐标访问，直观，适合初学和非性能敏感的代码。Debug 构建中通常有相关断言；不要把断言当成所有构建模式下的内存安全保障。

### ptr()

```cpp
cv::Vec3b* row = image.ptr<cv::Vec3b>(y);
cv::Vec3b pixel = row[x];
```

先取得行指针，再访问当前行像素。指针访问需要程序员保证行列索引合法。`ptr<T>()` 中的 T 必须与 Mat 实际布局匹配，例如 `CV_8UC3` 对应 `Vec3b`，`CV_8UC1` 对应 `uchar`。

### 迭代器

```cpp
auto it = image.begin<cv::Vec3b>();
auto end = image.end<cv::Vec3b>();
for (; it != end; ++it) {
    // *it 是一个 Vec3b 像素
}
```

迭代器提供统一遍历接口，可正确处理非连续 Mat；它不等于自动进行所有边界检查的容器。

### 性能判断

通常优先使用已经实现好的 OpenCV 算子；必须自定义像素逻辑时，可从 `ptr()` 行优先循环开始。`at()`、`ptr()`、迭代器之间没有适用于所有 CPU 和构建配置的固定倍数关系。性能测试应在 Release 模式、相同数据和相同算法下进行。

**配套代码：** `src/02_mat_roi.cpp`、`src/03_pixel_access.cpp`。  
**练习：** 打开 `src/02_mat_roi.cpp` 和 `src/03_pixel_access.cpp`，按对应手册章节的步骤自己实现，并对照验收标准检查。

# 第五章 imgcodecs 与 highgui：图像输入输出

## 5.1 imread()

```cpp
cv::Mat image = cv::imread("assets/sample.jpg", cv::IMREAD_COLOR);
if (image.empty()) {
    // 路径错误、文件不存在或解码失败等
}
```

常用读取标志：

- `IMREAD_COLOR`：读取为常见三通道彩色图。
- `IMREAD_GRAYSCALE`：读取为灰度图。
- `IMREAD_UNCHANGED`：尽可能保留原始通道和深度信息。

OpenCV 常见彩色图像默认使用 BGR 通道顺序，而不是 RGB。

## 5.2 imwrite()

```cpp
bool ok = cv::imwrite("output/result.png", image);
```

返回值表示是否成功写入。编码格式通常由文件扩展名决定，并受 OpenCV 构建中可用编码器影响。

JPG 常用于照片压缩，属于有损格式；PNG 通常无损并支持透明通道；BMP 通常文件较大。

## 5.3 imshow() 与 waitKey()

```cpp
cv::imshow("Preview", image);
int key = cv::waitKey(0);
cv::destroyAllWindows();
```

`imshow()` 显示或更新窗口；`waitKey()` 等待键盘事件并处理 HighGUI 窗口事件；`destroyAllWindows()` 关闭 OpenCV 创建的窗口。

实时视频循环一般用 `waitKey(1)` 一类短等待；静态图片查看通常用 `waitKey(0)` 等待按键。

HighGUI 是轻量显示/交互模块，不是完整 GUI 框架。

**配套代码：** `src/01_image_io.cpp`。运行时可以不传参数，程序会生成演示图片；也可以传入图片路径。

# 第六章 imgproc：常见图像处理

## 6.1 颜色空间转换

OpenCV 常用彩色图像是 BGR 顺序。灰度图只有一个亮度通道；HSV 将颜色表示为色相、饱和度和明度，常用于基于颜色范围的筛选。

```cpp
cv::cvtColor(bgr, gray, cv::COLOR_BGR2GRAY);
cv::cvtColor(bgr, hsv, cv::COLOR_BGR2HSV);
```

灰度化常用于降低处理维度；HSV 常用于颜色分割。颜色空间选择应由任务决定。

## 6.2 resize：图像缩放

```cpp
cv::resize(src, dst, cv::Size(640, 480), 0, 0, cv::INTER_LINEAR);
```

插值方式影响缩放质量与速度：最近邻简单；双线性常用；双三次计算更多；面积插值通常适合缩小图像。缩放会改变像素采样，不只是修改 Mat 的 rows/cols。

## 6.3 滤波与去噪

- `blur()`：邻域均值滤波，简单但可能模糊边缘。
- `GaussianBlur()`：高斯加权平滑，常用于边缘检测前抑制噪声。
- `medianBlur()`：邻域中值滤波，对椒盐噪声常有较好效果。
- `bilateralFilter()`：考虑空间距离与像素值差异，能够在一定程度上保留边缘，但计算开销通常更高。

例如：

```cpp
cv::GaussianBlur(src, dst, cv::Size(5, 5), 1.2);
```

核大小通常使用正奇数，sigma 控制高斯分布尺度。

## 6.4 threshold：二值化

固定阈值：

```cpp
cv::threshold(gray, binary, 127, 255, cv::THRESH_BINARY);
```

将像素按阈值映射到两个主要取值。自适应阈值根据局部邻域计算阈值，适合部分光照不均匀图像：

```cpp
cv::adaptiveThreshold(gray, binary, 255,
    cv::ADAPTIVE_THRESH_GAUSSIAN_C,
    cv::THRESH_BINARY, 11, 2);
```

阈值和窗口大小需要根据图像内容调整。

## 6.5 Canny：边缘检测

```cpp
cv::Canny(gray, edges, 50, 150);
```

Canny 经典流程包括平滑、梯度计算、非极大值抑制、双阈值筛选和边缘连接。低阈值和高阈值控制边缘候选与强边缘判定。

## 6.6 形态学处理

形态学通常针对二值图像或灰度图像使用结构元素进行局部形状操作。

- 腐蚀 `erode()`：通常使白色前景区域收缩。
- 膨胀 `dilate()`：通常使白色前景区域扩张。
- 开运算：先腐蚀再膨胀，常用于去除小前景噪声。
- 闭运算：先膨胀再腐蚀，常用于连接小间隙或填补小孔洞。

```cpp
cv::Mat kernel = cv::getStructuringElement(
    cv::MORPH_RECT, cv::Size(3, 3));
cv::morphologyEx(binary, result, cv::MORPH_OPEN, kernel);
```

## 6.7 轮廓

`findContours()` 从二值图像中提取轮廓，`drawContours()` 绘制轮廓。可以结合 `contourArea()`、`boundingRect()`、`minAreaRect()` 等 API 分析目标的面积和几何范围。

输入通常是单通道二值图像，需先确认前景与背景的取值定义。

**配套代码：** `src/04_imgproc_pipeline.cpp`。程序默认生成图像，也接受一张用户图片作为参数，并把中间结果保存到 `output/`。

# 第七章 videoio 与 video：视频采集和分析

## 7.1 图像与视频帧

视频可以看成带有时间顺序的图像帧序列。视频处理除了像素，还涉及帧率、帧时间、编码、缓存和帧间运动。

## 7.2 VideoCapture

```cpp
cv::VideoCapture cap(0);          // 尝试打开默认摄像头
// cv::VideoCapture cap("clip.mp4"); // 也可以打开视频文件
if (!cap.isOpened()) {
    // 打开失败
}
cv::Mat frame;
while (cap.read(frame)) {
    if (frame.empty()) continue;
    // 处理当前帧
}
cap.release();
```

重要接口：

- `isOpened()`：检查视频源是否成功打开。
- `read()`：读取下一帧。
- `get()`：查询部分视频属性。
- `set()`：尝试设置部分视频属性。
- `release()`：释放资源。

摄像头驱动和视频后端可能不支持某些属性设置，因此 `set()` 不保证一定生效。

## 7.3 VideoWriter

`VideoWriter` 将连续图像帧编码写入视频文件。创建时要指定输出路径、编码器 FourCC、目标 FPS 和帧尺寸。写入帧的尺寸和格式应与创建时约定一致，且系统需要有可用编码器。

## 7.4 video 模块：运动分析

`videoio` 负责输入输出，`video` 负责分析帧间信息。

**光流：** 估计相邻帧像素或特征点的位移。稀疏光流常跟踪少量特征点，稠密光流估计更广泛区域的运动。

- `calcOpticalFlowPyrLK()`：常用于稀疏 Lucas–Kanade 光流。
- `calcOpticalFlowFarneback()`：常用于稠密 Farneback 光流。

**背景建模：** 根据时间序列估计背景并提取前景。MOG2、KNN 背景减除器适合固定摄像头场景，但对摄像机移动、光照变化和动态背景敏感。

**状态估计：** `KalmanFilter` 可以结合运动模型与观测更新估计目标状态。

**配套代码：** `src/05_videoio_camera.cpp`、`src/06_video_mog2.cpp`。

# 第八章 features2d 与 calib3d：传统视觉基础

## 8.1 features2d：特征检测与匹配

特征点是图像中具有可辨识局部结构的位置，例如角点或纹理变化明显的位置。特征检测器找到关键点，描述子把关键点邻域编码为向量，匹配器比较不同图像的描述子。

ORB 是常见的高效特征方法。它通常产生二进制描述子，常配合 Hamming 距离：

```cpp
cv::Ptr<cv::ORB> orb = cv::ORB::create(1000);
orb->detectAndCompute(image, cv::noArray(), keypoints, descriptors);
cv::BFMatcher matcher(cv::NORM_HAMMING);
```

SIFT 是经典的尺度不变特征方法，适用于需要一定尺度和旋转鲁棒性的局部特征匹配任务。实际速度和匹配质量取决于图像与参数。

典型流程：检测关键点 -> 计算描述子 -> 描述子匹配 -> 筛选匹配 -> 可视化或几何验证。

**配套代码：** `src/07_features2d_orb.cpp`。程序生成两张平移后的纹理图，避免学生没有数据集无法运行。

## 8.2 calib3d：相机标定与三维几何

相机将三维世界投影为二维图像。理解相机几何，需要区分内参、外参和畸变。

**内参：** 描述相机成像模型中的焦距、主点等参数。常见内参矩阵形式：

```text
[ fx   0  cx ]
[  0  fy  cy ]
[  0   0   1 ]
```

**外参：** 描述世界坐标系与相机坐标系之间的旋转和平移。

**畸变参数：** 描述镜头带来的径向、切向等畸变影响。

常见 API：

- `calibrateCamera()`：根据标定板观测估计相机参数。
- `undistort()`：对图像进行畸变校正。
- `solvePnP()`：根据 3D 点与 2D 图像点的对应关系估计物体相对相机的位姿。
- `projectPoints()`：把三维点投影到图像平面。

`solvePnP()` 不是完整的相机标定。真实标定需要拍摄多张不同姿态的已知标定图案，并使用角点观测估计内参和畸变。

**配套代码：** `src/08_calib3d_pnp.cpp`。代码用已知参数合成 2D 投影，再反求位姿，目的是理解数据关系，不替代真实标定。

# 第九章 dnn：深度学习模型推理

## 9.1 DNN 的职责

OpenCV DNN 主要提供模型导入和前向推理能力，通常不承担常规训练流程。训练一般在 PyTorch、TensorFlow 等框架中完成，再导出为 ONNX 等格式用于部署。

## 9.2 基本推理流程

```text
训练模型 -> 导出 ONNX -> 加载网络 -> 输入预处理
-> setInput -> forward -> 输出解码/后处理
```

关键接口：

- `readNetFromONNX()`：加载 ONNX 模型。
- `blobFromImage()`：把图像变为神经网络输入张量。
- `setInput()`：设置网络输入。
- `forward()`：执行前向推理。

## 9.3 预处理为什么重要

模型可能要求特定输入尺寸、BGR/RGB 顺序、缩放系数、均值、标准差、裁剪策略和数据布局。`blobFromImage()` 只是提供通用变换接口，参数必须符合模型定义。

不同检测模型的输出张量形状和含义不同，因此后处理不能只靠一段通用代码完成。必须查看模型导出说明，理解类别分数、置信度、边界框编码和 NMS 要求。

## 9.4 运行示例

`src/09_dnn_onnx.cpp` 需要用户自行准备 ONNX 模型和输入图片：

```bash
./build/09_dnn_onnx assets/model.onnx assets/sample.jpg
```

课程不附带模型文件，避免模型体积、许可和输入约定不一致问题。

# 第十章 性能与工程注意事项

## 10.1 优先使用内置算子

如果操作可以用 `cvtColor()`、`resize()`、`GaussianBlur()`、`threshold()`、`convertTo()` 等现有函数完成，通常先使用内置实现。部分实现可能利用 SIMD、并行后端或针对数据类型的优化，但具体优化取决于构建配置和运行环境。

## 10.2 手写循环时的原则

- 尽量按行遍历，利用空间局部性。
- 用 `ptr(y)` 获取当前行指针，注意索引边界和类型匹配。
- 不要假设 ROI 连续。
- 避免在循环中重复创建临时 Mat 或做不必要的数据复制。
- 用 Release 版本和真实数据测量，而不是凭 API 名称推断速度。

## 10.3 并行化

`parallel_for_()` 可以把范围拆成多个任务。自定义并行时，需保证线程写入区域不重叠，避免数据竞争；任务规模过小会让调度成本抵消并行收益。还需避免外层并行与内部并行叠加导致线程过度订阅。

## 10.4 Mat、UMat 与 OpenCL

`Mat` 主要面向主机侧图像数据；`UMat` 是 OpenCV Transparent API 的数据管理接口，在 OpenCL 可用且对应算子支持时，可能选择 OpenCL 路径。UMat 不等同于“数据必然在 GPU 显存中”，也不保证每个函数都在 GPU 上执行。

GPU 路径可能适合较大的、并行度高且能够连续留在设备侧的处理链。小图像、简单算子、频繁主机/设备转换和同步可能导致负优化。应测量完整链路，包括上传、计算、同步和下载。

# 第十一章 综合项目：实时图像处理工具

目标是用摄像头输入，实时切换不同图像处理方式，并保存结果。

**功能键：**

- `1`：原图。
- `2`：灰度图。
- `3`：高斯滤波。
- `4`：Canny 边缘。
- `S`：保存当前处理结果。
- `ESC`：退出。

代码见 `src/10_realtime_tool.cpp`。该程序将 `videoio`、`core`、`imgproc`、`imgcodecs` 和 `highgui` 组合起来。

### 程序主循环

```text
打开摄像头
    ↓
读取一帧 Mat
    ↓
根据模式选择图像处理操作
    ↓
显示原图和处理结果
    ↓
处理键盘输入
    ↓
继续读取下一帧，或退出
```

建议先逐行阅读代码，再尝试增加 HSV 颜色筛选、形态学清理、轮廓绘制和 FPS 显示。

# 第十二章 学习验收

完成课程后，学生应能：

1. 说明 OpenCV 的定位以及与 OpenGL、CUDA、PyTorch 的区别。
2. 说明 `core`、`imgproc`、`imgcodecs`、`highgui`、`videoio`、`video`、`features2d`、`calib3d`、`dnn` 的职责。
3. 创建 Mat，读取图像并解释 rows、cols、channels、type、step。
4. 区分浅拷贝、clone、ROI 与独立数据。
5. 使用 at、ptr 或迭代器访问像素。
6. 完成灰度化、滤波、阈值、Canny 和形态学处理。
7. 使用 VideoCapture 获取视频帧。
8. 解释视频采集与视频分析的区别。
9. 理解 ORB 特征匹配、solvePnP 位姿估计和 DNN 推理的基本流程。
10. 独立完成实时图像处理工具。

所有实现步骤与验收标准见 `动手学OpenCV_自学手册.md` 的各课和附录 D。

# 参考资料

- OpenCV 4.12 官方文档与模块索引：<https://docs.opencv.org/4.12.0/>
- OpenCV 官方代码仓库：<https://github.com/opencv/opencv>
- OpenCV 扩展模块仓库：<https://github.com/opencv/opencv_contrib>
