//
// Created by michael on 17-4-17.
//

#ifndef GVRSAMPLES_GLPROGRAM_H
#define GVRSAMPLES_GLPROGRAM_H

#include <GLES3/gl3.h>

class GLProgram {
private:
    GLuint program;

public:
    GLProgram(GLuint program);
    ~GLProgram();
    void Use();
    GLuint id();
};


#endif //GVRSAMPLES_GLPROGRAM_H
