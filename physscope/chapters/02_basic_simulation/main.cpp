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
        // Note: struct Ball on the private section of this class
        ball = Ball{start_position, "ball", physscope::shapes::uv_sphere, ""};
        polyscope::view::lookAt(glm::vec3{0.0f, 10.0f, 30.0f}, glm::vec3{0.0f, 0.0f, 0.0f});
        positions.clear();
        positions.emplace_back(ball.position.y);
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
            while (delta_time > 1e-9f)
            {
                const float dt{std::min(delta_time, time_step)};
                // Cache variables at n-th time step
                const glm::vec3 current_position{ball.position};
                const glm::vec3 current_velocity{ball.velocity};

                // Updating variables for (n+1)-th time step
                ball.velocity = current_velocity + (acceleration * dt);
                ball.position = current_position + (current_velocity * dt);
                delta_time -= time_step;
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

        if (ImPlot::BeginPlot("Position over Time"))
        {
            ImPlot::SetupLegend(ImPlotLocation_NorthEast);
            ImPlot::SetupAxesLimits(0, std::max(positions.size(), std::size_t{1000}), -60, 20);
            ImPlot::PlotLine("Position", positions.data(), std::min(positions.size(), std::size_t{1'000'000}));
            ImPlot::EndPlot();
        }
    }

private:
    struct Ball
    {
        glm::vec3 position{0.0f, 0.0f, 0.0f};
        glm::vec3 velocity{0.0f, 0.0f, 0.0f};
        physscope::geometry::IndexedTriangleMesh triangle_mesh;
        polyscope::SurfaceMesh* mesh_view{nullptr};

        Ball() = default;
        Ball(const Ball&) = delete;
        Ball(Ball&&) = default;
        Ball& operator=(const Ball&) = delete;
        Ball& operator=(Ball&&) = default;
        ~Ball() = default;

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

    Ball ball;
    const glm::vec3 default_start_position{0.0f, 10.0f, 0.0f};
    glm::vec3 start_position{default_start_position};
    glm::vec3 acceleration{0.0f, -10.0f, 0.0f};
    std::vector<float> positions;
};

int main()
{
    Simulation app{};
    app.run();
    return 0;
}