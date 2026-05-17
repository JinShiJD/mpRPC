#pragma once
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

// 异步写日志的日志队列
template <typename T>
class LockQueue
{
public:
    // 多个worker线程都会写日志queue
    void push(const T &data)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_q.push(data);
        m_cVariable.notify_one();
    }

    T Pop()
    {
        std::unique_lock<std::mutex> lock(m_mtx);
        while(m_q.empty())
        {
            // 日志队列为空， 线程进入wait()状态
            m_cVariable.wait(lock);
        }

        T data = m_q.front();
        m_q.pop();
        return data;
    }

private:
    std::queue<T> m_q;
    std::mutex m_mtx;
    std::condition_variable m_cVariable;
};