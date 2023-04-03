#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

class ThreadPool
{
private:
    std::vector<std::thread> threads;
    // 任务队列
    std::queue<std::function<void()>> tasks;
    std::mutex tasks_mtx;
    std::condition_variable cv;
    bool stop;
public:
    ThreadPool(int size = 10);
    ~ThreadPool();
    // 讲事件填入到任务队列中
    // void add(std::function<void()>);
    template<class F, class... Args>
    auto add(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>;
    // result_of 自动推导返回值 ->表示返回值
};

template<class F, class... Args>
    auto ThreadPool::add(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type> {
    // 返回值类型别名为return_type
    using return_type = typename std::result_of<F(Args...)>::type;

    // std::packaged_task将对象进行包装
    // 此处的task算是一个函数指针, 即 '返回类型'(*f)(Args&&... args)
    // 例子: std::packaged_task<int(int, int)> t(task);   t(1, 2);
    auto task = std::make_shared<std::packaged_task<return_type()> > ( //使用智能指针
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)  //完美转发参数
    );
    // future 保存线程的运行结果(函数指针), 并在想要的时候获取(即代替原本存储异步操作结果的全局变量)
    std::future<return_type> res = task->get_future();   // 等待未来task执行后的结果
    {
        // 队列锁作用域
        std::unique_lock<std::mutex> lock(tasks_mtx);

        if(stop)
            throw std::runtime_error("enqueue on stoped ThreadPool");

        // 添入任务队列, 这样就直接将函数指针存入
        tasks.emplace([task](){
            (*task)();
        });
    }
    cv.notify_one(); // 通知已经完成存入
    return res; // 通过返回值我们就可以做错误处理
}

/**
 * 笔记
 * std::promise<return_type>承诺必须运算后的值
 * 
 */