//
// Created by michael on 17-4-17.
//

#ifndef GVRSAMPLES_RENDERER_H
#define GVRSAMPLES_RENDERER_H


#include <bits/unique_ptr.h>
#include <vr/gvr/capi/include/gvr.h>
#include <map>

#include "gl/GLProgram.h"
#include "Log.h"
#include "gl/GLHelper.h"

static const GLfloat vertex[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.0f,  0.5f, -0.0f,  1.0f, 1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.0f,  0.5f,  0.0f,  1.0f, 1.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.0f,  0.5f,  0.0f,  1.0f, 0.0f,

        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.0f,  0.5f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
};

class Renderer {
private:
    std::unique_ptr<gvr::GvrApi> gvr_api;
    std::unique_ptr<gvr::SwapChain> swap_chain;
    std::unique_ptr<gvr::BufferViewportList> viewport_list;

    gvr::BufferViewport buffer_viewport;
    std::unique_ptr<GLProgram> program;
    GLuint vao;
    GLuint tex;
    gvr::ClockTimePoint getClockTimePoint();
    gvr::Sizei render_size;

    void checkGLError(const char *label) {
        GLenum error = glGetError();
        if (GL_NO_ERROR != error) {
            LOGE(label, "GL error: %d", error);
        }
    }

    void prepareFrameBuffer();

    std::map<std::string, GLint> gl_u_pos_map;
public:
    Renderer(gvr_context_ *ctx);
    ~Renderer();

    void InitializeGL();
    void InitProgram(std::unique_ptr<GLProgram> program);

    void OnResume();
    void OnPause();
    void DrawFrame();
    void DrawEye(const gvr::Mat4f &eye_view, const gvr::BufferViewport &viewport);
};


#endif //GVRSAMPLES_RENDERER_H
