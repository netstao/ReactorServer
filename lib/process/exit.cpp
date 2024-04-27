#退出方式
exit()
return;
_exit()  #不执行清理工作
_Exit()  #不执行清理工作
pthread_exit()

#异常终止
abort()
signal()

return #会调用局部析构函数和全局析构函数

exit() #不会调用局部析构函数，只调用全局析构函数

atexit() #登记进程终止函数  
exit()  #调用终止函数的顺序与登记时相反 作用 进程退出前的收尾工作
