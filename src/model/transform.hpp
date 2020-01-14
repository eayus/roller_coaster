#pragma once

#include <model/model.hpp>

// Functions which transofmr models. all the functions are constexpr so we can use
// them at compile time.


// Translation is as simple as adding an offset to each vertex.
template<size_t NV, size_t NI>
constexpr Model<NV, NI> translate_model(Model<NV, NI> model, glm::vec3 delta) {
    Model<NV, NI> result = model;
    
    for (auto& vertex : result.vertices.elems) {
        vertex.position += delta;
    }

    return result;
}

// This function takes two models, and combines them into one.
template<size_t NV1, size_t NI1, size_t NV2, size_t NI2>
constexpr Model<NV1 + NV2, NI1 + NI2> combine_models(
    Model<NV1, NI1> model1,
    Model<NV2, NI2> model2
) {
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

    // Indices require a little more thought, as we need to make sure
    // indices in the second model don't reference the vertices of the
    // first. That is why we (+ NV1) .
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

// Rotation a model 90
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

// Rotating a model more degrees can simply be the repeated
// application of the previous function. Performance is not
// a big issue as this is done at compile time.
template<size_t NI, size_t NV>
constexpr Model<NV, NI> rotate_180(Model<NV, NI> model) {
    return rotate_90(rotate_90(model));
}

template<size_t NI, size_t NV>
constexpr Model<NV, NI> rotate_270(Model<NV, NI> model) {
    return rotate_90(rotate_90(rotate_90(model)));
}

// Definition of a varialbe argument template function which
// adds all the paramters. This is useful when combining
// an arbitrary number of different models
template<typename T>
constexpr T add_all(T v) {
    return v;
}

template<typename T, typename ... Ts>
constexpr T add_all(T x, Ts ... xs) {
    return x + add_all(xs...);
}



// Combine all models into one.
// Base case is simply return the provided model.
template<size_t NV, size_t NI>
constexpr Model<NV, NI> combine_all(Model<NV, NI> v) {
    return v;
}

// Recursive case
template<size_t NV, size_t NI, size_t ... NVs, size_t ... NIs>
constexpr Model<NV + add_all(NVs...), NI + add_all(NIs...)> combine_all(Model<NV, NI> x, Model<NVs, NIs>... xs) {
    return combine_models(x, combine_all(xs...));
}
