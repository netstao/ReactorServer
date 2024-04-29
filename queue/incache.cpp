// 多进程的生产消费者模型的生产者程序
#include "_public.h"

int main()
{
  struct stgirl  // 循环队列的数据元素是超女结构体。
  {
    int no;
    char name[51];
  };

  using ElemType=stgirl;

  // 初始化共享内存。
  int shmid=shmget(0x5005, sizeof(squeue<ElemType,5>), 0640|IPC_CREAT);
  if ( shmid ==-1 )
  {
    cout << "shmget(0x5005) failed.\n"; return -1;
  }

  // 把共享内存连接到当前进程的地址空间。
  squeue<ElemType,5> *QQ=(squeue<ElemType,5> *)shmat(shmid,0,0);
  if ( QQ==(void *)-1 )
  {
    cout << "shmat() failed\n"; return -1;
  }

  QQ->init();       // 初始化循环队列。

  ElemType ee;      // 创建一个数据元素。

  csemp mutex; mutex.init(0x5001);     // 用于给共享内存加锁。
  csemp cond;  cond.init(0x5002,0,0);  // 信号量的值用于表示队列中数据元素的个数。

  mutex.wait();  // 加锁。
  // 生产3个数据。
  ee.no=3; strcpy(ee.name,"西施"); QQ->push(ee);
  ee.no=7; strcpy(ee.name,"冰冰"); QQ->push(ee);
  ee.no=8; strcpy(ee.name,"幂幂"); QQ->push(ee);
  mutex.post();  // 解锁。
  cond.post(3);  // 实参是3，表示生产了3个数据。

  shmdt(QQ);  // 把共享内存从当前进程中分离。
}