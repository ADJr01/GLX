#pragma once
#include "Glx.h"
#include <iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<functional>
#include<vector>

GLX::GLX(){
    if (!glfwInit()) {
        std::cout << "Failed To Init GLX:: Error occured when initializing glfw.";
        glfwTerminate();
    }
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
        std::cerr << "Failed to get primary monitor\n";
        glfwTerminate();
        return;
    }
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    this->window = nullptr;
    this->is_running=false;
    //By Default Using OpenGL4.1
    this->open_gl_version_major=4;
    this->open_gl_version_minor=1;
    this->Window_Width=800;
    this->Window_Height=600;
    this->focus_on_init=false;
    this->is_forward_compatable=false;
    this->WindowAspectRatio.numerator=16;
    this->WindowAspectRatio.denumerator=9;
    this->gl_experimental=true;
    this->WindowScreen.full_width= mode->width;
    this->WindowScreen.full_height= mode->height;
    
}
GLX::~GLX(){
    this->destroy();
}
int GLX::glx_primary_monitor_height(){
    return this->WindowScreen.full_height;
}
int GLX::glx_primary_monitor_width(){
    return this->WindowScreen.full_width;
}

void GLX::setVersionMajor(int version_major){
    this->open_gl_version_major=version_major;
}

void GLX::setGLExperimental(bool experimental){
    this->gl_experimental=experimental;
}

void GLX::setVersionMinor(int version_minor){
    this->open_gl_version_minor=version_minor;
}
void GLX::setFocusOnInit(bool focus_on_init){
    this->focus_on_init=focus_on_init;
}
void GLX::setWindowWidth(int window_width){
    this->Window_Width=window_width;
}
void GLX::setWindowHeight(int window_height){
    this->Window_Height=window_height;
}
void GLX::setWindowTitle(std::string window_title){
    this->Window_title=window_title;
}
void GLX::setIsForwardCompatable(bool is_forward_compatable){
    this->is_forward_compatable=is_forward_compatable;
}
void GLX::setAspectRatio(int nume, int denume){
    this->WindowAspectRatio.numerator = nume;
    this->WindowAspectRatio.denumerator = denume;
}


void GLX::destroy(){
    this->tasklist.clear();
    this->postLaunchQueue.clear();
    this->is_running=false;
    glfwDestroyWindow(this->window);
    glfwTerminate();
}
void GLX::inf(){
    std::cout<<"GLX Version:\t"<<this->version<<"\n";
    std::cout<<"Vendor:\t"<<glGetString(GL_VENDOR)<<"\n";
    std::cout<<"Renderer:\t"<<glGetString(GL_RENDERER)<<"\n";
    std::cout<<"Version:\t"<<glGetString(GL_VERSION)<<"\n";
    std::cout<<"Shader Version:\t"<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<"\n";
}
bool GLX::status(){
    return this->is_running && glfwGetCurrentContext()!=nullptr;
}

bool GLX::launch(){
    GLenum err;
    try
    {
        //responsible for setting up 
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->open_gl_version_major); //3.
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->open_gl_version_minor); //3
        glfwWindowHint(GLFW_FOCUS_ON_SHOW,this->focus_on_init?GLFW_TRUE:GLFW_FALSE);
        glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,this->is_forward_compatable?GLFW_TRUE:GLFW_FALSE);
        //create GLFW Window
        this->window = glfwCreateWindow(this->Window_Width,this->Window_Height,this->Window_title.c_str(),NULL,NULL);
        if (!this->window)
        {
            std::cout << "Failed To Init GLX:: Error occured when initializing Window.\n";
            glfwTerminate();
            return false;
        }
        glfwSetWindowAspectRatio(this->window,this->WindowAspectRatio.numerator,this->WindowAspectRatio.denumerator);
        glfwGetFramebufferSize(this->window,&this->frame_buffer_width,&this->frame_buffer_height);
        glfwMakeContextCurrent(this->window);
        glewExperimental=this->gl_experimental?GL_TRUE:GL_FALSE;
        if (glewInit()!=GLEW_OK)
        {
            std::cout << "GLX Failed To Init GLEW:: Error occured when initializing GLEW.\n";
            glfwDestroyWindow(this->window);
            glfwTerminate();
            return false;
        }
        
        glViewport(0, 0, this->Window_Width,this->Window_Height);
        this->is_running=true;
        //run post launch queue
        for (auto& task:this->postLaunchQueue)
        {
            try
            {
                task();
            }catch (...){
                std::cout<<"post launch queue error\n";
                err = glGetError();
                if (err != GL_NO_ERROR){
                    std::cout<<"Open GL error"<<err<<"\n";
                }
            }
        }
        while (!glfwWindowShouldClose(this->window))
        {
            glfwPollEvents();
            for (auto& task:this->tasklist)
            {
                try
                {
                    task();
                }catch (...){
                    try{
                        task();
                    }catch (...){
                        std::cout<<"GLX:: onTick error\n";
                        err = glGetError();
                        if (err != GL_NO_ERROR)
                        {
                            std::cout<<"Open GL error"<<err<<"\n";
                        }
                    }
                }
            }
            glfwSwapBuffers(this->window);
        }
        this->is_running=false;
        return true;
    }catch (...)
    {
        
        this->is_running=false;
        glfwTerminate();
        std::cout << "Failed To launch GLX.\n";
        err = glGetError();
        if (err != GL_NO_ERROR)
        {
            std::cout<<"Open GL error"<<err<<"\n";
        }
        return false;
    }
}



