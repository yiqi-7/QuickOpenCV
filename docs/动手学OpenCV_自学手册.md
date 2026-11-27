# 动手学 OpenCV（C++）快速上手

> 这份手册面向"想尽快跑起来、并且看懂每行代码在干什么"的同学。
> 每一课都是：**完整源码 → 逐段讲解 → 运行后该看到什么 → 易错点**。
> 下面的代码是每课的**完整参考实现**，包含行内注释，可以直接编译运行；`src/` 中保留的是你要自己完成的练习文件。建议先自己敲一遍，再回来看参考实现。

---

## 0. 快速开始

### 0.1 环境（本机已装好，无需再配置）

| 组件 | 位置 / 版本 |
|---|---|
| OpenCV | 4.13.0，**静态库**（运行时不需要 OpenCV DLL） |
| 头文件 | `E:\opencv\Release\include` |
| 库文件 | `E:\opencv\Release\lib`（Debug 用 `E:\opencv\Debug\lib`） |
| CMake 包配置 | `E:\opencv\cmake\opencv4`（供 `find_package(OpenCV 4)` 使用） |
| 编译器 | MSVC 19.29（Visual Studio 2019 Community） |
| CMake | 3.31.12（`E:\CMake\cmake-3.31.12-windows-x86_64\bin`） |

### 0.2 编译：三种方式，任选一种

**方式 A（推荐，最省事）**——用项目自带的一键脚本：

```powershell
cd F:\CodingFile\opencv_cpp_quickstart
.\build.ps1                     # 编译全部
.\build.ps1 01_image_io         # 只编译第 2 课
.\build.ps1 01_image_io -Run    # 编译并运行
.\build.ps1 -List               # 看有哪些目标
```

脚本会自动完成配置，新增/删除 `src/*.cpp` 后也不需要手动重新配置。

**方式 B**——原生命令（三行）：

```powershell
cd F:\CodingFile\opencv_cpp_quickstart

# 1) 配置（只需做一次）
cmake -S . -B build -G "Visual Studio 16 2019" -A x64 -DOpenCV_DIR="E:/opencv/cmake/opencv4"

# 2) 编译
cmake --build build --config Release

# 3) 运行（工作目录必须是项目根，因为程序用 output/ 相对路径）
.\build\Release\00_first_program.exe
```

**方式 C**——用 CMake 预设（等价于方式 B 的配置步骤）：

```powershell
cmake --preset vs2019
cmake --build --preset release
```

只编译某一课：

```powershell
cmake --build build --config Release --target 07_features2d_orb
```

> 三个易错点：目标名用**下划线**（`07_features2d_orb`）；选项是 `--build` / `--config`（别拼错）；
> 新增 `src/*.cpp` 后要单独编新目标时，必须先重跑一次配置（方式 A 会自动处理）。

### 0.3 目录分工

| 目录 | 作用 |
|---|---|
| `docs/` | 本手册（每课含**完整源码 + 逐段讲解**） |
| `src/` | **你要写的代码**：11 个空壳文件，只有目标、API 提示、TODO 和验收标准 |
| `assets/` | 已下载的素材图片 |
| `output/` | 程序运行产生的图片 |

> 每课读完讲解后，就打开 `src/` 里同编号的空壳文件，把代码自己写一遍。

### 0.4 三个必须知道的坑

1. **源码是 UTF-8**。简体中文 Windows 上 MSVC 默认按 GBK 解析源文件，中文注释会吞掉换行，
   报一堆 `C2065 未声明的标识符`。`CMakeLists.txt` 里已加 `/utf-8`。
2. **`imshow` + `waitKey(0)` 会阻塞**，窗口里按任意键才继续，这是正常的。
3. **路径是相对工作目录的**：`assets/sample.jpg` 不是相对 exe 所在目录。

---

## 1. 第一个程序：创建 Mat、画字、保存、显示

**文件**：`src/00_first_program.cpp`

### 1.1 完整源码

```cpp
// OpenCV 核心模块：Mat、Scalar、Point、Vec、RNG 等基础类型。
#include <opencv2/core.hpp>
// 图像编解码模块：提供 imread 和 imwrite。
#include <opencv2/imgcodecs.hpp>
// 图像处理模块：绘图、颜色转换、滤波、阈值、边缘等 API。
#include <opencv2/imgproc.hpp>
// HighGUI 模块：提供 imshow、waitKey 和窗口管理。
#include <opencv2/highgui.hpp>

#include <iostream>

int main()
{
    // CV_VERSION 是 OpenCV 提供的编译期版本字符串宏。
    std::cout << "OpenCV version: " << CV_VERSION << '\n';

    // 创建一张 640x360、三通道 8 位图像，初始颜色为深蓝色（BGR）。
    // Mat 参数顺序是 rows、cols、type、初始值；Scalar 按 BGR 排列。
    cv::Mat canvas(360, 640, CV_8UC3, cv::Scalar(80, 40, 20));

    // org 是文字左下角坐标；后面的参数依次控制字体、大小、颜色和线宽。
    cv::putText(canvas, "Hello OpenCV", cv::Point(90, 180),
                cv::FONT_HERSHEY_SIMPLEX, 1.5,
                cv::Scalar(0, 255, 255), 3, cv::LINE_AA);

    // imwrite 按扩展名选择编码器，返回 false 时表示路径或编码失败。
    if (!cv::imwrite("output/00_hello_opencv.png", canvas)) {
        std::cerr << "Failed to save output/00_hello_opencv.png\n";
        return 1;
    }

    std::cout << "Image saved: output/00_hello_opencv.png\n";
    std::cout << "Size: " << canvas.cols << " x " << canvas.rows
              << ", channels: " << canvas.channels() << '\n';

    // 无桌面环境或远程终端中可注释下面两行，仍然可以完成保存测试。
    // imshow 显示 Mat；必须配合 waitKey 处理窗口消息。
    cv::imshow("First OpenCV Program", canvas);
    // 0 表示无限等待按键；实时程序则应使用很小的毫秒数。
    cv::waitKey(0);
    return 0;
}
```

### 1.2 逐段讲解

| 代码 | 含义 |
|---|---|
| 4 个 `#include` | `core` 提供 `Mat`/`Scalar`/`Point`；`imgcodecs` 提供读写；`imgproc` 提供 `putText`；`highgui` 提供窗口 |
| `CV_VERSION` | 编译期宏，值是 `"4.13.0"`，直接当字符串输出 |
| `cv::Mat canvas(360, 640, CV_8UC3, Scalar(80,40,20))` | 构造图像：**第一个参数是行数（高 360）**，第二个是列数（宽 640）；`CV_8UC3` = 8 位无符号 + 3 通道；`Scalar` 是初始填充色，顺序 **B,G,R**，所以 `(80,40,20)` 是偏蓝的深色 |
| `putText(canvas, "Hello OpenCV", Point(90,180), ...)` | 在图上写字：`Point(90,180)` 是文字**左下角**的像素坐标；`FONT_HERSHEY_SIMPLEX` 是字体；`1.5` 是缩放；`Scalar(0,255,255)` 是黄色（B=0,G=255,R=255）；`3` 是线宽；`LINE_AA` 是抗锯齿 |
| `if (!cv::imwrite(...))` | `imwrite` 返回 `bool`。返回 false 的典型原因是**目录不存在**或扩展名不支持。一定要检查 |
| `canvas.cols / rows / channels()` | 宽 / 高 / 通道数 |
| `imshow` + `waitKey(0)` | 创建窗口并显示；`waitKey(0)` 无限等待键盘，同时驱动窗口消息循环。**没有 waitKey，窗口会一闪而过** |

### 1.3 运行结果

```text
OpenCV version: 4.13.0
Image saved: output/00_hello_opencv.png
Size: 640 x 360, channels: 3
```

同时弹出窗口，按任意键退出，`output/00_hello_opencv.png` 生成。

### 1.4 易错点

- 把 `Mat(360, 640, ...)` 写成 `Mat(640, 360, ...)`：图变竖图。
- 文字坐标给 `(0,0)`：文字画在画布上方之外，看不见。
- 直接跑 `.\build\Release\00_first_program.exe` 但当前目录不在项目根：`output/` 不存在，保存失败。

---

## 2. 图像读写：imread / imwrite / empty

**文件**：`src/01_image_io.cpp`

### 2.1 完整源码

```cpp
// OpenCV 核心模块：Mat、Scalar、Point、Vec、RNG 等基础类型。
#include <opencv2/core.hpp>
// 图像编解码模块：提供 imread 和 imwrite。
#include <opencv2/imgcodecs.hpp>
// HighGUI 模块：提供 imshow、waitKey 和窗口管理。
#include <opencv2/highgui.hpp>
// 图像处理模块：绘图、颜色转换、滤波、阈值、边缘等 API。
#include <opencv2/imgproc.hpp>

// C++17 文件系统：用于创建 output 目录。
#include <filesystem>
#include <iostream>
#include <string>

// 给文件系统命名空间取短别名，减少后续代码长度。
namespace fs = std::filesystem;

// 单独封装造图逻辑，让 main 只负责流程控制。
static cv::Mat makeDemoImage()
{
    // 创建 480 行、640 列的三通道 8 位图，并填充浅灰色。
    cv::Mat image(480, 640, CV_8UC3, cv::Scalar(235, 235, 235));
    cv::rectangle(image, cv::Rect(70, 80, 210, 260), cv::Scalar(40, 80, 220), 5);
    cv::circle(image, cv::Point(430, 190), 90, cv::Scalar(40, 180, 60), -1);
    // line 的两个 Point 分别是起点和终点。
    cv::line(image, cv::Point(60, 410), cv::Point(580, 410),
             cv::Scalar(200, 70, 30), 8);
    // 在生成图上写字，坐标仍然是文字左下角。
    cv::putText(image, "OpenCV image I/O", cv::Point(100, 455),
                cv::FONT_HERSHEY_SIMPLEX, 0.9,
                cv::Scalar(20, 20, 20), 2, cv::LINE_AA);
    return image;
}

// argc/argv 让程序可以从命令行接收输入图片路径。
int main(int argc, char** argv)
{
    // imwrite 不会自动创建目录，因此先确保输出目录存在。
    fs::create_directories("output");

    // 参数 1 可指定图片路径；未提供时自动生成演示图片。
    cv::Mat image;
        // argv[0] 是程序名，argv[1] 是用户传入的第一个参数。
    if (argc >= 2) {
        // IMREAD_COLOR 强制以 BGR 三通道彩色模式读取。
        image = cv::imread(argv[1], cv::IMREAD_COLOR);
            // 读取失败返回空 Mat；必须在后续访问前检查。
        if (image.empty()) {
            std::cerr << "Cannot read image: " << argv[1] << '\n';
            return 1;
        }
    } else {
        image = makeDemoImage();
        cv::imwrite("output/01_generated_input.png", image);
    }

    // cols/rows 是宽高，channels 是通道数，depth/type 描述像素格式。
    std::cout << "Width: " << image.cols << '\n'
              << "Height: " << image.rows << '\n'
              << "Channels: " << image.channels() << '\n'
              << "Depth: " << image.depth() << '\n'
              << "Type code: " << image.type() << '\n';

    // 用 string 保存路径，便于传给 imwrite 和输出错误信息。
    const std::string outputPath = "output/01_image_copy.png";
    if (!cv::imwrite(outputPath, image)) {
        std::cerr << "Failed to write: " << outputPath << '\n';
        return 1;
    }

    cv::imshow("Image I/O - input", image);
    std::cout << "Press any key in the image window to exit.\n";
    // 0 表示无限等待按键；实时程序则应使用很小的毫秒数。
    cv::waitKey(0);
    return 0;
}
```

### 2.2 逐段讲解

| 代码 | 含义 |
|---|---|
| `namespace fs = std::filesystem;` | C++17 文件系统库，用来建目录 |
| `makeDemoImage()` | 返回 `cv::Mat`：先铺白底，再画矩形、实心圆、粗线、文字。`rectangle` 的线宽 5 表示空心；`circle` 的线宽 `-1` 表示**填充** |
| `fs::create_directories("output")` | 保证 `output/` 存在，否则 `imwrite` 会失败 |
| `argc >= 2` | 命令行参数个数。`argv[0]` 是程序名，`argv[1]` 是第一个参数 |
| `cv::imread(path, cv::IMREAD_COLOR)` | 读成 3 通道彩色（`IMREAD_GRAYSCALE` 读灰度，`IMREAD_UNCHANGED` 保留原样） |
| `image.empty()` | **读失败不会抛异常**，只返回空 Mat。不检查后面会崩 |
| `image.cols / rows` | 宽 / 高 |
| `channels()` | 通道数，彩色 3、灰度 1 |
| `depth()` | 单通道数据类型，`CV_8U` 的值是 0 |
| `type()` | 类型编码，`CV_8UC3` = 16，公式 `depth + 8*(channels-1)` |
| `imwrite` 返回值 | 保存是否成功 |

### 2.3 运行结果

```powershell
.\build\Release\01_image_io.exe                    # 用程序生成的图
.\build\Release\01_image_io.exe assets\sample.jpg  # 用下载的真实照片
```

```text
Width: 512
Height: 480
Channels: 3
Depth: 0
Type code: 16
```

### 2.4 易错点

- 忘了 `create_directories`：`imwrite` 静默失败，你以为图丢了。
- 用 `type()` 打印出数字就懵了：记住 `16 = CV_8UC3`。
- 传了不存在的路径：必须靠 `empty()` 拦住。

---

## 3. Mat 的内存模型：ROI、浅拷贝、clone

**文件**：`src/02_mat_roi.cpp`

### 3.1 完整源码

```cpp
// OpenCV 核心模块：Mat、Scalar、Point、Vec、RNG 等基础类型。
#include <opencv2/core.hpp>
// 图像处理模块：绘图、颜色转换、滤波、阈值、边缘等 API。
#include <opencv2/imgproc.hpp>
// 图像编解码模块：提供 imread 和 imwrite。
#include <opencv2/imgcodecs.hpp>
// HighGUI 模块：提供 imshow、waitKey 和窗口管理。
#include <opencv2/highgui.hpp>

// C++17 文件系统：用于创建 output 目录。
#include <filesystem>
#include <iostream>

int main()
{
    // 运行前创建输出目录，避免保存图片失败。
    std::filesystem::create_directories("output");

    // 创建连续存储的 480x640、CV_8UC3 图像。
    // 这张原图连续存储，后面用 step 和 isContinuous 观察内存布局。
    cv::Mat image(480, 640, CV_8UC3, cv::Scalar(30, 30, 30));
    cv::rectangle(image, cv::Rect(0, 0, 640, 480), cv::Scalar(70, 70, 70), 2);

    std::cout << "Original: rows=" << image.rows
              << ", cols=" << image.cols
              << ", elemSize=" << image.elemSize()
              << ", step[0]=" << image.step[0]
              << ", continuous=" << image.isContinuous() << '\n';

    // ROI: x=100, y=50, width=200, height=120。
    // ROI 左上角为 (100,50)，大小为 200x120。
    const cv::Rect roiRect(100, 50, 200, 120);
    cv::Mat roi = image(roiRect);          // 浅拷贝：共享 image 的像素内存
    cv::Mat roiCopy = image(roiRect).clone(); // 深拷贝：独立像素内存

    std::cout << "ROI: rows=" << roi.rows
              << ", cols=" << roi.cols
              << ", elemSize=" << roi.elemSize()
              << ", step[0]=" << roi.step[0]
              << ", continuous=" << roi.isContinuous() << '\n';

    // 修改浅拷贝 ROI，会反映到原图。
    // 共享 ROI 改为红色，原图对应区域也会同步变化。
    roi.setTo(cv::Scalar(0, 0, 255));

    // 修改深拷贝副本，不会改变原图。
    // 深拷贝副本改为绿色，验证它和原图已经分离。
    roiCopy.setTo(cv::Scalar(0, 255, 0));

    cv::rectangle(image, roiRect, cv::Scalar(255, 255, 255), 2);
    // 保存共享 ROI 修改后的原图。
    cv::imwrite("output/02_mat_roi_shared.png", image);
    // 保存独立副本；它的尺寸仍然是 200x120。
    cv::imwrite("output/02_mat_roi_clone.png", roiCopy);

    cv::imshow("Original after ROI modification", image);
    cv::imshow("Independent clone", roiCopy);
    // 0 表示无限等待按键；实时程序则应使用很小的毫秒数。
    cv::waitKey(0);
    return 0;
}
```

### 3.2 逐段讲解

| 代码 | 含义 |
|---|---|
| `Mat(480, 640, CV_8UC3, Scalar(30,30,30))` | 高 480、宽 640 的深灰图 |
| `elemSize()` | **一个像素**占多少字节；`CV_8UC3` = 3 |
| `step[0]` | 相邻两行起始地址之间的**字节**跨度。连续图 = `cols * elemSize() = 1920` |
| `isContinuous()` | 整块像素是否连续存放 |
| `cv::Rect(x, y, w, h)` | 左上角坐标 + 宽高 |
| `image(roiRect)` | 返回 ROI 的**新 Mat 头**：`data` 指向 ROI 左上角，尺寸变成 ROI 尺寸，但**像素内存和原图是同一块** |
| `roi.setTo(Scalar(0,0,255))` | 把 ROI 全刷成红色 → 原图那块也变红，证明共享内存 |
| `.clone()` | 深拷贝，分配新内存；`roiCopy.setTo(green)` 不影响原图 |
| `cv::rectangle(image, roiRect, white, 2)` | 在白框标出 ROI 位置，方便肉眼确认 |

### 3.3 关键结论（本课最重要）

```text
原图: step[0] = 1920 = 640*3, continuous = 1
ROI : step[0] = 1920（继承原图！不是 200*3=600）, continuous = 0
```

因为 ROI 只截取了每行中间 200 列，下一行开头并不紧跟在本行末尾，所以**不连续**。
这解释了为什么"不能对非连续 ROI 直接做一次 `memcpy(total()*elemSize())`"——
那样会把别的行的像素也拷进来。

### 3.4 运行结果

- `output/02_mat_roi_shared.png`：原图中间有一块红色矩形（外加白框）。
- `output/02_mat_roi_clone.png`：一张纯绿色的 200×120 小图。

### 3.5 易错点

- ROI 越界（`x+w > cols`）：Release 下不报错，读脏内存，结果诡异。
- 以为 `Mat roi = image(rect)` 是拷贝 → 改 ROI 把原图改了。

---

## 4. 三种像素遍历方式与性能

**文件**：`src/03_pixel_access.cpp`

### 4.1 完整源码

```cpp
// OpenCV 核心模块：Mat、Scalar、Point、Vec、RNG 等基础类型。
#include <opencv2/core.hpp>
// 图像处理模块：绘图、颜色转换、滤波、阈值、边缘等 API。
#include <opencv2/imgproc.hpp>
// HighGUI 模块：提供 imshow、waitKey 和窗口管理。
#include <opencv2/highgui.hpp>
// 图像编解码模块：提供 imread 和 imwrite。
#include <opencv2/imgcodecs.hpp>

#include <algorithm>
// C++ 标准计时库：steady_clock 适合测量持续时间。
#include <chrono>
// C++17 文件系统：用于创建 output 目录。
#include <filesystem>
#include <iostream>

static cv::Mat makeInput()
{
    // 创建 1280x720 的三通道噪声输入图。
    cv::Mat image(720, 1280, CV_8UC3);
    // 用均匀随机数填充所有通道，范围是 [0,180)。
    cv::randu(image, cv::Scalar::all(0), cv::Scalar::all(180));
    return image;
}

// at 版本直观但每次访问都要做行列定位，Debug 还可能有断言。
static void processWithAt(const cv::Mat& src, cv::Mat& dst)
{
    // 准备与源图尺寸和类型相同的目标；匹配时可复用已有内存。
    dst.create(src.size(), src.type());
    for (int y = 0; y < src.rows; ++y) {
        for (int x = 0; x < src.cols; ++x) {
            // at 的参数顺序是 y、x；Vec3b 表示 B、G、R 三个 uchar。
            const cv::Vec3b pixel = src.at<cv::Vec3b>(y, x);
            // 取目标像素的引用，直接写入目标图而不产生额外副本。
            cv::Vec3b& out = dst.at<cv::Vec3b>(y, x);
            for (int c = 0; c < 3; ++c) {
                // 乘法后夹到 0~255；直接转 uchar 可能发生回绕。
                out[c] = cv::saturate_cast<uchar>(pixel[c] * 1.2);
            }
        }
    }
}

// ptr 版本每行只定位一次，通常比逐像素 at 更快。
static void processWithPtr(const cv::Mat& src, cv::Mat& dst)
{
    // 准备与源图尺寸和类型相同的目标；匹配时可复用已有内存。
    dst.create(src.size(), src.type());
    for (int y = 0; y < src.rows; ++y) {
        // 得到当前行首地址；ptr 会按 step 正确处理非连续 ROI。
        const cv::Vec3b* srcRow = src.ptr<cv::Vec3b>(y);
        // 得到目标行首地址，后面用 dstRow[x] 访问像素。
        cv::Vec3b* dstRow = dst.ptr<cv::Vec3b>(y);
        for (int x = 0; x < src.cols; ++x) {
            for (int c = 0; c < 3; ++c) {
                dstRow[x][c] = cv::saturate_cast<uchar>(srcRow[x][c] * 1.2);
            }
        }
    }
}

// 迭代器把 Vec3b 作为元素遍历，接口统一但抽象层次更高。
static void processWithIterator(const cv::Mat& src, cv::Mat& dst)
{
    // 准备与源图尺寸和类型相同的目标；匹配时可复用已有内存。
    dst.create(src.size(), src.type());
    // begin/end 的模板参数决定每次迭代得到一个三通道像素。
    auto in = src.begin<cv::Vec3b>();
    auto inEnd = src.end<cv::Vec3b>();
    auto out = dst.begin<cv::Vec3b>();
    for (; in != inEnd; ++in, ++out) {
        for (int c = 0; c < 3; ++c) {
            (*out)[c] = cv::saturate_cast<uchar>((*in)[c] * 1.2);
        }
    }
}

int main()
{
    // 运行前创建输出目录，避免保存图片失败。
    std::filesystem::create_directories("output");
    const cv::Mat src = makeInput();
    cv::Mat dstAt, dstPtr, dstIterator, dstBuiltin;

    // 通用 lambda 接收名称和可调用对象，统一测量四种实现。
    const auto measure = [](const char* name, auto&& fn) {
        // steady_clock 不受系统时间校准影响，适合测耗时。
        const auto begin = std::chrono::steady_clock::now();
        fn();
        const auto end = std::chrono::steady_clock::now();
        const double ms = std::chrono::duration<double, std::milli>(end - begin).count();
        std::cout << name << ": " << ms << " ms (single run; not a rigorous benchmark)\n";
    };

    measure("at", [&] { processWithAt(src, dstAt); });
    measure("ptr", [&] { processWithPtr(src, dstPtr); });
    measure("iterator", [&] { processWithIterator(src, dstIterator); });
    // convertTo 在 OpenCV 内部遍历数据，通常能使用更充分的底层优化。
    measure("convertTo", [&] { src.convertTo(dstBuiltin, -1, 1.2, 0.0); });

    std::cout << "at == ptr: "
              << cv::norm(dstAt, dstPtr, cv::NORM_INF) << '\n';
    std::cout << "at == iterator: "
              << cv::norm(dstAt, dstIterator, cv::NORM_INF) << '\n';
    std::cout << "at == convertTo: "
              << cv::norm(dstAt, dstBuiltin, cv::NORM_INF) << '\n';

    cv::imwrite("output/03_pixel_ptr_result.png", dstPtr);
    cv::imshow("Pixel processing result", dstPtr);
    // 0 表示无限等待按键；实时程序则应使用很小的毫秒数。
    cv::waitKey(0);
    return 0;
}
```

### 4.2 逐段讲解

| 代码 | 含义 |
|---|---|
| `cv::randu(img, low, high)` | 用均匀随机数填充整张图，生成"噪声"输入 |
| `dst.create(src.size(), src.type())` | 分配目标 Mat；如果尺寸类型已一致则**不重新分配** |
| `src.at<cv::Vec3b>(y, x)` | 按行列访问，**先行后列**；`Vec3b` = 3 个 uchar（B,G,R）。Debug 构建带边界断言 |
| `src.ptr<cv::Vec3b>(y)` | 取第 y 行首地址，之后用 `row[x]` 访问；`ptr` 内部按 `step[0]` 定位，所以**对非连续 ROI 也正确** |
| `src.begin<Vec3b>() / end<Vec3b>()` | 迭代器遍历；遇到行尾会按 step 跳到下一行，天然支持非连续 |
| `cv::saturate_cast<uchar>(v)` | 把 `v` 四舍五入并夹到 0~255；直接用 `uchar` 强转会**回绕**（300 变 44） |
| `convertTo(dst, -1, 1.2, 0.0)` | 内置算子，计算 `dst = src*1.2 + 0`，`type=-1` 表示保持原类型。内部走 SIMD/多线程，通常最快 |
| `cv::norm(a, b, cv::NORM_INF)` | 两图差的最大绝对值，用来验证"三种写法结果一致" |
| `measure` lambda + `steady_clock` | 单次计时。只用来观察量级，不是严谨 benchmark |

### 4.3 运行结果（本机实测参考）

```text
at: ~5 ms        ptr: ~2 ms        iterator: ~2 ms        convertTo: ~0.5 ms
at == ptr: 0
at == iterator: 0
at == convertTo: 0
```

三个差值为 0，说明三种手写遍历与 `convertTo` 结果**完全一致**。

### 4.4 易错点

- 忘记 `create`：写越界崩溃。
- 用 `uchar` 直接存 `*1.2`：溢出回绕。
- 拿 Debug 版测速：`at` 会慢十几倍（断言开销），别用它下结论。

---

## 5. imgproc 处理流水线

**文件**：`src/04_imgproc_pipeline.cpp`

### 5.1 完整源码

```cpp
// OpenCV 核心模块：Mat、Scalar、Point、Vec、RNG 等基础类型。
#include <opencv2/core.hpp>
// 图像处理模块：绘图、颜色转换、滤波、阈值、边缘等 API。
#include <opencv2/imgproc.hpp>
// 图像编解码模块：提供 imread 和 imwrite。
#include <opencv2/imgcodecs.hpp>
// HighGUI 模块：提供 imshow、waitKey 和窗口管理。
#include <opencv2/highgui.hpp>

// C++17 文件系统：用于创建 output 目录。
#include <filesystem>
#include <iostream>

// 给文件系统命名空间取短别名，减少后续代码长度。
namespace fs = std::filesystem;

// 单独封装造图逻辑，让 main 只负责流程控制。
static cv::Mat makeDemoImage()
{
    // 创建 480 行、640 列的三通道 8 位图，并填充浅灰色。
    cv::Mat image(480, 640, CV_8UC3, cv::Scalar(235, 235, 235));
    cv::rectangle(image, cv::Rect(80, 70, 180, 250), cv::Scalar(30, 80, 220), -1);
    cv::circle(image, cv::Point(430, 170), 85, cv::Scalar(40, 180, 70), -1);
    cv::line(image, cv::Point(50, 390), cv::Point(590, 390),
             cv::Scalar(200, 60, 20), 12);
    cv::putText(image, "Image Processing", cv::Point(120, 455),
                cv::FONT_HERSHEY_SIMPLEX, 1.0,
                cv::Scalar(20, 20, 20), 2, cv::LINE_AA);
    return image;
}

// argc/argv 让程序可以从命令行接收输入图片路径。
int main(int argc, char** argv)
{
    // imwrite 不会自动创建目录，因此先确保输出目录存在。
    fs::create_directories("output");

    // 有参数读外部图片；没有参数就使用人工生成的演示图。
    cv::Mat src = (argc >= 2)
        ? cv::imread(argv[1], cv::IMREAD_COLOR)
        : makeDemoImage();

    if (src.empty()) {
        std::cerr << "Input image is empty. Check the path.\n";
        return 1;
    }

    cv::Mat gray, gaussian, median, binary, edges, morphology;
    // 先转灰度，因为后续很多传统算子要求单通道输入。
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    // 高斯平滑抑制噪声；核尺寸必须是正奇数。
    cv::GaussianBlur(gray, gaussian, cv::Size(5, 5), 1.2);
    // 中值滤波对椒盐噪声尤其有效，核大小也必须是奇数。
    cv::medianBlur(gray, median, 5);
    // 固定阈值：大于 127 的像素设为 255，否则设为 0。
    cv::threshold(gray, binary, 127, 255, cv::THRESH_BINARY);
    // 在平滑后的灰度图上做边缘检测，降低噪声造成的假边缘。
    cv::Canny(gaussian, edges, 50, 150);

    // 创建 3x3 矩形结构元素，供形态学操作使用。
    const cv::Mat kernel = cv::getStructuringElement(
        cv::MORPH_RECT, cv::Size(3, 3));
    // 开运算等于先腐蚀后膨胀，用于去掉小白色噪点。
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
    cv::imshow("Gaussian", gaussian);
    cv::imshow("Canny", edges);
    // 0 表示无限等待按键；实时程序则应使用很小的毫秒数。
    cv::waitKey(0);
    return 0;
}
```

### 5.2 逐段讲解

| 代码 | 含义 |
|---|---|
| `(argc >= 2) ? imread(...) : makeDemoImage()` | 三目运算符选输入源：有参数读文件，没参数自己画 |
| `cvtColor(src, gray, COLOR_BGR2GRAY)` | 三通道 → 单通道灰度。后续大部分算子要求单通道 |
| `GaussianBlur(gray, gaussian, Size(5,5), 1.2)` | 高斯平滑：核 5×5（必须是**正奇数**），`sigmaX=1.2` 控制平滑强度 |
| `medianBlur(gray, median, 5)` | 中值滤波：对**椒盐噪声**效果好，`ksize` 是单个奇数 |
| `threshold(gray, binary, 127, 255, THRESH_BINARY)` | 固定阈值二值化：>127 变 255，否则 0 |
| `Canny(gaussian, edges, 50, 150)` | 边缘检测，输入是**已平滑的灰度图**；50 低阈值、150 高阈值（比例 1:3） |
| `getStructuringElement(MORPH_RECT, Size(3,3))` | 3×3 矩形结构元素 |
| `morphologyEx(binary, morphology, MORPH_OPEN, kernel)` | 开运算 = 先腐蚀再膨胀，去掉小的白色噪点 |
| 7 次 `imwrite` | 把每一步的中间结果都存下来，方便对照 |

### 5.3 运行结果

`output/` 下生成 7 张图：`04_source`、`04_gray`、`04_gaussian`、`04_median`、`04_binary`、`04_canny`、`04_morphology_open`。

建议对比：
- `04_gaussian` vs `04_median`：高斯让边缘更"糊"，中值更能保住边缘、干掉孤立噪点。
- `04_canny`：只保留边缘，线条细。
- `04_morphology_open`：二值图里的小白点消失。

### 5.4 易错点

- 彩色图直接进 `threshold` / `Canny`：运行时断言失败。
- 核给 `Size(4,4)`：断言失败（必须是奇数）。
- 顺序颠倒：先 Canny 再滤波，边缘会被抹掉。

---

## 6. 摄像头与实时帧

**文件**：`src/05_videoio_camera.cpp`

### 6.1 完整源码

```cpp
// OpenCV 核心模块：Mat、Scalar、Point、Vec、RNG 等基础类型。
#include <opencv2/core.hpp>
// 视频输入输出模块：提供 VideoCapture。
#include <opencv2/videoio.hpp>
// 图像处理模块：绘图、颜色转换、滤波、阈值、边缘等 API。
#include <opencv2/imgproc.hpp>
// HighGUI 模块：提供 imshow、waitKey 和窗口管理。
#include <opencv2/highgui.hpp>
// 图像编解码模块：提供 imread 和 imwrite。
#include <opencv2/imgcodecs.hpp>

// C++17 文件系统：用于创建 output 目录。
#include <filesystem>
#include <iostream>
#include <string>

// argc/argv 让程序可以从命令行接收输入图片路径。
int main(int argc, char** argv)
{
    // 用户可以传入摄像头编号；缺省使用设备 0。
    const int cameraIndex = (argc >= 2) ? std::stoi(argv[1]) : 0;
    // 构造 VideoCapture 并尝试打开指定摄像头。
    cv::VideoCapture capture(cameraIndex);
    // 必须检查设备是否真正打开，否则 read 只能得到空帧。
    if (!capture.isOpened()) {
        std::cerr << "Cannot open camera index " << cameraIndex << '\n';
        return 1;
    }

    std::cout << "Camera opened. Press G to toggle grayscale, S to save, ESC to quit.\n";
    bool grayscale = false;
    cv::Mat frame, output;
    // 运行前创建输出目录，避免保存图片失败。
    std::filesystem::create_directories("output");

    // 实时循环持续读帧，直到读帧失败或用户按 ESC。
    while (true) {
        // read 写入下一帧；失败或空帧时立即停止处理。
        if (!capture.read(frame) || frame.empty()) {
            std::cerr << "Failed to read frame.\n";
            break;
        }

        if (grayscale) {
            cv::cvtColor(frame, output, cv::COLOR_BGR2GRAY);
        } else {
            // Mat 赋值是浅拷贝，只复制头并共享当前帧像素。
            output = frame;
        }

        cv::imshow("Camera input", frame);
        cv::imshow("Camera output", output);

        // 1ms 等待让窗口刷新，同时取得本帧键盘输入。
        const int key = cv::waitKey(1) & 0xFF;
        if (key == 27) {
            break;
        } else if (key == 'g' || key == 'G') {
            grayscale = !grayscale;
        } else if (key == 's' || key == 'S') {
            const std::string path = grayscale
                ? "output/05_camera_gray.png"
                : "output/05_camera_color.png";
            if (cv::imwrite(path, output)) {
                std::cout << "Saved: " << path << '\n';
            } else {
                std::cerr << "Save failed: " << path << '\n';
            }
        }
    }

    // 主动释放摄像头句柄，否则设备可能继续被占用。
    capture.release();
    cv::destroyAllWindows();
    return 0;
}
```

### 6.2 逐段讲解

| 代码 | 含义 |
|---|---|
| `std::stoi(argv[1])` | 字符串转 int，用作摄像头索引（0 = 默认摄像头） |
| `cv::VideoCapture capture(cameraIndex)` | 打开设备；也可以 `VideoCapture("video.mp4")` 打开文件 |
| `capture.isOpened()` | **必须检查**，否则后面读到空帧 |
| `capture.read(frame)` | 读一帧，返回 `bool`；还要再判 `frame.empty()` 双保险 |
| `output = frame;` | 浅拷贝：`output` 与 `frame` 共享像素。**这里安全**，因为下一帧 `read` 会写新数据，且我们只读 |
| `waitKey(1) & 0xFF` | 等 1ms 并取键码低 8 位（避免高位标志位干扰） |
| `key == 27` | ESC 的 ASCII 码 |
| `'g' / 'G'`、`'s' / 'S'` | 大小写都接受，用户体验好 |
| `capture.release()` + `destroyAllWindows()` | 释放摄像头、关闭窗口。不加的话摄像头可能被占用 |

### 6.3 运行结果

- 两个窗口实时刷新：左原图，右灰度（按 G 切换）。
- 按 S 保存 `output/05_camera_gray.png` 或 `05_camera_color.png`。
- 按 ESC 退出。

> 没有摄像头：把构造参数换成 `"assets/你的视频.mp4"`，循环代码一个字都不用改。

### 6.4 易错点

- 视频循环里用 `waitKey(0)`：每帧都要按键，画面变幻灯片。
- 不判空帧：拔掉摄像头后拿空 Mat 去 `cvtColor`，崩溃。
- 退出不 `release()`：下次打开设备可能失败。

---

## 7. MOG2 背景建模

**文件**：`src/06_video_mog2.cpp`

### 7.1 完整源码

```cpp
// OpenCV 核心模块：Mat、Scalar、Point、Vec、RNG 等基础类型。
#include <opencv2/core.hpp>
// 视频输入输出模块：提供 VideoCapture。
#include <opencv2/videoio.hpp>
// 视频分析模块：提供背景建模等接口。
#include <opencv2/video.hpp>
// 背景分割模块：提供 MOG2 工厂函数。
#include <opencv2/video/background_segm.hpp>
// HighGUI 模块：提供 imshow、waitKey 和窗口管理。
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>

// argc/argv 让程序可以从命令行接收输入图片路径。
int main(int argc, char** argv)
{
    // 默认使用摄像头；也可以传入视频文件路径。
    cv::VideoCapture capture;
        // argv[0] 是程序名，argv[1] 是用户传入的第一个参数。
    if (argc >= 2) {
        capture.open(argv[1]);
    } else {
        capture.open(0);
    }

    // 必须检查设备是否真正打开，否则 read 只能得到空帧。
    if (!capture.isOpened()) {
        std::cerr << "Cannot open video source.\n";
        return 1;
    }

    // cv::Ptr 是 OpenCV 的引用计数智能指针，管理算法对象生命周期。
    cv::Ptr<cv::BackgroundSubtractor> mog2 =
        cv::createBackgroundSubtractorMOG2();

    cv::Mat frame, foreground;
    std::cout << "MOG2 background subtraction. Press ESC to quit.\n";

    while (capture.read(frame)) {
        if (frame.empty()) {
            break;
        }

        // 更新背景模型，并生成单通道前景掩码。
        mog2->apply(frame, foreground);
        cv::imshow("Input video", frame);
        cv::imshow("MOG2 foreground mask", foreground);

        if ((cv::waitKey(1) & 0xFF) == 27) {
            break;
        }
    }

    // 主动释放摄像头句柄，否则设备可能继续被占用。
    capture.release();
    cv::destroyAllWindows();
    return 0;
}
```

### 7.2 逐段讲解

| 代码 | 含义 |
|---|---|
| `capture.open(...)` | 先构造再打开，这样"摄像头/文件"两种来源可以共用同一个变量 |
| `cv::Ptr<cv::BackgroundSubtractor>` | OpenCV 的智能指针（类似 `shared_ptr`），算法对象都由 `create*` 工厂函数返回 |
| `createBackgroundSubtractorMOG2()` | MOG2 = 混合高斯背景建模，为每个像素维护若干高斯分布 |
| `mog2->apply(frame, foreground)` | **核心调用**：输入当前帧，输出单通道掩码。前景 = 255，背景 = 0，阴影约 127 |
| `while (capture.read(frame))` | 读帧成功就继续；视频读完自动结束循环 |

### 7.3 原理一句话

MOG2 假设"每个像素的颜色长期看服从若干个高斯分布"。一开始模型是空的，需要几十帧学习背景；
学习完成后，**偏离背景模型**的像素被判为前景。所以它天然依赖两个假设：**摄像头固定**、**光照稳定**。

### 7.4 运行结果

- 静止画面：掩码几乎全黑（学完背景后）。
- 有物体移动：出现白色块。
- 摄像头晃动：整幅画面都变白 —— 这是算法假设被破坏，不是 bug。

### 7.5 易错点

- 把掩码当三通道处理：它其实是 `CV_8UC1`。
- 前几十帧掩码很乱：正常，背景还没学完。
- 想调参：`mog2->setHistory(500)`（学习帧数）、`setVarThreshold(16)`（判定阈值）、`setDetectShadows(false)`（关阴影）。

---

## 8. ORB 特征检测与匹配

**文件**：`src/07_features2d_orb.cpp`

### 8.1 完整源码

```cpp
// OpenCV 核心模块：Mat、Scalar、Point、Vec、RNG 等基础类型。
#include <opencv2/core.hpp>
// 图像处理模块：绘图、颜色转换、滤波、阈值、边缘等 API。
#include <opencv2/imgproc.hpp>
// 二维特征模块：提供 ORB、BFMatcher 和 drawMatches。
#include <opencv2/features2d.hpp>
// HighGUI 模块：提供 imshow、waitKey 和窗口管理。
#include <opencv2/highgui.hpp>
// 图像编解码模块：提供 imread 和 imwrite。
#include <opencv2/imgcodecs.hpp>

// C++17 文件系统：用于创建 output 目录。
#include <filesystem>
#include <iostream>
#include <vector>

static cv::Mat makeTextureImage()
{
    cv::Mat image(480, 640, CV_8UC1, cv::Scalar(235));
    // 固定随机种子，保证每次生成相同纹理，便于复现实验。
    cv::RNG rng(12345);
    for (int i = 0; i < 180; ++i) {
        const cv::Point center(rng.uniform(20, 620), rng.uniform(20, 460));
        const int radius = rng.uniform(3, 16);
        const int value = rng.uniform(0, 220);
        cv::circle(image, center, radius, cv::Scalar(value),
                   rng.uniform(1, 4), cv::LINE_AA);
    }
    for (int i = 0; i < 35; ++i) {
        cv::Point p1(rng.uniform(0, 640), rng.uniform(0, 480));
        cv::Point p2(rng.uniform(0, 640), rng.uniform(0, 480));
        cv::line(image, p1, p2, cv::Scalar(rng.uniform(0, 220)), 2, cv::LINE_AA);
    }
    return image;
}

int main()
{
    // 运行前创建输出目录，避免保存图片失败。
    std::filesystem::create_directories("output");
    cv::Mat image1 = makeTextureImage();
    cv::Mat image2;
    const cv::Mat affine = (cv::Mat_<double>(2, 3) << 1, 0, 18, 0, 1, 12);
    // 根据仿射矩阵生成第二张平移后的图像。
    cv::warpAffine(image1, image2, affine, image1.size());

    // 创建 ORB；1000 是最多保留的关键点数量。
    const cv::Ptr<cv::ORB> orb = cv::ORB::create(1000);
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;

    // 一次完成关键点检测和二进制描述子计算；noArray 表示不限制区域。
    orb->detectAndCompute(image1, cv::noArray(), keypoints1, descriptors1);
    orb->detectAndCompute(image2, cv::noArray(), keypoints2, descriptors2);

    std::cout << "Keypoints image 1: " << keypoints1.size() << '\n'
              << "Keypoints image 2: " << keypoints2.size() << '\n';

    if (descriptors1.empty() || descriptors2.empty()) {
        std::cerr << "No descriptors found.\n";
        return 1;
    }

    // ORB 的描述子是二进制描述子，通常使用 Hamming 距离。
    // ORB 描述子是二进制串，所以匹配距离使用汉明距离；true 开启互相验证。
    cv::BFMatcher matcher(cv::NORM_HAMMING, true);
    std::vector<cv::DMatch> matches;
    // 为每个查询描述子寻找距离最近的训练描述子。
    matcher.match(descriptors1, descriptors2, matches);

    cv::Mat matchImage;
    // 把两图并排拼接，并绘制匹配关键点之间的连线。
    cv::drawMatches(image1, keypoints1, image2, keypoints2,
                    matches, matchImage);

    cv::imwrite("output/07_orb_matches.png", matchImage);
    cv::imshow("ORB feature matching", matchImage);
    // 0 表示无限等待按键；实时程序则应使用很小的毫秒数。
    cv::waitKey(0);
    return 0;
}
```

### 8.2 逐段讲解

| 代码 | 含义 |
|---|---|
| `makeTextureImage()` | 生成一张有 180 个圆 + 35 条线的纹理图。ORB 需要**角点/纹理**，纯色图提不出特征 |
| `cv::RNG rng(12345)` | 固定种子的随机数生成器，保证每次运行结果一样（可复现） |
| `cv::Mat_<double>(2,3) << 1,0,18, 0,1,12` | 2×3 仿射矩阵：x 方向平移 18，y 方向平移 12 |
| `warpAffine(image1, image2, affine, size)` | 按仿射矩阵生成第二张图（等价于"同一场景平移后再拍一张"） |
| `ORB::create(1000)` | 最多保留 1000 个特征点；ORB = FAST 角点 + BRIEF 二进制描述子 |
| `detectAndCompute(img, noArray(), kps, desc)` | 一步完成"找关键点 + 算描述子"。第二个参数是掩码，`noArray()` 表示全图 |
| `descriptors.empty()` | 纯色图可能一个特征都提不出来，必须判空 |
| `BFMatcher(NORM_HAMMING, true)` | 暴力匹配器：对每个描述子找最近邻；ORB 是**二进制**描述子，所以用**汉明距离**；`true` = crossCheck，双向互查 |
| `matcher.match(desc1, desc2, matches)` | 输出 `DMatch` 列表，每项含 `queryIdx`/`trainIdx`/`distance` |
| `drawMatches(...)` | 把两张图并排拼接，画出匹配连线 |

### 8.3 为什么用汉明距离

ORB 描述子是 256 bit 的**二进制串**，比较的是"有多少位不同"，汉明距离正好就是这个含义，
而且可以用异或 + popcount 极快算出来。用欧氏距离（`NORM_L2`）则完全没有物理意义。

### 8.4 运行结果

```text
Keypoints image 1: 1000
Keypoints image 2: 995
```

`output/07_orb_matches.png`：两张图并排，连线大体平行、方向一致（因为只是平移）。

### 8.5 易错点

- 彩色图直接进 `detectAndCompute`：断言失败，必须先转灰度。
- 用 `NORM_L2`：匹配结果很差。
- 期望"完美匹配"：ORB 有误匹配，实际工程里还要用 ratio test / RANSAC 过滤。

> 想用真实图片：把 `makeTextureImage()` 换成读 `assets/box.png` 与 `assets/box_in_scene.png`，
> 其余代码不变，匹配效果更有说服力。

---

## 9. 相机几何与 solvePnP

**文件**：`src/08_calib3d_pnp.cpp`

### 9.1 完整源码

```cpp
// OpenCV 核心模块：Mat、Scalar、Point、Vec、RNG 等基础类型。
#include <opencv2/core.hpp>
// 相机标定与三维重建模块：提供 projectPoints 和 solvePnP。
#include <opencv2/calib3d.hpp>

#include <iostream>
#include <vector>

int main()
{
    // 相机内参矩阵：fx、fy 为焦距（像素单位），cx、cy 为主点。
    // 相机内参 K：fx/fy 是像素焦距，cx/cy 是主点坐标。
    const cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) <<
        800.0, 0.0, 320.0,
        0.0, 800.0, 240.0,
        0.0, 0.0, 1.0);
    // 五个畸变系数全零，表示本例假设镜头没有畸变。
    const cv::Mat distCoeffs = cv::Mat::zeros(5, 1, CV_64F);

    // 构造立方体的 8 个三维角点，单位可理解为任意一致的长度单位。
    // 立方体八个角点的三维坐标，单位可以是任意一致的长度单位。
    const std::vector<cv::Point3f> objectPoints = {
        {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0},
        {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1}
    };

    // 为演示生成一组已知位姿下的二维投影点。
    const cv::Mat trueRvec = (cv::Mat_<double>(3, 1) << 0.1, -0.15, 0.05);
    const cv::Mat trueTvec = (cv::Mat_<double>(3, 1) << 0.0, 0.0, 6.0);
    std::vector<cv::Point2f> imagePoints;
    // 先用已知真值把 3D 点投影成 2D 点，构造无噪声测试数据。
    cv::projectPoints(objectPoints, trueRvec, trueTvec,
                      cameraMatrix, distCoeffs, imagePoints);

    // solvePnP 根据 3D-2D 对应关系估计物体相对于相机的位姿。
    cv::Mat estimatedRvec, estimatedTvec;
    // 再根据 3D-2D 对应点、内参和畸变反求位姿。
    const bool success = cv::solvePnP(
        objectPoints, imagePoints,
        cameraMatrix, distCoeffs,
        estimatedRvec, estimatedTvec,
        false, cv::SOLVEPNP_ITERATIVE);

    // solvePnP 返回 false 时不能使用输出的旋转和平移结果。
    if (!success) {
        std::cerr << "solvePnP failed.\n";
        return 1;
    }

    std::cout << "Estimated rotation vector:\n" << estimatedRvec << '\n';
    std::cout << "Estimated translation vector:\n" << estimatedTvec << '\n';
    std::cout << "This example uses synthetic correspondences; real calibration\n"
                 "requires images of a known calibration target.\n";
    return 0;
}
```

### 9.2 逐段讲解

| 代码 | 含义 |
|---|---|
| `cameraMatrix` 3×3 | 内参：`fx/fy` 焦距（像素单位），`cx/cy` 主点（通常接近图像中心 320,240） |
| `Mat::zeros(5,1,CV_64F)` | 5 个畸变系数全零，表示"理想无畸变镜头" |
| `objectPoints` 8 个 `Point3f` | 世界坐标系下边长 1 的立方体角点 |
| `trueRvec = (0.1, -0.15, 0.05)` | 旋转向量（罗德里格斯形式），方向是旋转轴、长度是旋转角（弧度） |
| `trueTvec = (0, 0, 6)` | 平移：物体在相机前方 6 个单位 |
| `projectPoints(...)` | 3D → 2D 投影，得到"假设相机看到"的像素坐标（这一步是造数据） |
| `solvePnP(objPts, imgPts, K, dist, rvec, tvec, false, SOLVEPNP_ITERATIVE)` | **核心**：已知 3D 点、对应 2D 点、内参、畸变，求位姿。返回 `bool` |
| 打印估计值 | 合成数据下应该和真值几乎一模一样 |

### 9.3 内参/外参/畸变，一次分清

```text
内参 K    ：相机自身的属性（焦距、主点）      —— 由 calibrateCamera 标定得到
外参 R, t ：物体/世界相对相机的位置与姿态      —— 就是 solvePnP 要求的东西
畸变系数  ：镜头造成的径向/切向变形            —— 也在标定时一起估出来
```

`solvePnP` **不是**相机标定：它假设内参已知，只求外参。

### 9.4 运行结果

```text
Estimated rotation vector:
[0.1000000288034142;
 -0.1500000407666627;
 0.0500000188704091]
Estimated translation vector:
[1.565736324250528e-08;
 -1.981155247294888e-08;
 6.000000016880526]
```

和真值 `(0.1, -0.15, 0.05)` / `(0, 0, 6)` 对比，误差在 1e-8 量级 —— 说明求解正确。

### 9.5 易错点

- `Point3f` 写成 `Point2f`：编译或运行报错。
- 以为 `solvePnP` 能顺便算出内参：不能。
- 用真实照片却沿用这套假内参：结果没有物理意义。

> 拓展：`assets/chessboard.png`、`assets/calib_left01.jpg` 是棋盘格图，
> 可以用 `findChessboardCorners` + `cornerSubPix` 找角点，体会真实标定的输入。

---

## 10. DNN 推理：加载 ONNX 并前向

**文件**：`src/09_dnn_onnx.cpp`

### 10.1 完整源码

```cpp
// OpenCV 核心模块：Mat、Scalar、Point、Vec、RNG 等基础类型。
#include <opencv2/core.hpp>
// 图像编解码模块：提供 imread 和 imwrite。
#include <opencv2/imgcodecs.hpp>
// 深度神经网络模块：提供 ONNX 加载、blob 和前向推理。
#include <opencv2/dnn.hpp>

#include <iostream>
#include <string>
#include <vector>

// argc/argv 让程序可以从命令行接收输入图片路径。
int main(int argc, char** argv)
{
    if (argc < 3) {
        std::cout << "Usage: 09_dnn_onnx <model.onnx> <image>\n"
                  << "The model's input size and preprocessing must match its training/export settings.\n";
        return 0;
    }

    // 保存模型和图片路径，后续错误信息可以直接打印它们。
    const std::string modelPath = argv[1];
    const std::string imagePath = argv[2];

    // DNN 输入通常从彩色图片开始，后面再统一做 blob 预处理。
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
            // 读取失败返回空 Mat；必须在后续访问前检查。
    if (image.empty()) {
        std::cerr << "Cannot read image: " << imagePath << '\n';
        return 1;
    }

    cv::dnn::Net net;
    try {
        // 读取 ONNX 计算图和权重；格式问题会抛出 cv::Exception。
        net = cv::dnn::readNetFromONNX(modelPath);
    } catch (const cv::Exception& e) {
        std::cerr << "Failed to load ONNX model: " << e.what() << '\n';
        return 1;
    }

    if (net.empty()) {
        std::cerr << "Loaded network is empty.\n";
        return 1;
    }

    // 示例使用常见的 640x640、缩放到 [0,1]、交换 B/R 通道配置。
    // 这只是演示参数；必须根据具体模型的预处理定义修改。
    // 把 HxWxC 图片转换成网络常用的 NCHW 四维输入张量。
    cv::Mat blob = cv::dnn::blobFromImage(
        image, 1.0 / 255.0, cv::Size(640, 640),
        cv::Scalar(), true, false);

    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    // 将预处理后的 blob 设置为网络当前输入。
    net.setInput(blob);

    try {
        std::vector<cv::Mat> outputs;
        // 执行前向推理，并收集所有未连接输出层的结果。
        net.forward(outputs, net.getUnconnectedOutLayersNames());
        std::cout << "Output tensors: " << outputs.size() << '\n';
        for (std::size_t i = 0; i < outputs.size(); ++i) {
            std::cout << "Output[" << i << "] dims=" << outputs[i].dims
                      << ", total=" << outputs[i].total()
                      << ", type=" << outputs[i].type() << '\n';
            std::cout << "Shape: ";
            for (int d = 0; d < outputs[i].dims; ++d) {
                std::cout << outputs[i].size[d]
                          << (d + 1 == outputs[i].dims ? '\n' : 'x');
            }
        }
    } catch (const cv::Exception& e) {
        std::cerr << "Inference failed: " << e.what() << '\n';
        return 1;
    }

    std::cout << "Inference completed. Decoding outputs is model-specific.\n";
    return 0;
}
```

### 10.2 逐段讲解

| 代码 | 含义 |
|---|---|
| `argc < 3` | 少参数时打印用法并 `return 0`：**"没给参数"不算错误**，所以返回 0 |
| `readNetFromONNX(path)` | 解析 ONNX 文件，失败会**抛 `cv::Exception`**，必须 try/catch |
| `net.empty()` | 再判一次：有的失败路径不抛异常 |
| `blobFromImage(img, 1/255, Size(640,640), Scalar(), swapRB=true, crop=false)` | 预处理成 4 维张量 `1×3×640×640`：缩放像素到 [0,1]、resize 到 640×640、BGR→RGB |
| `setPreferableBackend(DNN_BACKEND_OPENCV)` | 用 OpenCV 自带实现（也可换 CUDA/OpenVINO 等后端） |
| `setPreferableTarget(DNN_TARGET_CPU)` | 在 CPU 上算 |
| `setInput(blob)` | 喂入输入 |
| `getUnconnectedOutLayersNames()` | 拿到输出层名字（可能多个） |
| `net.forward(outputs, names)` | 执行前向，结果填进 `vector<Mat>` |
| 打印 `dims / total() / type() / size[d]` | 看输出张量形状，这是接后处理的第一步 |

### 10.3 预处理为什么是"必须一致"的

模型在训练时约定了：输入尺寸、通道顺序（RGB 还是 BGR）、是否归一化、均值方差、是否裁剪。
`blobFromImage` 只是把这些操作暴露成参数，**参数填错不会报错，但结果全错**。
示例里的 640×640 + `1/255` + `swapRB=true` 只是演示值。

### 10.4 运行结果

```powershell
.\build\Release\09_dnn_onnx.exe                      # 打印用法，退出码 0
.\build\Release\09_dnn_onnx.exe assets\model.onnx assets\sample.jpg
```

有模型时输出类似：

```text
Output tensors: 1
Output[0] dims=3, total=705600, type=5
Shape: 1x84x8400
```

### 10.5 易错点

- 预处理参数照抄：模型要 `1/255` 你给 `1.0`，要 RGB 你给 BGR。
- 输出形状和你猜的不一样：**必须查模型文档**，后处理无法通用。
- 本课程不附带模型文件（体积与许可问题），可先验证"无参数/错误路径"的行为。

---

## 11. 综合项目：实时图像处理工具

**文件**：`src/10_realtime_tool.cpp`

### 11.1 完整源码

```cpp
// OpenCV 核心模块：Mat、Scalar、Point、Vec、RNG 等基础类型。
#include <opencv2/core.hpp>
// 视频输入输出模块：提供 VideoCapture。
#include <opencv2/videoio.hpp>
// 图像处理模块：绘图、颜色转换、滤波、阈值、边缘等 API。
#include <opencv2/imgproc.hpp>
// 图像编解码模块：提供 imread 和 imwrite。
#include <opencv2/imgcodecs.hpp>
// HighGUI 模块：提供 imshow、waitKey 和窗口管理。
#include <opencv2/highgui.hpp>

// C++17 文件系统：用于创建 output 目录。
#include <filesystem>
#include <iostream>

// argc/argv 让程序可以从命令行接收输入图片路径。
int main(int argc, char** argv)
{
    // 用户可以传入摄像头编号；缺省使用设备 0。
    const int cameraIndex = (argc >= 2) ? std::stoi(argv[1]) : 0;
    // 打开摄像头，后续 read 会不断取得新帧。
    cv::VideoCapture camera(cameraIndex);
    if (!camera.isOpened()) {
        std::cerr << "Cannot open camera. Try another camera index.\n";
        return 1;
    }

    // 运行前创建输出目录，避免保存图片失败。
    std::filesystem::create_directories("output");
    // 当前模式：1 原图、2 灰度、3 高斯平滑、4 Canny。
    int mode = 1;
    cv::Mat frame, output, gray, blurred;

    std::cout << "1: Original | 2: Gray | 3: Gaussian | 4: Canny\n"
                 "S: Save current result | ESC: Quit\n";

    // 只要读帧成功就继续处理，设备断开时循环自动结束。
    while (camera.read(frame)) {
        if (frame.empty()) {
            continue;
        }

        // 根据用户选择执行不同的图像处理分支。
        switch (mode) {
        case 1:
            // Mat 赋值是浅拷贝，只复制头并共享当前帧像素。
            output = frame;
            break;
        case 2:
            cv::cvtColor(frame, output, cv::COLOR_BGR2GRAY);
            break;
        case 3:
            cv::GaussianBlur(frame, output, cv::Size(5, 5), 1.0);
            break;
        case 4:
            // Canny 需要单通道输入，因此先把 BGR 转成灰度。
            cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
            // 先平滑再检测边缘，可以减少噪声造成的边缘。
            cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 1.0);
            // 低/高阈值为 50/150，结果 output 是单通道边缘图。
            cv::Canny(blurred, output, 50, 150);
            break;
        default:
            // Mat 赋值是浅拷贝，只复制头并共享当前帧像素。
            output = frame;
            break;
        }

        cv::imshow("Camera", frame);
        cv::imshow("Processing result", output);

        // 1ms 等待让窗口刷新，同时取得本帧键盘输入。
        const int key = cv::waitKey(1) & 0xFF;
        if (key == 27) {
            break;
        }
        if (key >= '1' && key <= '4') {
            // 字符数字减去字符 0，得到对应的整数模式。
            mode = key - '0';
        } else if (key == 's' || key == 'S') {
            // 把模式号放入文件名，防止不同模式互相覆盖。
            const std::string filename = "output/10_mode_" +
                std::to_string(mode) + ".png";
            if (!cv::imwrite(filename, output)) {
                std::cerr << "Failed to save " << filename << '\n';
            } else {
                std::cout << "Saved " << filename << '\n';
            }
        }
    }

    // 退出前释放摄像头句柄。
    camera.release();
    cv::destroyAllWindows();
    return 0;
}
```

### 11.2 逐段讲解

| 代码 | 含义 |
|---|---|
| `int mode = 1` | 当前处理模式，1=原图 |
| `cv::Mat frame, output, gray, blurred` | 复用 Mat 变量：**在循环外声明**，避免每帧重新分配内存 |
| `while (camera.read(frame))` | 读帧失败自动退出循环（比如摄像头被拔） |
| `continue` | 空帧直接跳过本帧，不让后面的 `cvtColor` 崩 |
| `switch (mode)` | 四种处理：原图 / 灰度 / 高斯 / Canny（Canny 前先灰度+高斯） |
| `key >= '1' && key <= '4'` | 用字符范围判断数字键，比四个 `if` 干净 |
| `mode = key - '0'` | 字符 `'3'` 的 ASCII 是 51，减去 `'0'`(48) 得到数字 3 |
| `std::to_string(mode)` | 拼进文件名，避免覆盖 |
| `release()` / `destroyAllWindows()` | 收尾释放 |

### 11.3 运行结果

- 两个窗口：Camera（原始画面）+ Processing result（按模式处理后的画面）。
- 按 1/2/3/4 实时切换，延迟很低。
- 按 S 保存 `output/10_mode_2.png` 这类文件，文件名带模式号。
- 按 ESC 退出，摄像头释放。

### 11.4 易错点

- 在循环里 `cv::Mat` 反复声明：每帧都分配内存，帧率下降。
- 忘了 `continue`：空帧直接进 `cvtColor` 崩溃。
- Canny 直接用彩色 `frame`：断言失败，必须先灰度。

### 11.5 建议的拓展练习

1. 加 HSV 颜色筛选：`cvtColor(frame, hsv, COLOR_BGR2HSV)` + `inRange(hsv, low, high, mask)`。
2. 用 `morphologyEx` 清理掩码，再 `findContours` + `boundingRect` 框出目标。
3. 左上角用 `putText` 显示 FPS（自己用 `getTickCount`/`steady_clock` 算）。

---

## 附录 A：素材清单（assets/）

| 文件 | 用途 |
|---|---|
| `sample.jpg` | 彩色照片：第 2、5 课读写/滤波/阈值/边缘 |
| `butterfly.jpg` | 彩色照片：对比不同内容的处理效果 |
| `baboon.jpg` | 纹理图：第 4 课像素遍历与性能对比 |
| `box.png` / `box_in_scene.png` | 第 8 课 ORB 匹配的模板与场景 |
| `chessboard.png` / `calib_left01.jpg` | 第 9 课拓展：角点检测与标定 |
| `sudoku.png` | 二值文档图：自适应阈值、形态学 |

## 附录 B：常用 API 速查

```text
【Mat】
  Mat(rows, cols, type, Scalar)   创建（先高后宽）
  rows / cols / channels() / depth() / type() / elemSize() / step[0] / total()
  empty()  clone()  copyTo()  convertTo(dst, type, alpha, beta)
  at<Vec3b>(y, x)  ptr<Vec3b>(y)  begin<T>() / end<T>()
  image(Rect(x,y,w,h))            ROI（浅拷贝）

【imgcodecs / highgui】
  imread(path, IMREAD_COLOR|IMREAD_GRAYSCALE|IMREAD_UNCHANGED)
  imwrite(path, mat) -> bool
  imshow(name, mat)  waitKey(ms)  destroyAllWindows()

【imgproc】
  cvtColor(src, dst, COLOR_BGR2GRAY|COLOR_BGR2HSV)
  resize(src, dst, Size(w,h), 0, 0, INTER_LINEAR)
  GaussianBlur(src, dst, Size(k,k), sigma)   medianBlur(src, dst, k)
  threshold(src, dst, thresh, maxval, THRESH_BINARY)
  adaptiveThreshold(src, dst, maxval, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, blockSize, C)
  Canny(src, dst, low, high)
  getStructuringElement(MORPH_RECT, Size(k,k))  morphologyEx(..., MORPH_OPEN|MORPH_CLOSE, kernel)
  findContours / drawContours / contourArea / boundingRect

【videoio / video】
  VideoCapture(idx 或 路径)  isOpened()  read(frame)  get/set(prop)  release()
  createBackgroundSubtractorMOG2()  ->apply(frame, fgmask)
  calcOpticalFlowPyrLK / calcOpticalFlowFarneback

【features2d / calib3d】
  ORB::create(n)  detectAndCompute(img, noArray(), kps, desc)
  BFMatcher(NORM_HAMMING, crossCheck)  match / knnMatch  drawMatches
  projectPoints  solvePnP  Rodrigues  findChessboardCorners  calibrateCamera  undistort

【dnn】
  readNetFromONNX(path)  blobFromImage(img, scale, Size, mean, swapRB, crop)
  setPreferableBackend/Target  setInput  forward(outputs, names)  getUnconnectedOutLayersNames()
```

## 附录 C：报错速查

| 现象 | 原因 | 处理 |
|---|---|---|
| 一堆 `C2065 未声明的标识符`、行号错乱 | MSVC 按 GBK 解析 UTF-8 源码 | 确认 `CMakeLists.txt` 有 `/utf-8` |
| `imread` 返回空 / `imwrite` 失败 | 相对路径不对或目录不存在 | 工作目录设为项目根；先 `create_directories("output")` |
| 运行时 `Assertion failed` | 通道/类型不匹配（彩色图进 `threshold`/`Canny`） | 先 `cvtColor` 到灰度 |
| 窗口一闪而过 | 没调 `waitKey` | 加 `cv::waitKey(0)` |
| 找不到 `OpenCVConfig.cmake` | 没指定 `OpenCV_DIR` | `-DOpenCV_DIR="E:/opencv/cmake/opencv4"` |
| 视频文件读不出来 | 缺 ffmpeg 插件 DLL | 把 `E:\opencv\Debug\bin\opencv_videoio_ffmpeg4130_64.dll` 加到 `PATH` 或 exe 同目录 |
| 程序跑起来立刻退出、没图 | 工作目录不在项目根 | `cd F:\CodingFile\opencv_cpp_quickstart` 后再运行 |

## 附录 D：自己动手（src/）

`src/` 里是 11 个**空壳文件**：只有目标、API 提示、TODO 和验收标准，代码由你自己写。
`CMakeLists.txt` 会自动收集 `src/*.cpp`，每个文件编译成一个独立可执行目标：

```powershell
cmake -S . -B build -G "Visual Studio 16 2019" -A x64 -DOpenCV_DIR="E:/opencv/cmake/opencv4"
cmake --build build --config Release --target 07_features2d_orb
.\build\Release\07_features2d_orb.exe
```

建议节奏：先读本手册某一课的讲解，**合上屏幕自己敲**；卡住超过 15 分钟再回来看那一课的代码，只对比差异。

建议节奏：先照着本手册的讲解**合上屏幕自己敲**，卡住超过 15 分钟再看 `src/` 对应文件，只对比差异。
