#pragma once

//#include <array>
#include <glm/glm.hpp>
#include <std_extras/array.hpp>

using std_extras::Array;

using Index = unsigned short;

struct Vertex {
    constexpr Vertex()
        : position(0, 0, 0)
        , normal(0, 0, 0)
        , color(0, 0, 0) {}
    constexpr Vertex(glm::vec3 position,
                     glm::vec3 normal,
                     glm::vec3 color)
        : position(position)
        , normal(normal)
        , color(color) {}

    glm::vec3 position, normal, color;
};

// NV = number of vertices, NI = number of indices
template<size_t NV, size_t NI>
struct Model {
    constexpr Model<NV, NI>(Array<Vertex, NV> vertices,
                            Array<Index, NI> indices)
        : vertices(vertices)
        , indices(indices) {}

    /*constexpr static Model<0, 0> empty() {
        return Model<0, 0>({}, {});
    }*/

    Array<Vertex, NV> vertices;
    Array<Index, NI> indices;
};

struct ModelRef {
    template <size_t NV, size_t NI>
    constexpr ModelRef(const Model<NV, NI>& model)
        : vertices_start(model.vertices.elems)
        , vertices_len(NV)
        , indices_start(model.indices.elems)
        , indices_len(NI) {}

    const Vertex* vertices_start;
    size_t vertices_len;
    const Index* indices_start;
    size_t indices_len;
};
