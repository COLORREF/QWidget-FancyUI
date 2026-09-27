#ifndef MINI_BLUR_WORKERS_H
#define MINI_BLUR_WORKERS_H
#include <condition_variable>
#include <exception>
#include <mutex>
#include <thread>
#include <vector>


namespace mini_blur::detail
{
    // Context-owned pool. Jobs finish before run returns; caller is worker 0.
    class Workers
    {
        std::vector<std::thread> workers_;
        std::mutex mutex_;
        std::condition_variable ready_, done_;
        void *jobData_ = nullptr;

        void (*jobFunction_)(void *, unsigned) = nullptr;

        std::exception_ptr error_;
        unsigned generation_ = 0, pending_ = 0;
        bool stop_ = false;

        void execute(unsigned id)
        {
            try
            {
                jobFunction_(jobData_, id);
            }
            catch (...)
            {
                std::lock_guard<std::mutex> lock(mutex_);
                if (!error_)
                    error_ = std::current_exception();
            }
        }

    public:
        explicit Workers(unsigned count)
        {
            try
            {
                for (unsigned id = 1; id < count; ++id)
                    workers_.emplace_back([this, id] {
                        unsigned seen = 0;
                        std::unique_lock<std::mutex> lock(mutex_);
                        for (;;)
                        {
                            ready_.wait(lock, [&] { return stop_ || generation_ != seen; });
                            if (stop_)
                                return;
                            seen = generation_;
                            lock.unlock();
                            execute(id);
                            lock.lock();
                            if (--pending_ == 0)
                                done_.notify_one();
                        }
                    });
            }
            catch (...)
            {
                shutdown();
                throw;
            }
        }

        ~Workers() { shutdown(); }

        void shutdown()
        {
            {
                std::lock_guard<std::mutex> lock(mutex_);
                stop_ = true;
            }
            ready_.notify_all();
            for (auto &t: workers_)
                if (t.joinable())
                    t.join();
        }

        [[nodiscard]] unsigned count() const { return static_cast<unsigned>(workers_.size()) + 1; }

        template<class F>
        void run(F job)
        {
            if (workers_.empty())
            {
                job(0);
                return;
            }
            {
                std::lock_guard<std::mutex> lock(mutex_);
                jobData_ = &job;
                jobFunction_ = [](void *data, unsigned id) { (*static_cast<F *>(data))(id); };
                error_ = nullptr;
                pending_ = static_cast<unsigned>(workers_.size());
                ++generation_;
            }
            ready_.notify_all();
            execute(0);
            std::unique_lock<std::mutex> lock(mutex_);
            done_.wait(lock, [&] { return pending_ == 0; });
            jobData_ = nullptr;
            jobFunction_ = nullptr;
            if (error_)
                std::rethrow_exception(error_);
        }
    };
} // namespace mini_blur::detail

#endif // MINI_BLUR_WORKERS_H
