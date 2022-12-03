#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <polyscope/point_cloud.h>
#include <vector>

#include "application.hpp"
#include "implot.h"
#include "semaphore.hpp"

class TemplateApplication : public pbma::Application
{
public:
    TemplateApplication() = default;
    ~TemplateApplication() override = default;

    void physics_update() override
    {
        if (is_animating())
        {
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
    }

    void pre_draw() override
    {
        // Update point cloud
        points.clear();
        for (int i = 0; i < number_of_points; ++i)
        {
            points.emplace_back(glm::vec3{polyscope::randomUnit(), polyscope::randomUnit(), polyscope::randomUnit()});
        }
        point_cloud = polyscope::registerPointCloud("Sample Point Cloud", points);
        if (is_animating())
        {
            point_cloud->setTransform(glm::translate(glm::mat4{1.0f}, glm::vec3{0.0f, displacement, 0.0f}));
        }
        const std::size_t time_samples{std::min(positions.size(), std::size_t{10'000})};
        if (ImPlot::BeginPlot("Position over Time"))
        {
            ImPlot::SetupAxesLimits(0, std::max(time_samples, std::size_t{1'000}), 0, 4);
            ImPlot::PlotLine("Position", positions.data(), time_samples);
            ImPlot::EndPlot();
        }
    }

private:
    const int number_of_points{10000};
    polyscope::PointCloud* point_cloud{nullptr};
    std::vector<glm::vec3> points;
    std::vector<float> positions;
    std::vector<float> times;
    float displacement{0.0f};
    int sign{1};
};

int main()
{
    TemplateApplication app{};
    app.run();
    return 0;
}