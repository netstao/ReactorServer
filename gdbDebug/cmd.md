ulimit -a 

ulimit -c unlimited  # codeduamp文件打开调试

gdb demo core.2313  # gdb 程序名 core文件名

bt #打印函数调用栈

n # 下一步   vs F10
c # 继续     vs F5
r # 运行 
s # 执行当前语句 vs F11
b line #b 行号 断点
p var # 打印 变量的值

set args a1 a2 # 设置参数

set var  变量名 # 手动改变比变量名

p 变量名=1 # 手动改名变量名

q # 退出gdb


#调试正在运行中程序
gdb  demo pid  # gdb 程序名 pid  被调试的程序会被暂停



