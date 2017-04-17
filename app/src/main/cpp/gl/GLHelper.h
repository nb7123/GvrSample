//
// Created by michael on 17-4-17.
//

#ifndef GVRSAMPLES_GLHELPER_H
#define GVRSAMPLES_GLHELPER_H

#include <android/asset_manager.h>
#include <string>
#include <GLES3/gl3.h>
#include <bits/unique_ptr.h>
#include "GLProgram.h"

class GLHelper {
private:
    AAssetManager *mgr;
    std::string shaderDir;
public:
    GLHelper(AAssetManager *mgr);
    ~GLHelper();

    void Init();

    /**
     * load shader source from asset file
     * @param file shader source file
     * @return
     */
    std::string LoadShaderSrc(const char *file);

    /**
     * load gl shader use gived shader source
     * @param type  shader type
     * @param file  shader source file
     * @return
     */
    GLuint LoadShader(GLenum type, const char *file);

    /**
     * create gl program
     * @param vs_file   the vertex shader src file
     * @param fs_file   the frag shader src file
     * @return
     */
    std::unique_ptr<GLProgram> CreateGLProgram(const char *vs_file, const char *fs_file);
};


#endif //GVRSAMPLES_GLHELPER_H
