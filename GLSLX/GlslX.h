#ifndef GLSLX_H
#define GLSLX_H
#include <string>

class GlslX{
private:
    unsigned int program;
    unsigned int vertexShader{};
    unsigned int fragmentShader{};
    std::string vertexSource;
    std::string fragmentSource;
    std::string geometrySource;
    std::string tessControlSource;
    //ErrorFlags;
    std::string ErrorLog;
    //shaders
    unsigned int FragShader;
    unsigned int VertShader;
    //status flags
    unsigned int CompilationStatus;
    unsigned int BuildStatus;
    //shader compiler
    void CompileShader(unsigned int shader_type);
public:
    GlslX();
    ~GlslX();
    bool setFragmentShaderPath(std::string path);
    bool setVertexShaderPath(std::string path);
    bool deleteFragmentShader();
    bool deleteVertexShader();
    bool deleteProgram();
    bool buildProgram();
    bool isCompiled();
    void logVertexShader() const;
    void logFragmentShader() const;
    unsigned int& getProgram() ;
    bool isBuildReady() const;
    std::string& getError();

    
};
#endif // GLSLX_H