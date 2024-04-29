// demo2.cpp，本程序演示基于共享内存的循环队列。
#include "_public.h"

int main()
{
  using ElemType=int;

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

  cout << "元素（1、2、3）入队。\n";
  ee=1;  QQ->push(ee);
  ee=2;  QQ->push(ee);
  ee=3;  QQ->push(ee);

  cout << "队列的长度是" << QQ->size() << endl;
  QQ->printqueue();

  ee=QQ->front(); QQ->pop(); cout << "出队的元素值为" << ee << endl;
  ee=QQ->front(); QQ->pop(); cout << "出队的元素值为" << ee << endl;

  cout << "队列的长度是" << QQ->size() << endl;
  QQ->printqueue();

  cout << "元素（11、12、13、14、15）入队。\n";
  ee=11;  QQ->push(ee);
  ee=12;  QQ->push(ee);
  ee=13;  QQ->push(ee);
  ee=14;  QQ->push(ee);
  ee=15;  QQ->push(ee);

  cout << "队列的长度是" << QQ->size() << endl;
  QQ->printqueue();

  shmdt(QQ);  // 把共享内存从当前进程中分离。
}