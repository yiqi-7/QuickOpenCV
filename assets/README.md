# 素材目录

本课程的大部分示例都能自行生成演示图像，不要求预先下载数据集。以下图片已下载到本目录，可直接使用。

| 文件 | 类型 | 建议用途 |
|---|---|---|
| `sample.jpg` | 彩色照片 | `01_image_io`、`04_imgproc_pipeline`：读写、滤波、阈值、边缘 |
| `butterfly.jpg` | 彩色照片 | 对比不同内容下的阈值 / Canny 效果 |
| `baboon.jpg` | 彩色纹理 | `03_pixel_access`：像素遍历与性能对比 |
| `box.png` | 灰度模板图 | `07_features2d_orb`：特征匹配模板 |
| `box_in_scene.png` | 灰度场景图 | `07_features2d_orb`：特征匹配场景 |
| `chessboard.png` | 棋盘格 | `08_calib3d_pnp` 拓展：角点检测 |
| `calib_left01.jpg` | 棋盘格照片 | 相机标定的真实输入示例 |
| `sudoku.png` | 文档图像 | 自适应阈值、形态学处理拓展 |

可选素材（需要时自行准备）：

- `sample.mp4`：视频文件，可在 `05_videoio_camera` 的扩展练习中读取（需要 ffmpeg 插件 DLL）。
- `model.onnx`：自行训练或获取的 ONNX 模型，用于 `09_dnn_onnx`。

图片来源：OpenCV 官方示例数据（<https://github.com/opencv/opencv/tree/4.x/samples/data>），遵循其仓库许可，仅用于教学。

运行程序时可以通过命令行参数指定素材路径，例如：

```powershell
.\build\Release\01_image_io.exe assets\sample.jpg
```
