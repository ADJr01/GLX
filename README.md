# 🌐 GLX - A Modern OpenGL Context Handler for C++

GLX is a lightweight wrapper around **GLFW** and **GLEW** that simplifies the process of creating and managing an OpenGL context Easily. Built with modern C++ practices, it allows for quick bootstrapping of graphics applications with minimal boilerplate.
#### 🎯 GLX Follows Modern RAII-based Resource Management techniques
---

## 📦 Features

* Simple and consistent Cross-Platform API for OpenGL context setup.
* Easy-to-use event hooks (`onTick`, `addPostLaunchProcedure`).
* Configurable window dimensions, title, focus, and OpenGL versions.
* Smart encapsulation of GLFW and GLEW logic.
* Minimal, maintainable, and extendable structure.

---

## 🛠️ Dependencies

* [GLFW](https://www.glfw.org/)
* [GLEW](http://glew.sourceforge.net/)
* C++17 or later

---

## 💠 Class: `GLX`

This is the core class that handles the OpenGL context and window system. Below is a documentation of each method grouped by functionality.
---

### 🧱 Constructor & Destructor

```cpp
GLX();
~GLX();
```

* **GLX()**: Initializes internal states and prepares for context setup. Calling the Constructor will initialize `GLFW`
* **\~GLX()**: Destroys the context carefully and releases resources.

---

### 🚟️ Window Configuration

```cpp
void setWindowTitle(std::string window_title);
void setWindowWidth(int window_width);
void setWindowHeight(int window_height);
void setAspectRatio(int nume, int denume);
```

* `setWindowTitle(...)`: Set the window title.
* `setWindowWidth(...)`: Set desired window width.
* `setWindowHeight(...)`: Set desired window height.
* `setAspectRatio(nume, denume)`: Set the fixed aspect ratio (numerator/denominator).

---

### 🥪 OpenGL Context Configuration

```cpp
void setGLExperimental(bool experimental);
void setVersionMajor(int ver_major);
void setVersionMinor(int ver_minor);
void setFocusOnInit(bool focus_on_init);
void setIsForwardCompatable(bool is_forward_compatable);
```

* `setGLExperimental(...)`: Enable GLEW experimental flag for modern OpenGL extensions.
* `setVersionMajor(...)`: Specify major OpenGL version (e.g., 4 for OpenGL 4.5).
* `setVersionMinor(...)`: Specify minor OpenGL version (e.g., 5 for OpenGL 4.5).
* `setFocusOnInit(...)`: Request initial focus for the window on launch.
* `setIsForwardCompatable(...)`: Use a forward-compatible OpenGL profile.

---

### 🗂️ Information Utilities

```cpp
void inf();
int glx_primary_monitor_width();
int glx_primary_monitor_height();
bool status();
```

* `inf()`: Prints the current configuration: GLX-Version,Vendor,Renderer, Version & Shader Version.
* `glx_primary_monitor_width()`: Returns the width of the primary monitor.
* `glx_primary_monitor_height()`: Returns the height of the primary monitor.
* `status()`: Returns the current running state of the GLX window.

---

### 🔀 Main Lifecycle Hook

### 🧨 Post-Launch Tasks

```cpp
template<typename Func, typename... Args>
void addPostLaunchProcedure(Func func, Args... args);
```

* `addPostLaunchProcedure(...)`: Schedule a task to be executed **once**, immediately before the window has been launched and OpenGL context is made current. you can pass functions you want to run Immediuate before tick starts.

---

```cpp
bool launch();
```

* `launch()`: Finalizes the setup and starts the glx system. All queued `onTick` functions are called each frame. Any `addPostLaunchProcedure` callbacks are called once after setup. OpenGl context will be initialized on this stage.

---

### 🔁🔁 🔁 Frame Update Tasks

```cpp
template<typename Func, typename... Args>
void onTick(Func func, Args... args);
```

* `onTick(...)`: Schedule a task to be executed on every frame during the render loop. Good for logic, rendering, or input handling functions.

---



## 🔧 Example Usage

```cpp
#include<iostream>
#include<memory>
#include "glx/Glx.h"


int main(){
 
    auto gl =std::make_unique<GLX>();
    gl->setVersionMajor(3); //default 4
    gl->setVersionMinor(3);//default 1
    gl->setAspectRatio(16,9); //default 16:9
    gl->setWindowWidth(static_cast<int>(gl->glx_primary_monitor_width())); //default full width
    gl->setWindowHeight(static_cast<int>(gl->glx_primary_monitor_height())); //default full height
    gl->setWindowTitle("GLX Window");
    gl->setIsForwardCompatable(true); //default true
    gl->setFocusOnInit(true); ////default false
    gl->addPostLaunchProcedure([] {
           //attach functionalities those need to run before tick start
        });

    
    gl->onTick([=]()
    {
            //tick: run tasks need to execute on each frame
        glClearColor(0.0f,0.0f,0.0f,1.f);
        glClear(GL_COLOR_BUFFER_BIT);


    });

    gl->launch(); // will trigger the system
    return 0;
    
}

```
## 🔧 Example of Using ShaderTool from GLX library
```cpp
   int main(){
     std::string frag_src = "path/to/you/fragment.shader"; //fragment shader source
     std::string vert_src = "path/to/you/vertex.shader"; //vertex shader source
     glx.ShaderTool().setVertexShaderPath(vert_src);
     glx.ShaderTool().setFragmentShaderPath(frag_src);
     glx.ShaderTool().buildProgram(); // build program does all the thing for us. Compiling and linking the shader program
     glx.ShaderTool().getProgram(); //to get the Program we'll use this function
     return 0;
   }
```

## 🚧 Features Under Development
### 🛠️ GLX Program Builder 🚧: 

A utility designed to simplify shader and program handling tool:
- Vulkan Support

