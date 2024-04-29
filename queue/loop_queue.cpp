// demo1.cpp，本程序演示循环队列的使用。
#include "_public.h"

int main()
{
  using ElemType=int;

  squeue<ElemType,5> QQ;

  ElemType ee;      // 创建一个数据元素。

  cout << "元素（1、2、3）入队。\n";
  ee=1;  QQ.push(ee);
  ee=2;  QQ.push(ee);
  ee=3;  QQ.push(ee);

  cout << "队列的长度是" << QQ.size() << endl;
  QQ.printqueue();

  ee=QQ.front(); QQ.pop(); cout << "出队的元素值为" << ee << endl;
  ee=QQ.front(); QQ.pop(); cout << "出队的元素值为" << ee << endl;

  cout << "队列的长度是" << QQ.size() << endl;
  QQ.printqueue();

  cout << "元素（11、12、13、14、15）入队。\n";
  ee=11;  QQ.push(ee);
  ee=12;  QQ.push(ee);
  ee=13;  QQ.push(ee);
  ee=14;  QQ.push(ee);
  ee=15;  QQ.push(ee);

  cout << "队列的长度是" << QQ.size() << endl;
  QQ.printqueue();
}