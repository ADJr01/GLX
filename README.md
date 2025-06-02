# 🌐 GLX - A Modern OpenGL Context Handler for C++

GLX is a lightweight wrapper around **GLFW** and **GLEW** that simplifies the process of creating and managing an OpenGL context. Built with modern C++ practices, it allows for quick bootstrapping of graphics applications with minimal boilerplate.

---

## 📦 Features

* Simple and consistent API for OpenGL context setup.
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
* **\~GLX()**: Gracefully destroys the context and releases resources.

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
    gl->setVersionMajor(3);
    gl->setVersionMinor(3);
    gl->setAspectRatio(16,9);
    gl->setWindowWidth(static_cast<int>(gl->glx_primary_monitor_width()*0.8));
    gl->setWindowHeight(static_cast<int>(gl->glx_primary_monitor_height()*0.8));
    gl->setWindowTitle("GLX Window");
    gl->setIsForwardCompatable(true);
    gl->setFocusOnInit(true);
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


