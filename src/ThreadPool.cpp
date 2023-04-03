#include "./include/ThreadPool.h"

ThreadPool::ThreadPool(int size) : stop(false) {
    for (int i = 0; i < size; i++)
    {
        threads.emplace_back(std::thread([this](){
            while (true)
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(tasks_mtx);
                    cv.wait(lock,[this](){ // 抢夺访问任务队列的锁
                        return stop || !tasks.empty();
                    });
                    if (stop && tasks.empty())
                        return;
                    task = tasks.front();
                    tasks.pop();
                }
                task(); // 执行任务
            }
        }));
    }
    
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);
        stop = true;
    }
    cv.notify_all(); // 通知所有条件变量我要终止
    for (std::thread &th : threads)
    {
        if(th.joinable())
            th.join();
    }
    
}

// 讲事件填入到任务队列中
// void ThreadPoll::add(std::function<void()> func) {
//     {
//         std::unique_lock<std::mutex> lock(tasks_mtx);
//         // 防止在读任务队列时放入, 直到读取完才放入
//         if(stop)
//             throw std::runtime_error("ThreadPoll already stop, can't add task any more");
//         tasks.emplace(func);
//     }
//     cv.notify_one(); // 提醒一次条件变量
// }