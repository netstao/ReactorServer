#include "_public.h"

// 如果信号量已存在，获取信号量；如果信号量不存在，则创建它并初始化为value。
// 如果用于互斥锁，value填1，sem_flg填SEM_UNDO。
// 如果用于生产消费者模型，value填0，sem_flg填0。
bool csemp::init(key_t key,unsigned short value,short sem_flg)
{
  if (m_semid!=-1) return false; // 如果已经初始化了，不必再次初始化。

  m_sem_flg=sem_flg;

  // 信号量的初始化不能直接用semget(key,1,0666|IPC_CREAT)
  // 因为信号量创建后，初始值是0，如果用于互斥锁，需要把它的初始值设置为1，
  // 而获取信号量则不需要设置初始值，所以，创建信号量和获取信号量的流程不同。

  // 信号量的初始化分三个步骤：
  // 1）获取信号量，如果成功，函数返回。
  // 2）如果失败，则创建信号量。
  // 3) 设置信号量的初始值。

  // 获取信号量。
  if ( (m_semid=semget(key,1,0666)) == -1)
  {
    // 如果信号量不存在，创建它。
    if (errno==ENOENT)
    {
      // 用IPC_EXCL标志确保只有一个进程创建并初始化信号量，其它进程只能获取。
      if ( (m_semid=semget(key,1,0666|IPC_CREAT|IPC_EXCL)) == -1)
      {
        if (errno==EEXIST) // 如果错误代码是信号量已存在，则再次获取信号量。
        {
          if ( (m_semid=semget(key,1,0666)) == -1)
          { 
            perror("init 1 semget()"); return false; 
          }
          return true;
        }
        else  // 如果是其它错误，返回失败。
        {
          perror("init 2 semget()"); return false;
        }
      }

      // 信号量创建成功后，还需要把它初始化成value。
      union semun sem_union;
      sem_union.val = value;   // 设置信号量的初始值。
      if (semctl(m_semid,0,SETVAL,sem_union) <  0) 
      { 
        perror("init semctl()"); return false; 
      }
    }
    else
    { perror("init 3 semget()"); return false; }
  }

  return true;
}

// 信号量的P操作（把信号量的值减value），如果信号量的值是0，将阻塞等待，直到信号量的值大于0。
bool csemp::wait(short value)
{
  if (m_semid==-1) return false;

  struct sembuf sem_b;
  sem_b.sem_num = 0;      // 信号量编号，0代表第一个信号量。
  sem_b.sem_op = value;   // P操作的value必须小于0。
  sem_b.sem_flg = m_sem_flg;
  if (semop(m_semid,&sem_b,1) == -1) { perror("p semop()"); return false; }

  return true;
}

// 信号量的V操作（把信号量的值减value）。
bool csemp::post(short value)
{
  if (m_semid==-1) return false;

  struct sembuf sem_b;
  sem_b.sem_num = 0;     // 信号量编号，0代表第一个信号量。
  sem_b.sem_op = value;  // V操作的value必须大于0。
  sem_b.sem_flg = m_sem_flg;
  if (semop(m_semid,&sem_b,1) == -1) { perror("V semop()"); return false; }

  return true;
}

// 获取信号量的值，成功返回信号量的值，失败返回-1。
int csemp::getvalue()
{
  return semctl(m_semid,0,GETVAL);
}

// 销毁信号量。
bool csemp::destroy()
{
  if (m_semid==-1) return false;

  if (semctl(m_semid,0,IPC_RMID) == -1) { perror("destroy semctl()"); return false; }

  return true;
}

csemp::~csemp()
{
}