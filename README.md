> **基于 muduo + Protobuf + ZooKeeper 的 C++ 分布式 RPC 框架**
>
> **平台工具**：Linux（Ubuntu/CentOS）、VSCode 远程开发、CMake 构建系统、GDB 调试、Shell 自动化编译脚本（autobuild.sh）
>
> **项目内容**：
>
> 1. **基于 muduo 网络库的高并发 RPC 通信**：使用 muduo 的 Reactor 多线程模型（`setThreadNum(4)`），将网络 I/O 与业务逻辑分离，通过 `setConnectionCallback` 和 `setMessageCallback` 绑定回调，实现非阻塞高并发 RPC 服务
>
> 2. **Protobuf 服务定义与序列化**：使用 Protobuf 定义 RPC 服务接口（`service`）和请求/响应消息（`message`），通过 `cc_generic_services = true` 自动生成 Service/Stub 基类；Provider 端重写虚函数实现业务逻辑，Consumer 端通过 Stub 代理对象发起远程调用
>
> 3. **自定义 RPC 协议设计**：设计 `RpcHeader` 消息（含 service_name、method_name、args_size），协议格式为 `header_size(4字节定长) + RpcHeader序列化 + args序列化`；接收端先读取 4 字节 header_size，再按长度读取完整数据，解决 TCP 粘包问题
>
> 4. **ZooKeeper 分布式服务注册与发现**：封装 ZkClient 类，基于 ZooKeeper C API 实现服务注册（`zoo_create`）与服务发现（`zoo_get`）；Provider 将 service_name 注册为永久 ZNode，method_name 注册为临时 ZNode（存储 IP:Port），Consumer 通过路径 `/service_name/method_name` 动态获取服务地址，实现分布式服务治理
>
> 5. **异步日志系统**：实现 LockQueue 线程安全队列（`std::mutex` + `std::condition_variable`），采用生产者-消费者模式；Logger 为单例模式，构造时启动独立写日志线程（`detach`），按日期自动分文件（`YYYY-M-D-log.txt`），日志含时间戳和级别标记（INFO/ERROR），通过宏 `LOG_INFO`/`LOG_ERR` 简化调用
>
> 6. **MprpcChannel 统一调用通道**：继承 `google::protobuf::RpcChannel`，重写 `CallMethod` 集中处理所有 RPC 方法的参数序列化、网络发送、响应接收与反序列化；使用原生 socket API（`socket/connect/send/recv`）实现 TCP 短连接通信
>
> 7. **MprpcController 调用控制**：继承 `google::protobuf::RpcController`，管理 RPC 调用的失败状态与错误信息，在 socket 创建、连接、发送、接收、解析等各环节捕获错误并设置失败原因
>
> 8. **MprpcConfig 配置管理**：实现配置文件解析器，支持 `key=value` 格式、`#` 注释行过滤、前后空格 Trim；通过 `getopt` 解析命令行参数 `-i <configfile>` 指定配置文件路径
>
> 9. **MprpcApplication 框架初始化**：单例模式（静态局部变量 + 删除拷贝/移动构造），统一管理框架初始化流程（命令行解析 → 配置加载），对外暴露 `GetInstance()` 和 `GetConfig()` 接口
>
> 10. **CMake 模块化构建**：项目分为 `src`（框架核心库 `libmprpc.a`）和 `example`（示例代码）两个子目录，链接 muduo、Protobuf、ZooKeeper、pthread 等依赖库；编写 `autobuild.sh` 一键编译脚本

---
