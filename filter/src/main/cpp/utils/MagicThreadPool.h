#include <functional>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <condition_variable>
#include <future>

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

namespace std {  //声明std的作用域
#define MAX_THREAD_NUM = 256

    class MagicThreadPool {
    public:
        //内联函数，
        inline MagicThreadPool(unsigned short size = 4) : stoped{false} {
            //线程池大小
            idlThrNum = size < 1 ? 1 : size;
            for (size = 0; size < idlThrNum; ++size) {   //初始化线程数量
                // 只有一次构造函数，不调用拷贝构造函数，速度最快
                pool.emplace_back(
                        [this] { // 工作线程函数
                            while (!this->stoped) {
                                //声明任务
                                std::function<void()> task;
                                {   // 获取一个待执行的 task
                                    std::unique_lock<std::mutex> lock{this->m_lock};// unique_lock 相比 lock_guard 的好处是：可以随时 unlock() 和 lock()
                                    this->cv_task.wait(lock,
                                                       [this] {
                                                           return this->stoped.load() ||
                                                                  !this->tasks.empty();
                                                       }
                                    ); // wait 直到有 task
                                    if (this->stoped && this->tasks.empty())
                                        return false;
                                    task = std::move(this->tasks.front()); // 取一个 task
                                    this->tasks.pop();
                                }
                                idlThrNum--;
                                //运行任务
                                task();
                                idlThrNum++;
                            }
                            return true;
                        }
                );
            }
        }
        //析构函数
        inline ~MagicThreadPool() {
            //停止全部任务
            stoped.store(true);
            cv_task.notify_all(); // 唤醒所有线程执行
            for (std::thread &thread : pool) {
                //thread.detach(); // 让线程“自生自灭”
                if (thread.joinable())
                    thread.join(); // 等待任务结束， 前提：线程一定会执行完
            }
        }

        //模板，需要声明类的类型，自动匹配返回值
        template<class F, class... Args>
        auto commit(F &&f, Args &&... args) -> std::future<decltype(f(args...))> {
            //直接停止任务
            if (stoped.load())    // stop == true ??
                throw std::runtime_error("commit on ThreadPool is stopped.");

            using RetType = decltype(f(
                    args...)); // typename std::result_of<F(Args...)>::type, 函数 f 的返回值类型
            //自动返回值
            auto task = std::make_shared<std::packaged_task<RetType()> >(
                    std::bind(std::forward<F>(f), std::forward<Args>(args)...)
            );    // wtf !
            std::future<RetType> future = task->get_future();
            {    // 添加任务到队列
                std::lock_guard<std::mutex> lock{
                        m_lock};//对当前块的语句加锁  lock_guard 是 mutex 的 stack 封装类，构造的时候 lock()，析构的时候 unlock()
                tasks.emplace( //放入任务到任务池
                        [task]() { // push(Task{...})
                            (*task)();
                        }
                );
            }
            cv_task.notify_one(); // 唤醒一个线程执行

            return future;
        };

        //空闲线程数量
        int idlCount() { return idlThrNum; }

    private:
        using Task = std::function<void()>;
        //线程池
        std::vector<std::thread> pool;
        //任务队列
        std::queue<Task> tasks;
        //同步锁
        std::mutex m_lock;
        std::condition_variable cv_task;
        std::atomic<bool> stoped;
        std::atomic<int> idlThrNum;
    };
}
#endif