#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include <condition_variable>
#include <mutex>

namespace pbma
{

class Semaphore
{
public:
    explicit Semaphore(std::size_t initial_counter) noexcept;

    // Release resources on the critical region
    void release() noexcept;

    // Acquire resources on the critical region
    void acquire() noexcept;

private:
    std::size_t counter_;
    std::mutex mutex_;
    std::condition_variable condition_;
};

} // namespace pbma

#endif // SEMAPHORE_HPP