//
// Created by michael on 17-4-17.
//

#include "GLM.h"
#include <cmath>

namespace glm {
    // matrix mult
    gvr::Mat4f MatrixMul(const gvr::Mat4f &mat1, const gvr::Mat4f &mat2) {
        gvr::Mat4f result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i][j] = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    result.m[i][j] += mat1.m[i][k] * mat2.m[k][j];
                }
            }
        }
        return result;
    }

    // matrix mult vector
    std::array<float, 4> MatrixMulVec(const gvr::Mat4f &mat, const std::array<float, 4> vec4) {
        std::array<float, 4> result;
        for (int i = 0; i < 4; ++i) {
            result[i] = 0;
            for (int k = 0; k < 4; ++k) {
                result[i] += mat.m[i][k] * vec4[k];
            }
        }
        return result;
    };

    // frustum from vr fov view
    gvr::Mat4f FrustumFromFov(const gvr::Rectf &fov, const float z_near, const float z_far) {
        const float radian = (const float) (M_PI / 180.0f);
        const float l = -std::tan(fov.left * radian);
        const float r = std::tan(fov.right * radian);
        const float b = -std::tan(fov.bottom * radian);
        const float t = std::tan(fov.top * radian);

        // assert all params are valid
        assert(l < r && b < t && z_near < z_far && z_near > 0.0f);

        gvr::Mat4f result;
        // init to 0.0f
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i][j] = 0.0f;
            }
        }
        // calculate params
        const float X = (2 * z_near) / (r - l);
        const float Y = (2 * z_near) / (t - b);
        const float A = (r + l) / (r - l);
        const float B = (t + b) / (t - b);
        const float C = (z_near + z_far) / (z_near - z_far);
        const float D = (2 * z_near * z_far) / (z_near - z_far);

        result.m[0][0] = X;
        result.m[0][2] = A;
        result.m[1][1] = Y;
        result.m[1][2] = B;
        result.m[2][2] = C;
        result.m[2][3] = D;
        result.m[3][2] = -1;

        return result;
    }

    gvr::Mat4f GLMatrixToGvrMatrix(const glm::mat4 &mat) {
        gvr::Mat4f result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i][j] = mat[j][i];
            }
        }

        return result;
    }

    std::array<float, 16> GvrMatrixToGLArray(const gvr::Mat4f &mat) {
        std::array<float, 16> result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result[j*4+i] = mat.m[i][j];
            }
        }

        return result;
    }
}
