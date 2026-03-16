# SpeedyNote 技术栈与跨平台框架

## 概述

SpeedyNote 是一款跨平台手写笔记应用，能够在 Windows、macOS、Linux、Android 和 iOS 上运行。本文档介绍项目采用的核心技术栈和跨平台实现方案。

---

## 核心技术栈

### 1. Qt 框架 (主要跨平台技术)

**Qt** 是项目的主要跨平台 UI 和应用框架，提供了一次开发、多平台部署的能力。

```
┌─────────────────────────────────────────────────────────┐
│                    SpeedyNote 应用                       │
├─────────────────────────────────────────────────────────┤
│  Qt Framework (Qt5 / Qt6)                               │
│  ┌─────────┬─────────┬─────────┬─────────┬────────────┐ │
│  │  Core   │  GUI    │ Widgets │ Network │   XML      │ │
│  └─────────┴─────────┴─────────┴─────────┴────────────┘ │
├─────────────────────────────────────────────────────────┤
│  平台层: Windows | macOS | Linux | Android | iOS        │
└─────────────────────────────────────────────────────────┘
```

#### Qt 组件使用情况

| Qt 模块 | 用途 |
|---------|------|
| Qt Core | 核心功能：文件 I/O、JSON 处理、线程、时间 |
| Qt GUI | 图形渲染、剪贴板、输入事件 |
| Qt Widgets | 传统桌面 UI 组件 |
| Qt Network | 网络功能、PDF 下载 |
| Qt XML | 文档格式解析 |
| Qt SVG | SVG 图标渲染 |
| Qt Concurrent | 多线程处理 |
| Qt Test | 单元测试 (可选) |

### 2. C++17 标准

项目使用现代 C++17 标准开发，充分利用其特性：

```cpp
// 使用 structured bindings
auto [x, y] = getPosition();

// 使用 std::unique_ptr 管理资源
vectorLayers.push_back(std::make_unique<VectorLayer>("Layer 1"));

// 使用 std::optional (Qt6)
std::optional<QPointF> getLastPoint() const;
```

### 3. CMake 构建系统

项目使用 CMake 进行跨平台构建配置：

```cmake
cmake_minimum_required(VERSION 3.16)
project(SpeedyNote VERSION 1.3.1 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
```

---

## 跨平台实现方案

### 1. 条件编译 (Preprocessor Guards)

项目使用 Qt 提供的平台宏来实现平台特定代码：

```cpp
#ifdef Q_OS_WIN
    // Windows 特定代码
    #include <windows.h>
#elif defined(Q_OS_ANDROID)
    // Android 特定代码
    #include <QJniObject>
#elif defined(Q_OS_IOS)
    // iOS 特定代码
    #include "../ios/IOSTouchTracker.h"
#elif defined(Q_OS_LINUX)
    // Linux 特定代码
#endif
```

#### 常用 Qt 平台宏

| 宏 | 描述 |
|----|------|
| `Q_OS_WIN` | Windows 平台 |
| `Q_OS_MACOS` | macOS 平台 |
| `Q_OS_LINUX` | Linux 平台 |
| `Q_OS_ANDROID` | Android 平台 |
| `Q_OS_IOS` | iOS 平台 |
| `Q_OS_WASM` | WebAssembly 平台 |

### 2. 平台特定源文件

项目按平台组织特定代码：

```
source/
├── android/
│   ├── AndroidShareHelper.cpp
│   └── PdfPickerAndroid.cpp
├── ios/
│   ├── IOSTouchTracker.cpp
│   ├── IOSPlatformHelper.cpp
│   ├── PdfPickerIOS.mm
│   └── SnbxPickerIOS.mm
├── core/
│   ├── TouchGestureHandler.cpp    # 包含 Android/iOS JNI 代码
│   └── DarkModeUtils.cpp          # 跨平台颜色处理
└── Main.cpp                        # 包含 Android 特定代码
```

### 3. CMake 平台检测

在 CMakeLists.txt 中自动检测目标平台：

```cmake
# iOS 检测
if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    set(IOS TRUE)
endif()

# Android 检测
if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    set(ANDROID TRUE)
endif()

# Windows 检测
if(WIN32)
    # Windows 配置
endif()
```

---

## 第三方依赖

### 1. MuPDF

用于 PDF 渲染和导出，是跨平台的 PDF 处理库：

```cpp
#include <mupdf/fitz.h>
#include <mupdf/pdf.h>
```

- **Windows**: 通过 MSYS2 或预编译静态库
- **Android**: 从 `android/mupdf-build/${ANDROID_ABI}/` 加载
- **iOS/macOS**: 通过 Homebrew 或系统包

### 2. SDL2 (可选)

游戏手柄/控制器支持：

```cmake
option(ENABLE_CONTROLLER_SUPPORT "Enable SDL2 game controller support" OFF)
```

### 3. Qt 兼容层

项目包含 Qt 版本兼容处理：

```cpp
// Qt5/Qt6 兼容性
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    #include <QTextCodec>
#else
    // Qt6 无需编码转换
#endif
```

---

## 架构设计

### 模块化架构

```
┌─────────────────────────────────────────────────────────┐
│                     UI Layer                            │
│  (Toolbars, Sidebars, Dialogs, Launcher)                │
├─────────────────────────────────────────────────────────┤
│                   Core Layer                            │
│  (Document, Page, DocumentViewport, DocumentManager)   │
├─────────────────────────────────────────────────────────┤
│                   Data Layer                            │
│  (VectorLayer, VectorStroke, InsertedObject)            │
├─────────────────────────────────────────────────────────┤
│                 Platform Layer                          │
│  (Android JNI, iOS Native, System Notifications)       │
└─────────────────────────────────────────────────────────┘
```

### 设计模式

| 模式 | 使用场景 |
|------|----------|
| 单例模式 | `ShortcutManager`, `NotebookLibrary`, `DocumentManager` |
| 观察者模式 | Qt 信号槽机制 (signals/slots) |
| 工厂模式 | `PdfProviderFactory` 创建不同 PDF 提供器 |
| 策略模式 | 不同工具的实现 (Pen, Eraser, Marker) |

---

## 构建配置

### 支持的平台

| 平台 | Qt 版本 | 构建工具 |
|------|---------|----------|
| Windows (x64) | Qt5 / Qt6 | MSYS2 Clang64 |
| Windows (ARM64) | Qt6 | MSYS2 ClangARM64 |
| Windows (x86) | Qt5 | MinGW |
| macOS | Qt5 / Qt6 | Xcode / CMake |
| Linux | Qt5 / Qt6 | GCC / CMake |
| Android | Qt5 / Qt6 | NDK / CMake |
| iOS | Qt5 / Qt6 | Xcode |

### 构建选项

```cmake
# 可选功能
option(ENABLE_CONTROLLER_SUPPORT "SDL2 手柄支持" OFF)
option(ENABLE_DEBUG_OUTPUT "调试输出" OFF)
option(USE_QT5 "使用 Qt5 替代 Qt6" OFF)
```

---

## 总结

SpeedyNote 的跨平台能力主要依赖于：

1. **Qt 框架** - 提供统一的 API 覆盖所有主流桌面和移动平台
2. **条件编译** - 使用 Qt 平台宏处理平台差异
3. **独立平台代码** - Android (JNI) 和 iOS (Objective-C++) 特定实现
4. **CMake 构建系统** - 自动化平台检测和配置
5. **跨平台库** - MuPDF、SDL2 等原生跨平台库

这种架构使得开发者可以用统一的 C++/Qt 代码库，同时针对每个平台进行必要的适配，最终实现"一次编写，多平台运行"的目标。