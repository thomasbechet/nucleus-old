#include <nucleus/module/task/thread_pool.hpp>

using namespace nucleus;

thread_pool_t::~thread_pool_t()
{
    if (m_running) stop();
}

void thread_pool_t::start(unsigned workerCount) noexcept
{
    if (!m_running)
    {
        m_running = true;
        m_next_worker = 0;
        m_job_count.store(0, std::memory_order_relaxed);

        uint32_t proc_count = (workerCount == 0) ? std::thread::hardware_concurrency() : workerCount;
        proc_count = (proc_count > 0) ? proc_count : 1;

        /* create queues */
        for (uint32_t i = 0; i < proc_count; i++)
            m_queues.emplace_back(std::make_unique<rigtorp::MPMCQueue<nu_task_job_t>>(25));
        /* create workers */
        for (uint32_t i = 0; i < proc_count; i++)
            m_workers.emplace_back(std::make_unique<std::thread>(&thread_pool_t::worker_main, this, i));

        nu_info(("[THREADPOOL] " + std::to_string(proc_count) + " workers created.\n").c_str());
    }
}
void thread_pool_t::stop() noexcept
{
    if (m_running)
    {
        m_running.store(false, std::memory_order_relaxed);
        for (auto& thread : m_workers)
            thread->join();
        
        uint32_t worker_count = m_workers.size();
        m_workers.clear();
        nu_info(("[THREADPOOL] " + std::to_string(worker_count) + " workers destroyed.\n").c_str());

        m_running = false;
    }
}

nu_task_handle_t thread_pool_t::create_task() noexcept
{
    return 0;
}
void thread_pool_t::perform(nu_task_handle_t task, const nu_task_job_t *jobs, uint32_t count)
{
    for (uint32_t i = 0; i < count; i++)
    {
        m_queues.at(m_next_worker)->push(jobs[i]);
        m_job_count.fetch_add(1, std::memory_order_relaxed);
        m_next_worker = (m_next_worker + 1) % m_queues.size();
    }
}
void thread_pool_t::wait_task(nu_task_handle_t task)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    while (m_job_count.load(std::memory_order_relaxed) > 0)
        m_cv_ended.wait(lock);
}

void thread_pool_t::worker_main(uint32_t id) noexcept
{
    bool was_working = false;

    nu_task_job_t job;
    while (m_running.load(std::memory_order_relaxed))
    {
        bool find = false;
        find = m_queues.at(id)->try_pop(job);
        for (uint32_t i = 0; !find && i < m_queues.size() && i != id; i++)
            find = m_queues.at(id)->try_pop(job); 

        if (find)
        {
            job.func(job.args, 0, 0);
            m_job_count.fetch_sub(1, std::memory_order_relaxed);
        }
        else
        {
            if (was_working) m_cv_ended.notify_one();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        was_working = find;
    }
}