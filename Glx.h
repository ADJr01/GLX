#ifndef GLX_H
#define GLX_H
#include <string>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<functional>
#include<vector>

#include "GLSLX/GlslX.h"

struct Ratio{
    int numerator;
    int denumerator;
};

struct Screen{
    int full_width;
    int full_height;
};

class GLX{
private:
    double version = 0.25;
    GLFWwindow* window;
    Ratio WindowAspectRatio;
    Screen WindowScreen;
    bool is_running;
    bool gl_experimental;
    std::string Window_title;
    int open_gl_version_minor;
    int open_gl_version_major;
    bool focus_on_init;
    bool is_forward_compatable;
    int Window_Width, Window_Height, frame_buffer_width, frame_buffer_height;
    std::vector<std::function<void()>> tasklist;
    std::vector<std::function<void()>> postLaunchQueue;
    //shader tool
    GlslX* shaderTool = new GlslX();
    void destroy();
    public:
    GLX();
    ~GLX();
    void setAspectRatio(int nume, int denume);
    void setGLExperimental(bool experimental);
    void setVersionMajor(int ver_major);
    void setVersionMinor(int ver_minor);
    void setFocusOnInit(bool focus_on_init);
    void setWindowHeight(int window_height);
    void setWindowWidth(int window_width);
    void setWindowTitle(std::string window_title);
    void setIsForwardCompatable(bool is_forward_compatable);
    void inf();
    int glx_primary_monitor_width();
    int glx_primary_monitor_height();
    bool status();
    template<typename Func, typename... Args>
    void  onTick(Func func, Args... args) {
        this->tasklist.push_back(std::bind(func, args...));
    }
    GlslX& ShaderTool() const;
    bool launch();
    template<typename Func, typename... Args>
    void  addPostLaunchProcedure(Func func, Args... args) {
        this->postLaunchQueue.push_back(std::bind(func, args...));
    }
    
};

#endif