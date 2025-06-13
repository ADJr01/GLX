#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <sstream>
#include "GlslX.h"

std::string parseShader(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        throw std::invalid_argument("GLX::Shader-Tool::Invalid File Ref:" + filepath);
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string source =  buffer.str();
    file.close();
    return source;

}

GlslX::GlslX() {
    this->CompilationStatus = 0;
    this->BuildStatus = 0;
    this->ErrorLog = "";
    this->FragShader = 0;
    this->VertShader = 0;
    this->program=0;
}
GlslX::~GlslX() {
    if (this->CompilationStatus != 0) {
        glDeleteShader(this->FragShader);
        glDeleteShader(this->VertShader);
        glDeleteProgram(this->program);
        this->CompilationStatus = 0;
        this->BuildStatus = 0;
    }
}

bool GlslX::setFragmentShaderPath(std::string path) {
    this->fragmentSource = parseShader(path);
    return true;
}

bool GlslX::setVertexShaderPath(std::string path) {
    this->vertexSource = parseShader(path);
    return true;
}
bool GlslX::deleteFragmentShader() {
    if (this->fragmentShader) {
        glDeleteShader(this->fragmentShader);
    }
    return true;
}

bool GlslX::deleteVertexShader() {
    if (this->vertexShader) {
        glDeleteShader(this->vertexShader);
    }
    return true;
}

std::string& GlslX::getError() {
    return this->ErrorLog;
}
bool GlslX::deleteProgram() {
    if (this->program) {
        glDeleteProgram(this->program);
    }
    return true;
}

void GlslX::CompileShader(unsigned int shader_type) {
    const char* src = shader_type==GL_FRAGMENT_SHADER?this->fragmentSource.c_str():this->vertexSource.c_str();
    const unsigned int shader_program = glCreateShader(shader_type);
    const GLchar* shader_src[1];
    shader_src[0] = src;
    GLint code_length[1];
    code_length[0] = static_cast<GLint>(shader_type==GL_FRAGMENT_SHADER?this->fragmentSource.length():this->vertexSource.length());
    glShaderSource(shader_program,1,shader_src,code_length);
    glCompileShader(shader_program);
    GLint result = 0;
    glGetShaderiv(shader_program,GL_COMPILE_STATUS,&result);
    if (!result)
    {
        int len = 0;
        glGetShaderiv(shader_program,GL_INFO_LOG_LENGTH,&len);
        char* info_log = static_cast<char *>(alloca(len));
        glGetShaderInfoLog(shader_program,1024,NULL,info_log);
        this->ErrorLog = std::string("GLX::Shader-Tool::") +
        (shader_type == GL_FRAGMENT_SHADER ? "Fragment" : "Vertex") +
        " Shader-Compilation Failed\n" +
        std::string(info_log) +
        "\n";
        std::cout<<ErrorLog;
        this->CompilationStatus = 0;
        return;
    }
    if (shader_type==GL_FRAGMENT_SHADER) {
        this->vertexShader=shader_program;
    }else {
        this->fragmentShader=shader_program;
    }
    this->CompilationStatus = 1;

}



bool GlslX::buildProgram() {
    if (this->BuildStatus==true)return this->BuildStatus;
    if (this->fragmentSource.length()<1 || this->vertexSource.length()<1) {
        this->ErrorLog = "GLX::Shader-Tool::Empty-Src is not acceptable\n";
        throw std::invalid_argument(this->getError());
    }
    this->program=glCreateProgram();
    this->CompileShader(GL_VERTEX_SHADER);
    this->CompileShader(GL_FRAGMENT_SHADER);
    if (this->CompilationStatus!=1)throw std::runtime_error("GLX::Shader-Tool::" + this->getError());
    glAttachShader(this->program,this->vertexShader);
    glAttachShader(this->program,this->fragmentShader);
    glLinkProgram(this->program);
    glValidateProgram(this->program);
    this->BuildStatus = 1;
    return this->BuildStatus;
}

unsigned int GlslX::getProgram() {
    if (this->BuildStatus==1)return this->program;
    throw std::runtime_error("GLX::Shader-Tool::Build is not ready yet");
}



