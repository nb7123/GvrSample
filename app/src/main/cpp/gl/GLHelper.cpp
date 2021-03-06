//
// Created by michael on 17-4-17.
//

#include "GLHelper.h"
#include "../Log.h"
#include "../soil/SOIL.h"

AAssetManager* GLHelper::mgr = nullptr;
std::string GLHelper::shaderDir = std::string();

std::string GLHelper::LoadShaderSrc(const char *file) {
    std::string result;

    AAsset *aAsset = AAssetManager_open(GLHelper::mgr, file, AASSET_MODE_BUFFER);
    if (nullptr == aAsset) {
        LOGE(__FUNCTION__, "Open asset file[%s] error.", file);
        return result;
    }

    int len = 0;
    char buff[1024];
    const size_t size = sizeof(buff);
    while ((len = AAsset_read(aAsset, buff, size)) > 0) {
        LOGI(__FUNCTION__, "---%d--- Read count[%d]", __LINE__, len);
        result.append(buff, (size_t)len);
    }

    AAsset_close(aAsset);

    return result;
}

GLuint GLHelper::LoadShader(GLenum type, const char *file) {
    // load shader source
    std::string f_path(GLHelper::shaderDir);
    f_path.append(file);

    const std::string src = LoadShaderSrc(f_path.data());
    if (src.length() <= 0) {
        return 0;
    }

    const char *ss = src.data();
    // create shader
    GLuint shader = glCreateShader(type);
    // set shader src
    glShaderSource(shader, 1, &ss, nullptr);
    // compile shader
    glCompileShader(shader);

    // check compile status
    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status) {
        LOGI(__FUNCTION__, "Shader compile success");
    } else {
        char buff[4096];
        glGetShaderInfoLog(shader, sizeof(buff), nullptr, buff);
        LOGW(__FUNCTION__, "Shader[%s] compile failed[%s]", f_path.data(), buff);

        glDeleteShader(shader);
        shader = 0;
    }

    return shader;
}

std::unique_ptr<GLProgram> GLHelper::CreateGLProgram(const char *vs_file, const char *fs_file) {
    // create vertex shader and fragment shader
    const GLuint vs = LoadShader(GL_VERTEX_SHADER, vs_file);
    const GLuint fs = LoadShader(GL_FRAGMENT_SHADER, fs_file);

    // create and link program
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    // check link status
    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status) {
        LOGI(__FUNCTION__, "Link program success, vertex file[%s], fragment file[%s]", vs_file, fs_file);
    } else {
        char buff[4096];
        glGetProgramInfoLog(program, sizeof(buff), nullptr, buff);
        LOGW(__FUNCTION__, "Link program failed, message[%s]", buff);
        // delete program when link error
        glDeleteProgram(program);
        program = 0;
    }

    // delete shader
    glDeleteShader(vs);
    glDeleteShader(fs);

    return std::unique_ptr<GLProgram>(new GLProgram(program));
}

void GLHelper::Init() {
    const char *names[] = {"Vendor", "Version", "Renderer", "Extensions"};
    GLenum infos[] = {GL_VENDOR, GL_VERSION, GL_RENDERER, GL_EXTENSIONS};
    for (int i = 0; i < 4; ++i) {
        LOGI(__FUNCTION__, "%s: %s", names[i], glGetString(infos[i]));
    }
    const char *version = (const char *) glGetString(GL_VERSION);
    if (strstr(version, "OpenGL ES 3.")) {
        GLHelper::shaderDir = "ShaderV3/";
    } else if (strstr(version, "OpenGL ES 2.")) {
        GLHelper::shaderDir = "ShaderV2/";
    } else {
        LOGE(__FUNCTION__, "Unknown gl es version: %s", version);
    }
}

unsigned char *
GLHelper::LoadTextureImage(const char *file, int *width, int *height,
                           int *channels, int force_channel) {
    std::string f_path(TEXTURE_DIR);
    f_path.append(file);
    LOGI(__FUNCTION__, "Texture path[%s]", f_path.data());

    AAsset *aAsset = AAssetManager_open(GLHelper::mgr, f_path.data(), AASSET_MODE_BUFFER);
    if (nullptr == aAsset) {
        LOGE(__FUNCTION__, "Open asset file[%s] error.", file);
        return nullptr;
    }

    const unsigned char *const buff = (const unsigned char *) AAsset_getBuffer(aAsset);

    unsigned char *result = SOIL_load_image_from_memory(buff, AAsset_getLength(aAsset), width, height, channels, force_channel);

    AAsset_close(aAsset);

    return result;
}

void GLHelper::setAAssetManager(AAssetManager *a) {
    GLHelper::mgr = a;
}




