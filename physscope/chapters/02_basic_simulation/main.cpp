#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <polyscope/point_cloud.h>
#include <polyscope/surface_mesh.h>
#include <vector>

#include "application.hpp"
#include "geometry.hpp"
#include "implot.h"
#include "io.hpp"
#include "shapes/uv_sphere.hpp"

class Simulation : public physscope::Application
{
public:
    // Note that Polyscope structures should NOT be created or
    // initialized inside the constructor. They should be created
    // and accessed only inside initialize() and pre_draw().
    Simulation() = default;
    ~Simulation() override = default;

    /*
    Polyscope-related structures are meant to be created inside
    initialize member-function.
    */
    void initialize() override
    {
        // Note: struct Ball is defined on the private section of this class
        ball = Ball{start_position, "ball", physscope::shapes::uv_sphere, ""};
        polyscope::view::lookAt(glm::vec3{0.0f, 20.0f, 20.0f}, glm::vec3{0.0f, 0.0f, 0.0f});
        positions.clear();
    }

    /*
    Update a displacement variable based on a simple logic.
    This variable can then be used at pre_draw() member function
    to update the render-related Polyscope structures
    */
    void physics_update(float delta_time) override
    {
        if (is_animating())
        {
            const float time_step{0.005f};
            int substep{0};
            while (delta_time > 1e-9f)
            {
                if (substep >= max_num_substeps)
                {
                    std::cout << "WARNING: physics update reached maximum number of substeps." << std::endl;
                    break;
                }

                const float dt{std::min(delta_time, time_step)};
                // Cache variables at n-th time step
                const glm::vec3 current_position{ball.position};
                const glm::vec3 current_velocity{ball.velocity};
                const glm::vec3 acceleration{
                    gravity - ((air_resistance_coefficient / ball.mass) * (wind_velocity - current_velocity))};

                // Updating variables for (n+1)-th time step

                ball.velocity = current_velocity + (acceleration * dt);

                // Explicit Euler
                // const glm::vec3 euler_position{current_position + (current_velocity * dt)};

                // Ending value approach: uses velocity at (n+1)-th time step
                // const glm::vec3 ending_value_position{current_position + (ball.velocity * dt)};

                // Average of Explicit Euler and ending value approach; this is an exact solution.
                const glm::vec3 exact_position{current_position + (((current_velocity + ball.velocity) / 2.0f) * dt)};

                ball.position = exact_position;
                delta_time -= time_step;
                ++substep;
            }

            positions.emplace_back(ball.position.y);
        }
    }

    /*
    Update Polyscope structures fields (e.g. position) based on the output
    generated by the physics_update member function.
    */
    void pre_draw() override
    {
        if (is_animating())
        {
            ball.update_mesh_view_position();
        }

        ImGui::PushItemWidth(300);
        if (ImGui::TreeNode("World Variables"))
        {
            ImGui::SliderFloat("Ball Mass", &ball.mass, 0.001f, 100.0f);
            ImGui::SliderFloat("Gravity", &gravity.y, 0.0f, -20.0f);
            ImGui::SliderFloat("Air Resistance Coefficient", &air_resistance_coefficient, 0.0f, 20.0f);
            ImGui::SliderFloat3("Wind Velocity", glm::value_ptr(wind_velocity), -20.0f, 20.0f);
            ImGui::TreePop();
        }
        ImGui::PopItemWidth();

        if (ImPlot::BeginPlot("Position (y) over Time (x)"))
        {
            ImPlot::SetupLegend(ImPlotLocation_NorthEast);
            ImPlot::SetupAxesLimits(0, std::max(positions.size(), std::size_t{1000}), 0, 20);
            ImPlot::PlotLine("Average Velocity (Exact)", positions.data(),
                             std::min(positions.size(), std::size_t{1'000'000}));
            ImPlot::EndPlot();
        }
    }

private:
    struct Ball
    {
        float mass{10.0f};
        glm::vec3 position{0.0f, 0.0f, 0.0f};
        glm::vec3 velocity{0.0f, 0.0f, 0.0f};
        physscope::geometry::IndexedTriangleMesh triangle_mesh;
        polyscope::SurfaceMesh* mesh_view{nullptr};

        Ball() = default;

        Ball(const glm::vec3& initial_position, const std::string& mesh_name, const std::string& wavefront_string,
             const std::string& material_string) :
            position{initial_position}
        {
            triangle_mesh = physscope::read_triangle_mesh_obj(wavefront_string, material_string);
            mesh_view = polyscope::registerSurfaceMesh(mesh_name, triangle_mesh.vertices, triangle_mesh.indices);
            mesh_view->setSmoothShade(true);
            update_mesh_view_position();
        }

        void update_mesh_view_position()
        {
            mesh_view->setTransform(glm::translate(glm::mat4{1.0f}, position));
        }
    };

    const int max_num_substeps{50};
    Ball ball{};
    const glm::vec3 default_start_position{0.0f, 10.0f, 0.0f};
    glm::vec3 start_position{default_start_position};
    std::vector<float> positions{};

    // World variables
    glm::vec3 gravity{0.0f, -10.0f, 0.0f};
    float air_resistance_coefficient{2.0f};
    glm::vec3 wind_velocity{-20.0f, 0.0f, 0.0f};
};

int main()
{
    Simulation app{};
    app.run();
    return 0;
}