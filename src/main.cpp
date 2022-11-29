#include <algorithm>
#include <atomic>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <polyscope/point_cloud.h>
#include <polyscope/polyscope.h>
#include <thread>
#include <vector>

#include "implot.h"
#include "semaphore.hpp"

struct Application
{
    const int number_of_points{10000};
    std::atomic<bool> running{true};
    std::atomic<bool> animate{false};
    pbma::Semaphore empty{1};
    pbma::Semaphore full{0};
    pbma::Semaphore mutex{1};
    std::vector<glm::vec3> points;
    std::vector<float> positions;
    std::vector<float> times;
    float displacement{0.0f};
    int sign{1};

    Application()
    {
        ImPlot::CreateContext();
        // Initialize polyscope
        polyscope::init();
        set_polyscope_settings();
    }

    void set_polyscope_settings()
    {
        polyscope::options::automaticallyComputeSceneExtents = false;
        polyscope::options::ssaaFactor = 2;
        polyscope::view::farClipRatio = 100.0;
        polyscope::state::lengthScale = 2.0f;
        polyscope::options::alwaysRedraw = true;
        polyscope::state::userCallback = [this]() { this->user_callback(); };
    }

    void run()
    {
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

    void main_loop()
    {
        while (running.load())
        {
            update();
        }
    }

    void update()
    {
        if (animate.load())
        {
            empty.acquire();
            mutex.acquire();
            {
                //
                if (sign > 0 && displacement > 4.0f)
                {
                    sign = -1;
                }
                else if (sign < 0 && displacement < 1.0f)
                {
                    sign = 1;
                }

                displacement += (sign)*0.01f;
                positions.emplace_back(displacement);
            }
            mutex.release();
            full.release();
        }
    }

    void user_callback()
    {
        ImGui::PushItemWidth(100);
        if (ImGui::Button("Animate"))
        {
            animate.store(true);
        }

        ImGui::PopItemWidth();

        // Update point cloud
        points.clear();
        for (int i = 0; i < number_of_points; ++i)
        {
            points.emplace_back(glm::vec3{polyscope::randomUnit(), polyscope::randomUnit(), polyscope::randomUnit()});
        }
        auto point_cloud = polyscope::registerPointCloud("Sample Point Cloud", points);

        if (animate.load())
        {
            full.acquire();
            mutex.acquire();
            point_cloud->setTransform(glm::translate(glm::mat4{1.0f}, glm::vec3{0.0f, displacement, 0.0f}));
            const std::size_t time_samples{std::min(positions.size(), std::size_t{10'000})};
            if (ImPlot::BeginPlot("Position over Time"))
            {
                ImPlot::SetupAxesLimits(0, std::max(time_samples, std::size_t{1'000}), 0, 4);
                ImPlot::PlotLine("Position", positions.data(), time_samples);
                ImPlot::EndPlot();
            }
            mutex.release();
            empty.release();
        }
    }

    void shutdown()
    {
        running.store(false);
        mutex.release();
        empty.release();
    }
};

int main()
{
    Application app{};
    app.run();
    return 0;
}