//
// Created by michael on 17-4-17.
//

#ifndef GVRSAMPLES_GLM_H
#define GVRSAMPLES_GLM_H

#include <vr/gvr/capi/include/gvr.h>
#include <vr/gvr/capi/include/gvr_types.h>
#include <array>

namespace glm {
    // matrix mult
    gvr::Mat4f MatrixMul(const gvr::Mat4f &mat1, const gvr::Mat4f &mat2);

    // matrix mult vector
    std::array<float, 4> MatrixMulVec(const gvr::Mat4f &mat, const std::array<float, 4> vec4);

    // frustum from vr fov view
    gvr::Mat4f FrustumFromFov(const gvr::Rectf &fov, const float z_near, const float z_far);

    std::array<float, 16> MatrixToGLArray(const gvr::Mat4f &mat4f);
}

class GLM {

};


#endif //GVRSAMPLES_GLM_H
