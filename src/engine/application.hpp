#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <atomic>
#include <thread>

#include "semaphore.hpp"

namespace pbma
{

class Application
{
public:
    Application() = default;
    virtual ~Application() = default;

    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    void run();
    bool is_running() const;
    bool is_animating() const;
    void start_animation();
    void pause_animation();

    virtual void set_polyscope_settings();
    virtual void physics_update() = 0;
    virtual void pre_draw() = 0;

private:
    void main_loop();
    void update();
    void user_callback();
    void shutdown();

    std::atomic<bool> running_{true};
    std::atomic<bool> animate_{false};
    pbma::Semaphore empty_{1};
    pbma::Semaphore full_{0};
    pbma::Semaphore mutex_{1};
};

} // namespace pbma

#endif // APPLICATION_HPP