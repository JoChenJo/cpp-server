#pragma once
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <memory>
#include <utility>
#include "Macros.h"

class ThreadPool
{
public:
    ThreadPool(int size = std::thread::hardware_concurrency());
    ~ThreadPool();
    DISALLOW_COPY_AND_MOVE(ThreadPool);

    template<typename F, typename... Args>
    auto add(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>;
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex tasks_mtx;
    std::condition_variable cv;
    std::atomic<bool> stop;
};

// void ThreadPool::add(std::function<void()> func){
//     {
//         std::unique_lock<std::mutex> lock(tasks_mtx);
//         if(stop)
//             throw std::runtime_error("ThreadPool already stop, can't add task any more");
//         tasks.emplace(func);
//     }
//     cv.notify_one();
// }

template<typename F, typename... Args>
auto ThreadPool::add(F&& f, Args&&... args) ->std::future<typename std::result_of<F(Args...)>::type>{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()>>(
        std::bind(std::forward(f), std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);

        if(stop){
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }
        tasks.emplace([task](){(*task)()});
    }
    cv.notify_one();
    return res;
}