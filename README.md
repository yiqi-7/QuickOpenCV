# OpenCV C++ 快速入门：配套代码

这套代码与 `docs/OpenCV_Cpp_Quickstart_Guide.md` 配套。建议先阅读文档对应章节，再编译运行同编号示例，最后按照自学手册附录 D 完成 `src/` 中的练习。

**`src/` 里现在是空壳文件**（只有目标、API 提示、TODO 和验收标准），代码由你自己写。
每个示例的完整源码和逐段讲解在 `docs/动手学OpenCV_自学手册.md`，卡住时对照那一课。

## 1. 环境要求

- OpenCV 4.x（建议使用包含常用主模块的完整安装版本）
- C++17 编译器：GCC、Clang 或 MSVC
- CMake 3.16 或更高版本
- 具备图形界面的运行环境（需要显示窗口的示例）

OpenCV 官方模块列表：<https://docs.opencv.org/4.12.0/>

### 1.1 本机已配置好的环境

| 组件 | 位置 / 版本 |
|---|---|
| OpenCV | 4.13.0，**静态库**（运行时不需要 OpenCV DLL） |
| OpenCV 头文件 | `E:\opencv\Release\include` |
| OpenCV 库文件 | `E:\opencv\Release\lib`（Debug 用 `E:\opencv\Debug\lib`） |
| OpenCV CMake 包 | `E:\opencv\cmake\opencv4`（本机补装，供 `find_package(OpenCV 4)` 使用） |
| 编译器 | MSVC 19.29（Visual Studio 2019 Community） |
| CMake | 3.31.12（`E:\CMake\cmake-3.31.12-windows-x86_64\bin`） |

本机 `OpenCV_DIR` 可写为 `E:/opencv/cmake/opencv4`。

> 源码是 UTF-8（含中文注释）。简体中文 Windows 上 MSVC 默认按 GBK 解析源文件，会吞掉换行并报出大量
> “未声明的标识符”。`CMakeLists.txt` 里已对 MSVC 加上 `/utf-8`，新建文件时无需额外处理。

## 2. 目录结构

```text
opencv_cpp_quickstart/
├── CMakeLists.txt
├── README.md
├── docs/
│   ├── OpenCV_Cpp_Quickstart_Guide.md   # 讲义
│   └── 动手学OpenCV_自学手册.md          # 完整源码 + 逐段讲解
├── src/                                 # 你要写的代码（11 个空壳文件）
│   ├── 00_first_program.cpp
│   ├── 01_image_io.cpp
│   ├── 02_mat_roi.cpp
│   ├── 03_pixel_access.cpp
│   ├── 04_imgproc_pipeline.cpp
│   ├── 05_videoio_camera.cpp
│   ├── 06_video_mog2.cpp
│   ├── 07_features2d_orb.cpp
│   ├── 08_calib3d_pnp.cpp
│   ├── 09_dnn_onnx.cpp
│   └── 10_realtime_tool.cpp
├── assets/                              # 素材图片（已下载）
└── output/                              # 运行结果
```

`CMakeLists.txt` 会自动收集 `src/*.cpp`，每个文件编译成一个独立可执行目标（目标名 = 文件名去掉 `.cpp`）。
新增或重命名练习文件后不需要改 CMakeLists，重新配置即可。

## 3. 编译

### Linux

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

如果 CMake 找不到 OpenCV，可以显式指定 OpenCVConfig.cmake 所在目录：

```bash
cmake -S . -B build -DOpenCV_DIR=/path/to/opencv/lib/cmake/opencv4
```

### Windows（Visual Studio 生成器）

```powershell
cmake -S . -B build -A x64 -DOpenCV_DIR="C:/opencv/build"
cmake --build build --config Release
```

`OpenCV_DIR` 应指向包含 `OpenCVConfig.cmake` 的目录，而不是随意填写 OpenCV 根目录。

本机可直接使用：

```powershell
cmake -S . -B build -G "Visual Studio 16 2019" -A x64 -DOpenCV_DIR="E:/opencv/cmake/opencv4"
cmake --build build --config Release
```

### 3.1 编译你写的练习

**推荐：用一键脚本**（自动配置 + 编译，参数不用记）：

```powershell
.\build.ps1                     # 编译全部（Release）
.\build.ps1 01_image_io         # 只编译一个
.\build.ps1 01_image_io -Run    # 编译并运行（工作目录自动切到项目根）
.\build.ps1 -List               # 列出所有目标
.\build.ps1 -Config Debug       # 编 Debug
.\build.ps1 -Reconfigure        # 强制重新配置
```

脚本每次编译前都会跑一次快速配置（约 1～2 秒），所以**新增或删除 `src/*.cpp` 后不用手动重新配置**。

**等价的原生命令**：

```powershell
cmake -S . -B build -G "Visual Studio 16 2019" -A x64 -DOpenCV_DIR="E:/opencv/cmake/opencv4"
cmake --build build --config Release
```

只编译某一课（改错时最常用，其余文件不参与）：

```powershell
cmake --build build --config Release --target 01_image_io
```

> 注意：目标名来自文件名，用**下划线**（`00_first_program`），不是连字符；`--config`、`--build` 都是两个短横线。
> 新增 `src/*.cpp` 后如果要单独编新目标，必须先重跑一次上面的配置命令（或直接用 `.\build.ps1`）。

## 4. 运行示例

Linux 示例：

```bash
./build/00_first_program
./build/01_image_io
./build/02_mat_roi
./build/03_pixel_access
./build/04_imgproc_pipeline
./build/05_videoio_camera
./build/06_video_mog2
./build/07_features2d_orb
./build/08_calib3d_pnp
```

如果希望指定图片：

```bash
./build/01_image_io assets/sample.jpg
./build/04_imgproc_pipeline assets/sample.jpg
```

Windows 下可执行文件一般位于 `build/Release/`，具体取决于生成器和配置。**运行时工作目录设为项目根目录**，因为程序使用 `output/...` 这类相对路径。

### DNN 示例

DNN 需要学生自行准备 ONNX 模型与输入图像：

```bash
./build/09_dnn_onnx assets/model.onnx assets/sample.jpg
```

示例中的输入尺寸和归一化参数只是演示值。必须按照模型训练/导出时的约定修改；检测模型的输出解码也依赖模型结构。

## 5. 每个示例做什么

| 程序 | 对应知识 | 运行后观察什么 |
|---|---|---|
| 00_first_program | 环境、Mat 创建、版本 | 输出一张简单图像 |
| 01_image_io | imread、imwrite、empty | 图像属性与文件读写 |
| 02_mat_roi | Mat、Rect、ROI、clone、step | 修改 ROI 对原图的影响 |
| 03_pixel_access | at、ptr、迭代器、convertTo | 不同遍历写法结果一致；单次计时仅供观察 |
| 04_imgproc_pipeline | 灰度、滤波、阈值、Canny、形态学 | 不同处理结果图 |
| 05_videoio_camera | VideoCapture、实时帧 | 摄像头原图/灰度切换 |
| 06_video_mog2 | video、背景建模 | 前景掩码随场景变化 |
| 07_features2d_orb | ORB、BFMatcher | 两张平移图像之间的特征匹配 |
| 08_calib3d_pnp | 相机内参、投影、solvePnP | 根据合成 2D-3D 对应点估计位姿 |
| 09_dnn_onnx | DNN、blobFromImage、forward | 模型输出张量形状 |
| 10_realtime_tool | 模块串联 | 摄像头实时处理小工具 |

## 6. 素材（assets/）

已下载可直接使用的图片（来自 OpenCV 官方示例数据）：

| 文件 | 用途 |
|---|---|
| `sample.jpg` | 彩色照片，用于读写、滤波、阈值、边缘检测 |
| `butterfly.jpg` | 彩色照片，用于对比不同内容下的处理效果 |
| `baboon.jpg` | 纹理图，用于像素遍历与性能对比 |
| `box.png` / `box_in_scene.png` | 特征匹配的模板图与场景图（ORB） |
| `chessboard.png` / `calib_left01.jpg` | 棋盘格，用于角点检测与标定拓展 |
| `sudoku.png` | 文档类图像，用于自适应阈值、形态学拓展 |

## 7. 重要说明

1. 大多数图像示例会自行生成测试图像，`assets/` 里的图片用于替换成真实素材。
2. 摄像头示例需要系统授予摄像头权限；虚拟机、远程桌面和无图形界面环境可能无法显示窗口。
3. MOG2 需要一段时间学习背景，开始阶段的前景掩码可能不稳定。
4. 手册里的 ORB 示例使用程序生成的纹理图像，避免依赖外部图片；也可以改用 `assets/box*.png` 做真实图像匹配。
5. PnP 示例使用合成数据用于理解接口，不等于真实相机标定流程。
6. DNN 示例不附带模型文件。模型通常体积较大且有各自许可；请自行准备合法的 ONNX 文件。
7. 本机 OpenCV 是静态构建，示例程序不依赖 OpenCV DLL；只有读取视频文件时需要
   `opencv_videoio_ffmpeg4130_64.dll`，可从 `E:\opencv\Debug\bin` 复制到 `PATH` 或 exe 同目录。
8. `imshow` 需要 HighGUI 窗口后端。无显示服务器的 Linux 环境可先注释显示代码，保留文件输出和控制台结果。

## 8. 推荐学习方式

1. 打开 `docs/动手学OpenCV_自学手册.md` 的第 1 课：里面有**完整源码 + 逐段讲解**。
2. 看懂讲解后，打开 `src/` 里同编号的空壳文件，按文件头的 TODO 自己写一遍。
3. 编译、运行，对照该课的「运行结果」和「验收标准」。
4. 卡住超过 15 分钟，再回手册看那一课的代码，只对比差异。
5. 依次完成 00 → 10；最后独立完成 `10_realtime_tool.cpp`，并加上手册 11.5 的拓展功能。
