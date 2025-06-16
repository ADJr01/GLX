#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <utility>
#include "GlslX.h"

std::string parseShader(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        throw std::runtime_error("Failed to open file");
    }
    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        throw std::runtime_error("GLX::Shader-Tool::Failed to seek to end of file");
    }
    long size = ftell(file);
    if (size == -1) {
        fclose(file);
        throw std::runtime_error("GLX::Shader-Tool::Failed to get file size");
    }
    if (fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        throw std::runtime_error("GLX::Shader-Tool::Failed to seek to beginning of file");
    }
    std::string buffer(size, '\0');
    size_t read_size = fread(&buffer[0], 1, size, file);
    fclose(file);
    if (read_size != static_cast<size_t>(size)) {
        throw std::runtime_error("GLX::Shader-Tool::Failed to read entire file");
    }
    return buffer;
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
    if (this->CompilationStatus != 0 || this->isBuildReady()) {
        this->deleteProgram();
        this->CompilationStatus = 0;
        this->BuildStatus = 0;
    }
}

bool GlslX::setFragmentShaderPath(std::string path) {
    try {
        return this->setFragmentShaderSource(parseShader(path.c_str()));
    }catch (std::exception& e) {
        this->ErrorLog = e.what();
        std::cerr << this->ErrorLog << std::endl;
        return false;
    }
}
bool GlslX::setFragmentShaderSource(std::string source) {
    this->fragmentSource = std::move(source);
    return true;
}


bool GlslX::setVertexShaderPath(std::string path) {
    try {
        return this->setVertexShaderSource(parseShader(path.c_str()));
    }catch (std::exception& e) {
        this->ErrorLog = e.what();
        std::cerr << this->ErrorLog << std::endl;
        return false;
    }

}

bool GlslX::setVertexShaderSource(std::string source) {
    this->vertexSource = std::move(source);
    return true;
}
bool GlslX::deleteFragmentShader() {
    if (this->fragmentShader && !this->fragmentSource.empty()) {
        glDeleteShader(this->fragmentShader);
        this->fragmentShader=0;
        this->fragmentSource="";
    }
    return true;
}

bool GlslX::deleteVertexShader() {
    if (this->vertexShader && !this->vertexSource.empty()) {
        glDeleteShader(this->vertexShader);
        this->vertexShader=0;
        this->vertexSource="";
    }
    return true;
}

std::string& GlslX::getError() {
    return this->ErrorLog;
}
bool GlslX::deleteProgram() {
    if (this->program) {
        this->deleteFragmentShader();
        this->deleteVertexShader();
        glDeleteProgram(this->program);
        this->CompilationStatus = 0;
        this->BuildStatus = 0;
    }
    return true;
}
void GlslX::logVertexShader() const {
    std::cout<<this->vertexSource<<"\n";
}
void GlslX::logFragmentShader() const {
    std::cout<<this->vertexSource<<"\n";
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
    if (result==GL_FALSE)
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
    if (!this->ErrorLog.empty()) throw std::runtime_error("GLX::Shader-Tool::Cannot Build Program With ErrorLog:"+this->ErrorLog);
    if (this->BuildStatus==true)return this->BuildStatus;
    if (this->fragmentSource.empty() || this->vertexSource.empty()) {
        this->ErrorLog = "GLX::Shader-Tool::Empty-Src is not acceptable\n";
        this->BuildStatus=0;
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


unsigned int& GlslX::getProgram() {
    if (this->isBuildReady()) {
        return this->program;
    }
    throw std::runtime_error("GLX::Shader-Tool::Build is not Ready");
}

bool GlslX::isBuildReady() const {
    return this->BuildStatus==1;
}


