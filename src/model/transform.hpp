#pragma once

#include <model/model.hpp>

template<size_t NV, size_t NI>
constexpr Model<NV, NI> translate_model(Model<NV, NI> model, glm::vec3 delta) {
    Model<NV, NI> result = model;
    
    for (auto& vertex : result.vertices.elems) {
        vertex.position += delta;
    }

    return result;
}

template<size_t NV1, size_t NI1, size_t NV2, size_t NI2>
constexpr Model<NV1 + NV2, NI1 + NI2> combine_models(
    Model<NV1, NI1> model1,
    Model<NV2, NI2> model2
) {
    //std::array<Vertex, NV1 + NV2> result_vertices{};
    //std::array<Index, NI1 + NI2> result_indices{};
    Array<Vertex, NV1 + NV2> result_vertices = {{}};
    Array<Index, NI1 + NI2> result_indices = {{}};

    size_t i = 0;

    // Copy Vertices
    for (auto& vertex : model1.vertices.elems) {
        result_vertices.elems[i++] = vertex;
    }
    for (auto& vertex : model2.vertices.elems) {
        result_vertices.elems[i++] = vertex;
    }

    // Copy Indices
    i = 0;
    for (auto& index : model1.indices.elems) {
        result_indices.elems[i++] = index;
    }
    for (auto& index : model2.indices.elems) {
        result_indices.elems[i++] = index + NV1;
    }

    return Model<NV1 + NV2, NI1 + NI2>(
        result_vertices,
        result_indices
    );
}

template<size_t NI, size_t NV>
constexpr Model<NV, NI> rotate_90(Model<NV, NI> model) {
    for (auto& vertex : model.vertices.elems) {
        glm::vec3 old_position = vertex.position;
        glm::vec3 old_normal = vertex.normal;

        vertex.position.x = -old_position.z;
        vertex.position.z = old_position.x;

        vertex.normal.x = -old_normal.z;
        vertex.normal.z = old_normal.x;
        //(x, y, z) -> (-z, y, x)
    }

    return model;
}

template<size_t NI, size_t NV>
constexpr Model<NV, NI> rotate_180(Model<NV, NI> model) {
    return rotate_90(rotate_90(model));
}

template<size_t NI, size_t NV>
constexpr Model<NV, NI> rotate_270(Model<NV, NI> model) {
    return rotate_90(rotate_90(rotate_90(model)));
}

template<typename T>
constexpr T adder(T v) {
    return v;
}

template<typename T, typename ... Args>
constexpr T adder(T first, Args... args) {
    return first + adder(args...);
}



template<size_t NV, size_t NI>
constexpr Model<NV, NI> combine_all(Model<NV, NI> v) {
    return v;
}

template<size_t NV, size_t NI, size_t ... NVs, size_t ... NIs>
constexpr Model<NV + adder(NVs...), NI + adder(NIs...)> combine_all(Model<NV, NI> x, Model<NVs, NIs>... xs) {
    return combine_models(x, combine_all(xs...));
}
