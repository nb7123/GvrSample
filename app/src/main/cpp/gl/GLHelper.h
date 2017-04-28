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

#define TEXTURE_DIR "Texture/"

class GLHelper {
private:
    static AAssetManager* mgr;
    static std::string shaderDir;
public:
    static void setAAssetManager(AAssetManager *mgr);
    static AAssetManager *getAAssetManager() { return mgr;}

    static void Init();

    /**
     * load shader source from asset file
     * @param file shader source file
     * @return
     */
    static std::string LoadShaderSrc(const char *file);

    /**
     * load gl shader use gived shader source
     * @param type  shader type
     * @param file  shader source file
     * @return
     */
    static GLuint LoadShader(GLenum type, const char *file);

    /**
     * create gl program
     * @param vs_file   the vertex shader src file
     * @param fs_file   the frag shader src file
     * @return
     */
    static std::unique_ptr<GLProgram> CreateGLProgram(const char *vs_file, const char *fs_file);

    /**
     * load image texture from buffer data
     * @param buffer
     * @param buffer_length
     * @param width
     * @param height
     * @param channels
     * @param force_channel
     * @return
     */
    static unsigned char *LoadTextureImage(const char *file,
                                    int *width,
                                    int *height,
                                    int *channels,
                                    int force_channel);
};


#endif //GVRSAMPLES_GLHELPER_H
