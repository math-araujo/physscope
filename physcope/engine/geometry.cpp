#include "geometry.hpp"

namespace physcope
{

namespace geometry
{

Triangle::Triangle(const glm::vec3& vertex_0, const glm::vec3& vertex_1, const glm::vec3& vertex_2) :
    vertices_{vertex_0, vertex_1, vertex_2}
{
}

const glm::vec3& Triangle::vertex(std::size_t index) const
{
    return vertices_[index];
}

std::size_t IndexedTriangleMesh::num_vertices() const
{
    return vertices.size();
}

std::size_t IndexedTriangleMesh::num_indices() const
{
    return indices.size();
}

Triangle IndexedTriangleMesh::triangle(std::size_t triangle) const
{
    return Triangle{vertices[3 * triangle], vertices[3 * triangle + 1], vertices[3 * triangle + 2]};
}

} // namespace geometry

} // namespace physcope