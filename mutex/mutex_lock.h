class mtx_t
{
private:
  pthread_mutex_t mutex;
public:
  mtx_t() {
    pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
    mutex = mutex2;
  }
  void lock() { pthread_mutex_lock(&mutex); }
  void unlock() { pthread_mutex_unlock(&mutex); }
};

class lock_t
{
private:
  mtx_t &mtx;
public:
  lock_t(mtx_t &_mtx) : mtx(_mtx) { mtx.lock(); }
  ~lock_t() { mtx.unlock(); }
};

