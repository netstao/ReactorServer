//shmget(key_t key, size_t size, int shmflg);ip  //共享内存

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
using  namespace std;

struct stgirl     // 超女结构体。
{
  int  no;        // 编号。
  char name[51];  // 姓名，注意，不能用string。

  //string name;  // 不可用string 因为string 分配的内存在堆区 无法用shmget
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

  // 第3步：使用共享内存，对共享内存进行读/写。
  cout << "原值：no=" << ptr->no << ",name=" << ptr->name << endl;  // 显示共享内存中的原值。
  ptr->no=atoi(argv[1]);        // 对超女结构体的no成员赋值。
  strcpy(ptr->name,argv[2]);    // 对超女结构体的name成员赋值。
//   ptr->name=argv[2];
  cout << "新值：no=" << ptr->no << ",name=" << ptr->name << endl;  // 显示共享内存中的当前值。

  // 第4步：把共享内存从当前进程中分离。
  shmdt(ptr);

  // 第5步：删除共享内存。
  //if (shmctl(shmid,IPC_RMID,0)==-1)
  //{
   // cout << "shmctl failed\n"; return -1;
  //}
}