<!--
 * @Date        : 2020-05-13 16:31:06
 * @LastEditors : anlzou
 * @Github      : https://github.com/anlzou
 * @LastEditTime: 2020-05-14 16:35:31
 * @FilePath    : \WinApi_Server\README.md
 * @Describe    : 
 -->
# winAPI-server
## 版本
> ### WinApi_Server

VS 2019 平台

> ### win32api_server_vc6

VC 6.0 平台

## 更新日志
### WinApi_Server
> #### 2020年5月13日
- 在VS 2019中编译成功，没有报错，运行过程不可以与客户端通信，原因是代码使用的是vc++ 6.0中修改后的代码，与VS 2019未调试成功。

### win32api_server_vc6
> #### 2020年5月14日
- 在vc++ 6.0中编译成功，没有报错，运行过程可以短时间与客户端通信，但是程序会崩溃，应该是不兼容W10系统的原因。