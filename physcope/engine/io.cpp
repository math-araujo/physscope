#include "io.hpp"
#include <iostream>
#include <tiny_obj_loader.h>

namespace physcope
{

geometry::IndexedTriangleMesh read_triangle_mesh_obj(std::string_view filename, bool verbose)
{
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "assets/";
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filename.data(), reader_config))
    {
        if (!reader.Error().empty())
        {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty())
    {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    return read_triangle_mesh_obj(reader, verbose);
}

geometry::IndexedTriangleMesh read_triangle_mesh_obj(std::string_view wavefront, std::string_view mtl, bool verbose)
{
    tinyobj::ObjReader reader;
    if (!reader.ParseFromString(wavefront.data(), mtl.data()))
    {
        if (!reader.Error().empty())
        {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty())
    {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    return read_triangle_mesh_obj(reader, verbose);
}

geometry::IndexedTriangleMesh read_triangle_mesh_obj(tinyobj::ObjReader& reader, bool verbose)
{
    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    std::vector<glm::vec3> vertices;
    vertices.reserve(attrib.vertices.size() / 3);
    for (std::size_t i = 0; i < attrib.vertices.size(); i += 3)
    {
        vertices.emplace_back(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]);
    }

    std::vector<std::array<std::size_t, 3>> indices;
    std::array<std::size_t, 3> current_face_indices{};
    for (std::size_t shape_index = 0; const auto& shape : shapes)
    {
        if (verbose)
        {
            std::cout << "Shape " << shape_index << ":\n";
            std::cout << "\tName: " << shape.name << std::endl;
            std::cout << "\tNumber of faces:\t" << shape.mesh.num_face_vertices.size() << std::endl;
            std::cout << "\tNumber of indices:\t" << shape.mesh.indices.size() << std::endl;
        }

        indices.reserve(3 * shape.mesh.num_face_vertices.size());
        for (std::size_t face = 0; face < shape.mesh.num_face_vertices.size(); ++face)
        {
            for (std::size_t offset = 0; offset < 3; ++offset)
            {
                current_face_indices[offset] = shape.mesh.indices[3 * face + offset].vertex_index;
            }
            indices.emplace_back(current_face_indices);
        }

        ++shape_index;
    }

    return geometry::IndexedTriangleMesh{.vertices = std::move(vertices), .indices = std::move(indices)};
}

} // namespace physcope