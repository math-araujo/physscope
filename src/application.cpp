#include <atomic>
#include <iostream>
#include <polyscope/polyscope.h>
#include <thread>

#include "application.hpp"
#include "implot.h"
#include "semaphore.hpp"

namespace pbma
{

void Application::run()
{
    // Initialize polyscope and ImPlot
    ImPlot::CreateContext();
    polyscope::init();
    set_polyscope_settings();

    std::thread animation_thread{&Application::main_loop, this};

    polyscope::show(); // This function call blocks the main thread

    /*
    When the window is closed, the main thread is unblocked.
    We need to set running to false and release the semaphores
    to notify the simulation thread to stop.
    */
    shutdown();
    animation_thread.join();
    std::cout << "Bye!" << std::endl;
    ImPlot::DestroyContext();
}

void Application::set_polyscope_settings()
{
    polyscope::options::automaticallyComputeSceneExtents = false;
    polyscope::options::ssaaFactor = 2;
    polyscope::view::farClipRatio = 100.0;
    polyscope::state::lengthScale = 2.0f;
    polyscope::options::alwaysRedraw = true;
    polyscope::state::userCallback = [this]() { this->user_callback(); };
}

void Application::main_loop()
{
    while (is_running())
    {
        update();
    }
}

bool Application::is_running() const
{
    return running_.load();
}

void Application::update()
{
    // Producer: generate physics updates
    empty_.acquire();
    mutex_.acquire();
    physics_update();
    mutex_.release();
    full_.release();
}

bool Application::is_animating() const
{
    return animate_.load();
}

void Application::user_callback()
{
    ImGui::PushItemWidth(100);
    if (ImGui::Button("Animate"))
    {
        start_animation();
    }
    ImGui::SameLine();
    if (ImGui::Button("Pause"))
    {
        pause_animation();
    }

    // Consumer: reads physics updates to update the graphics entities
    full_.acquire();
    mutex_.acquire();
    pre_draw();
    mutex_.release();
    empty_.release();

    ImGui::PopItemWidth();
}

void Application::start_animation()
{
    animate_.store(true);
}

void Application::pause_animation()
{
    animate_.store(false);
}

void Application::shutdown()
{
    running_.store(false);
    mutex_.release();
    empty_.release();
}

} // namespace pbma