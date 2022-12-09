#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <array>
#include <glm/glm.hpp>
#include <vector>

namespace physcope
{

namespace geometry
{

class Triangle
{
public:
    Triangle() = default;
    Triangle(const glm::vec3& vertex_0, const glm::vec3& vertex_1, const glm::vec3& vertex_2);
    const glm::vec3& vertex(std::size_t index) const;

private:
    std::array<glm::vec3, 3> vertices_{};
};

struct IndexedTriangleMesh
{
    std::vector<glm::vec3> vertices;
    std::vector<std::array<std::size_t, 3>> indices;

    std::size_t num_vertices() const;
    std::size_t num_indices() const;
    Triangle triangle(std::size_t triangle) const;
};

} // namespace geometry

} // namespace physcope

#endif // GEOMETRY_HPP