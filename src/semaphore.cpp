#include <mutex>
#include <thread>

#include "semaphore.hpp"

namespace pbma
{

Semaphore::Semaphore(std::size_t initial_counter) noexcept : counter_{initial_counter}
{
}

void Semaphore::release() noexcept
{
    // std::lock_guard lock_protection{mutex_}; // Automatically unlocks the mutex at the end of scope
    std::scoped_lock lock_protection{mutex_};
    ++counter_;
    condition_.notify_one(); // Notify any sleeping thread
}

void Semaphore::acquire() noexcept
{
    std::unique_lock<std::mutex> lock_protection{mutex_}; // Automatically unlocks the mutex at the end of scope

    /*
    If the condition is not satisfied, the thread is blocked until it's notified
    by a call of notify_one() or notify_all(). When the blocked thread is notified,
    the thread is waken up and the condition is checked again. If the condition
    still isn't satisfied, the thread is blocked again.
    */
    condition_.wait(lock_protection, [&counter = counter_] { return counter > 0; });
    --counter_;
}

} // namespace pbma