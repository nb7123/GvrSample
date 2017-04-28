//
// Created by michael on 17-4-17.
//

#include "Renderer.h"
#include "util/GLM.h"
#include "SOIL.h"
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer(gvr_context_ *ctx)
        : gvr_api(gvr::GvrApi::WrapNonOwned(ctx)),
          buffer_viewport(gvr_api->CreateBufferViewport()) {
}

Renderer::~Renderer() {
}

void Renderer::InitializeGL() {
    gvr_api->InitializeGl();

    std::vector<gvr::BufferSpec> specs;

    specs.push_back(gvr_api->CreateBufferSpec());
    specs[0].SetColorFormat(GVR_COLOR_FORMAT_RGBA_8888);
    specs[0].SetDepthStencilFormat(GVR_DEPTH_STENCIL_FORMAT_DEPTH_16);

    swap_chain.reset(new gvr::SwapChain(gvr_api->CreateSwapChain(specs)));

    new gvr::SwapChain(gvr_api->CreateSwapChain(specs));
//    swap_chain.reset();
    viewport_list.reset(new gvr::BufferViewportList(gvr_api->CreateEmptyBufferViewportList()));
}

void Renderer::InitProgram(std::unique_ptr<GLProgram> program) {
    this->program.release();
    this->program = std::move(program);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    // load texture
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    int w, h;
    unsigned char *image = GLHelper::LoadTextureImage("img_cheryl.jpg",
                                                            &w, &h, nullptr, SOIL_LOAD_RGBA
    );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);

    this->program->Use();
    GLuint pid = this->program->id();

    GLint p_location = glGetAttribLocation(pid, "pos");
    gl_u_pos_map.insert(
            std::pair<std::string, GLint>("u_model", glGetUniformLocation(pid, "u_model")));
    gl_u_pos_map.insert(
            std::pair<std::string, GLint>("u_view", glGetUniformLocation(pid, "u_view")));
    gl_u_pos_map.insert(std::pair<std::string, GLint>("u_projection",
                                                      glGetUniformLocation(pid, "u_projection")));

    for (auto it = gl_u_pos_map.begin(); it != gl_u_pos_map.end(); ++it) {
        LOGD(__FUNCTION__, "Uniform[%s]'s position[%d]", it->first.c_str(), it->second);
    }

    LOGD(__FUNCTION__, "Found pos location[%d]", p_location);
    glVertexAttribPointer(p_location, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(p_location);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::OnResume() {
    gvr_api->ResumeTracking();
}

void Renderer::OnPause() {
    gvr_api->PauseTracking();
}

void Renderer::DrawFrame() {
    prepareFrameBuffer();
    viewport_list->SetToRecommendedBufferViewports();
    gvr::Frame frame = swap_chain->AcquireFrame();
    gvr::Mat4f head_view = gvr_api->GetHeadSpaceFromStartSpaceRotation(getClockTimePoint());
    gvr::Mat4f eye_view;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    frame.BindBuffer(0);

    glClearColor(.3f, .3f, .3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < 1; ++i) {
        gvr::Eye eye = gvr::Eye(i);
        eye_view = glm::MatrixMul(gvr_api->GetEyeFromHeadMatrix(eye), head_view);

        viewport_list->GetBufferViewport(eye, &buffer_viewport);
        DrawEye(eye_view, buffer_viewport);
    }

    frame.Unbind();

    frame.Submit(*viewport_list, head_view);
}

void Renderer::DrawEye(const gvr::Mat4f &eye_view, const gvr::BufferViewport &viewport) {
    glViewport(0, 0, render_size.width / 2, render_size.height);

    program->Use();

    glBindTexture(GL_TEXTURE_2D, tex);

    glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(.0f, .0f, -1.0f));
    float s = 0.5f;

//    model = glm::scale(glm::mat4(), glm::vec3(s, s, s));
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    gvr::Mat4f m = glm::GLMatrixToGvrMatrix(model);

    gvr::Mat4f p = glm::FrustumFromFov(viewport.GetSourceFov(), 0.1f, 100.0f);
    glUniformMatrix4fv(gl_u_pos_map["u_model"], 1, GL_FALSE, glm::GvrMatrixToGLArray(m).data());
    glUniformMatrix4fv(gl_u_pos_map["u_view"], 1, GL_FALSE, glm::GvrMatrixToGLArray(eye_view).data());
    glUniformMatrix4fv(gl_u_pos_map["u_projection"], 1, GL_FALSE, glm::GvrMatrixToGLArray(p).data());

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 18);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

gvr::ClockTimePoint Renderer::getClockTimePoint() {
    auto result = gvr::GvrApi::GetTimePointNow();

    static const uint64_t kPredictionTimeWithoutVsyncNanos = 50000000;
    result.monotonic_system_time_nanos += kPredictionTimeWithoutVsyncNanos;

    return result;
}

void Renderer::prepareFrameBuffer() {
    gvr::Sizei size = gvr_api->GetMaximumEffectiveRenderTargetSize();
    size.width = (7 * size.width) / 10;
    size.height = (7 * size.height) / 10;

    if (render_size.width != size.width ||
        render_size.height != size.height) {
        // We need to resize the framebuffer.
        swap_chain->ResizeBuffer(0, size);
        render_size = size;
    }
}

