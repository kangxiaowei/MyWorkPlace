#include "ThreadPool.h"

namespace intent {

void Worker::operator()() { pool.service.run(); }

ThreadPool::ThreadPool(size_t threads) : working(service) {
    for (size_t i = 0; i < threads; ++i) {
        workers.push_back(
            std::unique_ptr<boost::thread>(
                new boost::thread(Worker(*this))
            )
        );
    }
}

ThreadPool::~ThreadPool() {
    service.stop();
    for (size_t i = 0; i < workers.size(); ++i) {
        workers[i]->interrupt();
        workers[i]->join();
    }
}

} // namespace intent
