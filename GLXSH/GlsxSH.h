#pragma once
#include <string>

class GlsxSH{
private:
    unsigned int program;
    unsigned int vertexShader;
    unsigned int fragmentShader;
    std::string vertexSource;
    std::string fragmentSource;
    std::string loadDirectory;
public:
    GlsxSH();
    ~GlsxSH();
    void loadShader();
    
};
