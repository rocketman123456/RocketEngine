# 3. 第一个窗口-GLFW<br>

## 3.1. GLFW简介<br>

## 3.2. 打开窗口<br>
`glfw_test.cpp`
```
#include "Memory/MemoryCheck.h"

//#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>

void processInput(GLFWwindow *window);

int main() {
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);
#if defined(RK_MACOS)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Rocket", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

	while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
```
## 3.3. 程序主循环<br>
以下代码仅用于展示程序运行的主流程，还未使用在实际项目中。<br>
`MainLoop.cpp`
```
#include "Memory/MemoryCheck.h"
#include "Pattern/IRuntimeModule.h"
#include "Containers/Bag/Bag.h"
#include "Log/Log.h"

using namespace Rocket;

int main() {
    Bag<IRuntimeModule*> modules;

    {
        auto iter = modules.GetIterator();
        while(iter.HasNext()) {
            IRuntimeModule* module = iter.Next();
            auto result = module->Initialize();
            if(result != 0) {
                RK_ERROR(Core, "Module {} Initialize Failed", module->ToString());
            }
        }
    }

    while(1) {
        auto iter = modules.GetIterator();
        while(iter.HasNext()) {
            IRuntimeModule* module = iter.Next();
            module->Tick(0.01);
        }
    }

    {
        auto iter = modules.GetIterator();
        while(iter.HasNext()) {
            IRuntimeModule* module = iter.Next();
            module->Finalize();
        }
    }

    return 0;
}
```
