// demo3.cpp，本程序演示用信号量给共享内存加锁。
#include "_public.h"

struct stgirl     // 超女结构体。
{
  int  no;        // 编号。
  char name[51];  // 姓名，注意，不能用string。
};

int main(int argc,char *argv[])
{
  if (argc!=3) { cout << "Using:./demo no name\n"; return -1; }

  // 第1步：创建/获取共享内存，键值key为0x5005，也可以用其它的值。
  int shmid=shmget(0x5005, sizeof(stgirl), 0640|IPC_CREAT);
  if ( shmid ==-1 )
  { 
    cout << "shmget(0x5005) failed.\n"; return -1; 
  }

  cout << "shmid=" << shmid << endl;

  // 第2步：把共享内存连接到当前进程的地址空间。
  stgirl *ptr=(stgirl *)shmat(shmid,0,0);
  if ( ptr==(void *)-1 )
  { 
    cout << "shmat() failed\n"; return -1; 
  }

  // 创建、初始化二元信号量。
  csemp mutex;
  if (mutex.init(0x50089)==false)
  {
    cout << "mutex.init(0x5005) failed.\n"; return -1;
  }

  cout << "申请加锁...\n";
  mutex.wait(); // 申请加锁。
  cout << "申请加锁成功。\n";

  // 第3步：使用共享内存，对共享内存进行读/写。
  cout << "原值：no=" << ptr->no << ",name=" << ptr->name << endl;  // 显示共享内存中的原值。
  ptr->no=atoi(argv[1]);        // 对超女结构体的no成员赋值。
  strcpy(ptr->name,argv[2]);    // 对超女结构体的name成员赋值。
  cout << "新值：no=" << ptr->no << ",name=" << ptr->name << endl;  // 显示共享内存中的当前值。
  sleep(10);

  mutex.post(); // 解锁。
  cout << "解锁。\n";

  // 查看信号量  ：ipcs -s    // 删除信号量  ：ipcrm sem 信号量id
  // 查看共享内存：ipcs -m    // 删除共享内存：ipcrm -m  共享内存id

  // 第4步：把共享内存从当前进程中分离。
  shmdt(ptr);

  // 第5步：删除共享内存。
  //if (shmctl(shmid,IPC_RMID,0)==-1)
  //{ 
   // cout << "shmctl failed\n"; return -1; 
  //}
}