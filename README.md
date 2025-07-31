之前太多东西没写，基本索性从这里开始

引擎更新日志

2025/07/25

working on 重构 windows 部分， 我想对引擎的UI下手，发现当前的window part 过于耦合，在Engine.cpp 创建与Loop几乎强绑定，我想做更改

目前已经完成了window.h windowswindow.h, 对Engine part做了一部分的解耦

2025/07/30

重构过程中尝试添加ImGUui, 之后在试图画Gizmos的时候发觉目前引擎耦合还是太严重，继续重构
目前分出来了Application, Window, EngineLayer, ImGuiLayer 多个层级，同时也提供添加更多层级，用PCSTree做了一个Manager，支持按照order排序，目前还没做ReOrder

重构完成，引擎的渲染，逻辑，ui三个层级都分离解耦，也恢复了之前的功能，可以开始正式进行UI的制作

2025/07/31

按照优先度 Shader Material 排序 渲染，减少GPU层面Shdaer的切换来进行优化
