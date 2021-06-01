# OpenOasis 变更日志
Open-Oasis，开放的绿洲，一个开放的物理世界机理模型实验项目。  


---------------------------------------------------------------------------
## [v6.2.4]  2015-12-16

### 变更/Changed

* `Node.fn.map()` 之前返回 NodeList 自身，现在将正确地返回被 map 后的数组。

### 修复/Fixed

* 修复在非 ks-debug 模式下仍然输出 `KISSY.log()` 控制台信息的问题。

### 新增/Features

* node 模块增加 API `Node.fn`，以兼容传统 KIMI 的 node 对象扩展机制。 
* ua 模块现在可以识别 Microsoft Edge 浏览器。

### 优化/Refactored

* `KISSY.getScript()` 从 loader 模块中独立出来，io 模块不再依赖 loader 模块。

### 已删除/Removed

* io 模块默认去掉了对 XML 的 converter 支持。

---------------------------------------------------------------------------
