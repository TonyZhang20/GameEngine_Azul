之前太多东西没写，基本索性从这里开始

引擎更新日志 - 主要更新在workingOn branch, Main很长周期才会做一次更新

2025/07/25

working on 重构 windows 部分， 我想对引擎的UI下手，发现当前的window part 过于耦合，在Engine.cpp 创建与Loop几乎强绑定，我想做更改

目前已经完成了window.h windowswindow.h, 对Engine part做了一部分的解耦

2025/07/30

重构过程中尝试添加ImGUui, 之后在试图画Gizmos的时候发觉目前引擎耦合还是太严重，继续重构
目前分出来了Application, Window, EngineLayer, ImGuiLayer 多个层级，同时也提供添加更多层级，用PCSTree做了一个Manager，支持按照order排序，目前还没做ReOrder

初步重构完成，引擎的渲染，逻辑，ui三个层级都分离解耦，也恢复了之前的功能

2025/07/31

按照优先度 Shader Material 排序 渲染，减少GPU层面Shdaer的切换来进行优化

2025/08/05

重新定向 back buffer 复制渲染 到 离屏texture （偷懒）

目前ZVector 与 ZHashMap 在大量数据的情况下表现优与 stl::vector stl::unorderedMap, 应该更适用于ECS的情况

完成自己的STL vector 和 unorder_map, 准备开始 ECS 的 实现与重构

2025/08/16
创建了Sparse Set 和 Archetype 的 Ecs 框架，开始重构

2025/08/17
硬盘挂了一天白干

2025/08/20
逻辑与渲染完全解耦，Ecs重构初步完成

2025/08/27
今天主要working on 编辑器
现在可以添加/移除 Entity / Component -> 适配了 Transform/ Mesh/ Material Components

2025/12/18
把上课后学的优化版本的数学库加入了引擎
把Scene部分集成到MainScene做一个代码隔离

2025/12/19 
把protobuf加入到了引擎里面

