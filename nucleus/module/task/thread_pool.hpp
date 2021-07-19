#pragma once

#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <functional>

#include <nucleus/nucleus.hpp>

namespace nucleus
{
    class thread_pool_t
    {
    public:
        ~thread_pool_t();

        void start(unsigned workerCount) noexcept;
        void stop() noexcept;

        nu_task_t create_task() noexcept;
        void perform(nu_task_t task, const nu_task_job_t *jobs, uint32_t count);
        void wait_task(nu_task_t task);

    private:
        void worker_main(uint32_t id) noexcept;

    private:
        /* context */
        uint32_t m_task_count = 0;
        std::atomic_bool m_running;
        std::atomic_uint m_job_count;

        /* workers */
        // std::vector<std::unique_ptr<rigtorp::MPMCQueue<nu_task_job_t>>> m_queues;
        std::vector<std::unique_ptr<std::thread>> m_workers;
        uint32_t m_next_worker;

        /* condition variable */
        std::condition_variable m_cv_ended;
        std::mutex m_mutex;
    };
}

