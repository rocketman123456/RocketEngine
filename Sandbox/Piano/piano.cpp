#include "Memory/MemoryCheck.h"
#include "Utils/AudioChecker.h"
#include "Utils/FindRootDir.h"
#include "FileSystem/FileSystem.h"
#include "AudioSystem/AudioManager.h"
#include "Log/Log.h"

//#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <sndfile.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include <iostream>
#include <string> 
#include <vector>
#include <random>
#include <filesystem>
#include <memory>

using namespace Rocket;

#define generate_task(X) {\
    auto buffer = AudioManager::Instance()->FindBuffer(X);\
    AudioTaskPtr task = std::make_unique<AudioTask>();\
    task->Initialize(buffer);\
    AudioManager::Instance()->AddTask(std::move(task));}

int main() {
    Log::Init();

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

    std::string root = FindRootDir("_root_dir_");
    std::filesystem::path root_path = root + "/Asset/Music/";
    std::filesystem::directory_iterator dir_list(root_path);
    std::vector<std::string> wav_files;
    std::vector<std::string> wav_file_names;
    for (auto& it:dir_list) {
        std::string name = it.path().filename().string();
        wav_files.push_back(root_path.string() + name);
        wav_file_names.push_back(name);
    }

    AudioManager::Create();
    auto result = AudioManager::Instance()->Initialize();
    if(result) {
        return 0;
    }

    for(int i = 0; i < wav_files.size(); ++i) {
        AudioFilePtr file_ptr = FileSystem::OpenAudio(root_path.string(), wav_file_names[i], FileOperateMode::ReadBinary);
        AudioManager::Instance()->InsertBuffer(file_ptr.get());
        FileSystem::CloseAudio(std::move(file_ptr));
    }

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
            generate_task("Piano.ff.C2.wav");
        }
        if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
            generate_task("Piano.ff.D2.wav");
        }
        if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
            generate_task("Piano.ff.E2.wav");
        }
        if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
            generate_task("Piano.ff.F2.wav");
        }
        if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
            generate_task("Piano.ff.G2.wav");
        }
        if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
            generate_task("Piano.ff.A2.wav");
        }
        if (key == GLFW_KEY_7 && action == GLFW_PRESS) {
            generate_task("Piano.ff.B2.wav");
        }
        if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
            generate_task("Piano.ff.C3.wav");
        }
        if (key == GLFW_KEY_W && action == GLFW_PRESS) {
            generate_task("Piano.ff.D3.wav");
        }
        if (key == GLFW_KEY_E && action == GLFW_PRESS) {
            generate_task("Piano.ff.E3.wav");
        }
        if (key == GLFW_KEY_R && action == GLFW_PRESS) {
            generate_task("Piano.ff.F3.wav");
        }
        if (key == GLFW_KEY_T && action == GLFW_PRESS) {
            generate_task("Piano.ff.G3.wav");
        }
        if (key == GLFW_KEY_Y && action == GLFW_PRESS) {
            generate_task("Piano.ff.A3.wav");
        }
        if (key == GLFW_KEY_U && action == GLFW_PRESS) {
            generate_task("Piano.ff.B3.wav");
        }
        if (key == GLFW_KEY_A && action == GLFW_PRESS) {
            generate_task("Piano.ff.C4.wav");
        }
        if (key == GLFW_KEY_S && action == GLFW_PRESS) {
            generate_task("Piano.ff.D4.wav");
        }
        if (key == GLFW_KEY_D && action == GLFW_PRESS) {
            generate_task("Piano.ff.E4.wav");
        }
        if (key == GLFW_KEY_F && action == GLFW_PRESS) {
            generate_task("Piano.ff.F4.wav");
        }
        if (key == GLFW_KEY_G && action == GLFW_PRESS) {
            generate_task("Piano.ff.G4.wav");
        }
        if (key == GLFW_KEY_H && action == GLFW_PRESS) {
            generate_task("Piano.ff.A4.wav");
        }
        if (key == GLFW_KEY_J && action == GLFW_PRESS) {
            generate_task("Piano.ff.B4.wav");
        }
    });

	while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        AudioManager::Instance()->Tick(10);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    AudioManager::Instance()->Finalize();

	glfwDestroyWindow(window);
	glfwTerminate();

    Log::End();
    return 0;
}
