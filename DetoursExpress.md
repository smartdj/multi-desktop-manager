# Introduction #

Search Detours in Google.


# Details #

---- Detours是微软开发的一个函数库（源代码可在http://research.microsoft.com/sn/detours 免费获得）, 用于修改运行中的程序在内存中的影像，从而即使没有源代码也能改变程序的行为。具体用途是：

拦截WIN32 API调用，将其引导到自己的子程序，从而实现WIN32 API的定制。
为一个已在运行的进程创建一新线程，装入自己的代码并运行。
---- 本文将简介Detours的原理，Detours库函数的用法， 并利用Detours库函数在Windows NT上编写了一个程序，该程序能使有“调试程序”的用户权限的用户成为系统管理员，附录利用Detours库函数修改该程序使普通用户即可成为系统管理员（在NT4 SP3上）。

一． Detours的原理

---- 1． WIN32进程的内存管理

---- 总所周知，WINDOWS NT实现了虚拟存储器，每一WIN32进程拥有4GB的虚存空间， 关于WIN32进程的虚存结构及其操作的具体细节请参阅WIN32 API手册， 以下仅指出与Detours相关的几点：

---- (1) 进程要执行的指令也放在虚存空间中
---- (2) 可以使用QueryProtectEx函数把存放指令的页面的权限更改为可读可写可执行，再改写其内容，从而修改正在运行的程序
---- (3) 可以使用VirtualAllocEx从一个进程为另一正运行的进程分配虚存，再使用 QueryProtectEx函数把页面的权限更改为可读可写可执行，并把要执行的指令以二进制机器码的形式写入，从而为一个正在运行的进程注入任意的代码

---- 2． 拦截WIN32 API的原理

---- Detours定义了三个概念：

---- (1) Target函数：要拦截的函数，通常为Windows的API。
---- (2) Trampoline函数：Target函数的复制品。因为Detours将会改写Target函数，所以先把Target函数复制保存好，一方面仍然保存Target函数的过程调用语义，另一方面便于以后的恢复。
---- (3) Detour 函数：用来替代Target函数的函数。

---- Detours在Target函数的开头加入JMP Address\_of_Detour_ Function指令（共5个字节）把对Target函数的调用引导到自己的Detour函数， 把Target函数的开头的5个字节加上JMP Address\_of_Target_ Function+5作为Trampoline函数。
---- 3． 为一个已在运行的进程装入一个DLL

---- 以下是其步骤：

---- (1) 创建一个ThreadFuction,内容仅是调用LoadLibrary。
---- (2) 用VirtualAllocEx为一个已在运行的进程分配一片虚存，并把权限更改为可读可写可执行。
---- (3) 把ThreadFuction的二进制机器码写入这片虚存。
---- (4) 用CreateRemoteThread在该进程上创建一个线程，传入前面分配的虚存的起始地址作为线程函数的地址，即可为一个已在运行的进程装入一个DLL。通过DllMain 即可在一个已在运行的进程中运行自己的代码。

二． Detours库函数的用法

---- 因为Detours软件包并没有附带帮助文件，以下接口仅从剖析源代码得出。

---- 1． PBYTE WINAPI DetourFindFunction(PCHAR pszModule, PCHAR pszFunction)

---- 功能：从一DLL中找出一函数的入口地址
---- 参数：pszModule是DLL名,pszFunction是函数名。
---- 返回：名为pszModule的DLL的名为pszFunction的函数的入口地址
---- 说明：DetourFindFunction除使用GetProcAddress外，还直接分析DLL的文件头，因此可以找到一些GetProcAddress找不到的函数入口。

---- 2． DETOUR\_TRAMPOLINE(trampoline\_prototype, target\_name)
---- 功能：该宏把名为target\_name 的Target函数生成Trampoline函数，以后调用 trampoline\_prototype在语义上等于调用Target函数。

---- 3． BOOL WINAPI DetourFunctionWithTrampoline(PBYTE pbTrampoline, BYTE pbDetour)
---- 功能：用Detour 函数拦截Target函数
---- 参数：pbTrampoline是DETOUR\_TRAMPOLINE得到的trampoline\_prototype，pbDetour是 Detour 函数的入口地址。

---- 4． BOOL WINAPI DetourRemoveWithTrampoline(PBYTE pbTrampoline,PBYTE pbDetour)
---- 功能：恢复Target函数
---- 参数：pbTrampoline是DETOUR\_TRAMPOLINE得到的trampoline\_prototype，pbDetour是 Detour 函数的入口地址。

---- 5． BOOL WINAPI ContinueProcessWithDll(HANDLE hProcess, LPCSTR lpDllName)
---- 功能：为一个已在运行的进程装入一个DLL
---- 参数：hProcess是进程的句柄，lpDllName是要装入的DLL名

三． 结论 ---- Detours是一强大的工具，提供了简单易用的函数接口来拦截WIN32 API调用和为一个已在运行的进程装入一个DLL。