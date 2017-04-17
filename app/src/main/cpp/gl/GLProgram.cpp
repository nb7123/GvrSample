//
// Created by michael on 17-4-17.
//

#include "GLProgram.h"

GLProgram::GLProgram(GLuint program) : program(program) {

}

void GLProgram::Use() {
    glUseProgram(program);
}

GLProgram::~GLProgram() {
    glDeleteProgram(program);
}

GLuint GLProgram::id() {
    return program;
}
