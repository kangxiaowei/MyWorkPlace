// Copyright INTENT 2016 - Current

/**
 * A quick thread pool manager using Boost,
 * adapt from http://progsch.net/wordpress/?p=71
 * Usage:
 *
 * // create a thread pool of 4 worker threads
 * ThreadPool pool(4);
 *
 * // queue a bunch of "work items"
 * for(int i = 0; i < 8; ++i)
 * {
 * pool.enqueue([i]
 * {
 * std::cout << "hello " << i << std::endl;
 * boost::this_thread::sleep(
 *         boost::posix_time::milliseconds(1000)
 * );
 * std::cout << "world " << i << std::endl;
 * });
 * }
 *
 */

#pragma once

#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>

namespace intent {

class ThreadPool;

class Worker {
public:
    Worker(ThreadPool &s) : pool(s) { }

    void operator()();

private:
    ThreadPool &pool;
};

class ThreadPool {
public:
    ThreadPool(size_t);

    template<class F>
    void enqueue(F f) {
        service.post(f);
    };

    ~ThreadPool();

private:
    std::vector<std::unique_ptr<boost::thread>> workers;

    boost::asio::io_service service;
    boost::asio::io_service::work working;

    friend class Worker;
};

} // namespace intent
