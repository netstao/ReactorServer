// 多进程的生产消费者模型的消费者程序
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

  while (true)
  {
    mutex.wait();  // 加锁。

    while (QQ->empty())    // 如果队列空，进入循环，否则直接处理数据。必须用循环，不能用if
    {
      mutex.post();   // 解锁。
      cond.wait();    // 等待生产者的唤醒信号。
      mutex.wait();   // 加锁。
    }

    // 数据元素出队。
    ee = QQ->front();  QQ->pop();
    mutex.post(); // 解锁。

    // 处理出队的数据（把数据消费掉）。
    cout << "no=" << ee.no << ",name=" << ee.name << endl;
    usleep(100);    // 假设处理数据需要时间，方便演示。
  }

  shmdt(QQ);
}