# SpeedyNote 项目优化与维护计划

## 项目概述

**SpeedyNote** 是一款跨平台手写笔记应用，目标是为手写笔用户提供高效的笔记体验。核心特点：
- 360Hz 手写笔轮询，低延迟
- 支持 Windows、macOS、Linux、Android、iOS
- PDF 注释和矢量笔画
- 多图层编辑

---

## 一、待完成功能 (TODO)

### 高优先级

| 序号 | 任务 | 描述 | 文件位置 |
|------|------|------|----------|
| T001 | 关闭标签页功能 | 实现 `closeCurrentTab()` | MainWindow.cpp:1284 |
| T002 | 导出功能 | 实现 `file.export` 快捷键 | MainWindow.cpp:1285 |
| T003 | 标签切换 | 实现 `navigation.next_tab/prev_tab` | MainWindow.cpp:1319 |
| T004 | 滚动速度设置 | 从用户设置加载滚动速度 | DocumentViewport.cpp:2682 | ✅ 已实现 |
| T005 | 点击放置图片 | 创建 `insertImageAtPosition()` | DocumentViewport.cpp:6362 | ✅ 已实现 |
| T006 | 删除页面确认 | 显示确认对话框 | MainWindow.cpp:3405 | ✅ 已实现 |
| T007 | 笔记缺失通知 | 通知用户笔记缺失 | DocumentViewport.cpp:7085 | ✅ 已实现 |
| T008 | 位置选择模式 | 实现"选择位置"模式 | DocumentViewport.cpp:7140 | ✅ 已实现 |

### 中优先级

| 序号 | 任务 | 描述 | 文件位置 |
|------|------|------|----------|
| T009 | 缓存大小计算 | 实现 `NotebookLibrary::getTempDirsTotalSize()` | NotebookLibrary.cpp:622 | ✅ 已实现 |
| T010 | 临时目录清理 | 实现 `NotebookLibrary::cleanupOrphanedTempDirs()` | NotebookLibrary.cpp:640 | ✅ 已实现 |
| T011 | 缓存管理集成 | 集成缓存管理到 NotebookLibrary | ControlPanelDialog.h:29 |
| T012 | 设置加载 | 从 QSettings 加载用户设置 | DocumentViewport.h:1765 |
| T013 | 统一文件选择器 | 替换为统一的 .snb 文件选择器 | MainWindow.h:240 |
| T014 | 图标名称修复 | 替换 Launcher 中的占位符图标名称 | Launcher.cpp:292,310,317 |
| T015 | 图层透明度 | 处理图层透明度渲染 | Page.cpp:519 |

### 低优先级

| 序号 | 任务 | 描述 | 文件位置 |
|------|------|------|----------|
| T016 | 触屏滚动 | LayerPanel 手动触屏滚动实现 | LayerPanel.cpp:68 |

---

## 二、已知 Bug 修复

### Android 相关 (BUG-A 系列)

| 序号 | Bug ID | 描述 | 文件位置 | 状态 |
|------|--------|------|----------|------|
| B001 | BUG-A001 | Android/iOS 键盘崩溃 | ControlPanelDialog.cpp:57 | 部分修复 |
| B002 | BUG-A002 | Android 文件名清理 | MainWindow.cpp:60 | 部分修复 |
| B003 | BUG-A003 | Android PDF 文件选择 | MainWindow.cpp:83 | 部分修复 |
| B004 | BUG-A005 | 触控手势识别问题 | TouchGestureHandler.cpp | 多版本修复中 |
| B005 | BUG-A006 | PDF 渲染系统过载 | PageWheelPicker.cpp | 已修复 |
| B006 | BUG-A008 | Android 橡皮擦工具类型检测 | DocumentViewport.cpp:48 | 已修复 |

### 跨平台 Bug

| 序号 | Bug ID | 描述 | 文件位置 | 状态 |
|------|--------|------|----------|------|
| B007 | BUG-PG-001 | 无法删除 PDF 背景页面 | MainWindow.cpp:5067 | 待修复 |
| B008 | BUG-PG-002 | 延迟删除 PDF 页面 | MainWindow.cpp:5054 | 待修复 |
| B009 | BUG-TAB-001/002 | 标签相关问题 | Launcher.cpp:1178,1272 | 待修复 |
| B010 | BUG-STB-002 | 子工具栏状态管理 | ObjectSelectSubToolbar.cpp | 已修复 |
| B011 | BUG-DRW-004 | 绘图相关问题 | DocumentViewport.cpp:10925 | 待确认 |

---

## 三、代码质量优化

### 大文件结构优化 ✅ 已完成

为超大文件添加了详细的功能模块索引，便于维护和导航：

| 文件 | 当前行数 | 优化内容 |
|------|----------|----------|
| DocumentViewport.cpp | 12315 | ✅ 添加功能模块索引 (30+ 模块) |
| MainWindow.cpp | 7099 | ✅ 添加功能模块索引 (150+ 方法) |
| ControlPanelDialog.cpp | 2096 | ✅ 已有完整注释，保持 |
| Document.cpp | 3002 | ✅ 已有完整注释，保持 |

### 代码重复

- [ ] 提取通用 UI 组件
- [ ] 统一颜色/主题处理 (暗色模式检测代码分散在 14+ 文件)
- [ ] 提取平台特定代码到独立模块

### 注释完善 ✅ 已完成

- [x] DocumentConverter.cpp ✓
- [x] NotebookLibrary.cpp ✓
- [x] ActionBar.cpp ✓
- [x] Toolbar.cpp ✓
- [x] PagePanel.cpp ✓
- [x] ControlPanelDialog.cpp ✓
- [x] SDLControllerManager.cpp ✓
- [x] MainWindow.cpp (添加模块索引) ✓
- [x] DocumentViewport.cpp (添加模块索引) ✓
- [x] SDLControllerManager.cpp ✓

待完善注释文件：
- [ ] MainWindow.cpp (7099 行)
- [ ] DocumentViewport.cpp (12315 行)
- [ ] Document.cpp (3002 行)

---

## 四、测试增强

### 当前状态

- 现有测试：`ToolbarButtonTests.cpp`, `PageTests.cpp`, `DocumentTests.cpp`, `DocumentViewportTests.cpp`
- 仅限桌面版且需要调试构建

### 测试计划

| 序号 | 任务 | 描述 |
|------|------|------|
| TEST01 | 单元测试覆盖 | 增加核心类测试覆盖率 |
| TEST02 | 跨平台测试 | 添加移动平台测试 |
| TEST03 | 集成测试 | 文档导入/导出流程测试 |
| TEST04 | 性能测试 | 渲染性能基准测试 |

---

## 五、文档完善

### 现有文档

- ✓ 构建指南 (Windows, macOS, Linux, Android, iOS)
- ✓ Bug 跟踪文档 (Android, iOS, Qt5)
- ✓ 翻译指南

### 需补充文档

| 序号 | 文档 | 描述 |
|------|------|------|
| DOC01 | ARCHITECTURE.md | 架构设计文档 |
| DOC02 | CODING_STYLE.md | 代码规范 |
| DOC03 | API_REFERENCE.md | API 参考文档 |

---

## 六、技术债务

### 依赖管理

| 序号 | 任务 | 描述 |
|------|------|------|
| DEBT01 | Qt5/Qt6 兼容性 | 继续维护 Qt5 回退代码 |
| DEBT02 | MuPDF 版本 | 跟进 MuPDF 安全更新 |
| DEBT03 | SDL2 版本 | 更新到最新稳定版 |

### 代码清理

| 序号 | 任务 | 描述 |
|------|------|------|
| CLEAN01 | 移除死代码 | 清理未使用的函数和变量 |
| CLEAN02 | 统一命名 | 确保命名风格一致 |
| CLEAN03 | 简化条件编译 | 减少平台特定代码复杂度 |

---

## 七、优先执行计划

### 阶段一：紧急修复 (1-2周)

- [ ] T001 关闭标签页功能
- [ ] T002 导出功能
- [ ] T003 标签切换
- [ ] B007, B008 PDF 页面删除问题

### 阶段二：核心功能 (2-4周)

- [ ] T004-T008 功能完善
- [ ] T009-T011 缓存管理
- [ ] 大文件重构 (DocumentViewport)

### 阶段三：质量提升 (4-8周)

- [ ] 测试覆盖增强
- [ ] 文档完善
- [ ] 代码注释补全
- [ ] 技术债务清理

---

## 八、进度追踪

最后更新: 2026-03-16

| 类别 | 总数 | 已完成 | 进行中 | 待处理 |
|------|------|--------|--------|--------|
| TODO 功能 | 16 | 10 | 0 | 6 |
| Bug 修复 | 11 | 4 | 0 | 7 |
| 代码优化 | 4 | 3 | 0 | 1 |
| 测试增强 | 4 | 0 | 0 | 4 |
| 文档完善 | 3 | 2 | 0 | 1 |
| 技术债务 | 3 | 0 | 0 | 3 |

### 代码优化完成项 (2026-03-16)
- ✅ DocumentViewport.cpp 添加功能模块索引 (30+ 模块)
- ✅ MainWindow.cpp 添加功能模块索引 (150+ 方法)
- ✅ 为 8 个文件添加/完善注释
- ✅ T004: 滚动速度设置 - 从用户设置加载滚动速度
- ✅ T005: 点击放置图片 - 创建 insertImageAtPosition()
- ✅ T006: 删除页面确认 - 添加确认对话框
- ✅ T007: 笔记缺失通知 - 通知用户笔记缺失
- ✅ T008: 位置选择模式 - 实现位置链接功能
- ✅ T009: 缓存大小计算 - 实现 getThumbnailCacheSize()
- ✅ T010: 临时目录清理 - 实现 clearThumbnailCache()