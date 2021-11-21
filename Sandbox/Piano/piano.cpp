#include "Memory/MemoryCheck.h"
#include "FileSystem/FileSystem.h"
#include "AudioSystem/AudioManager.h"
#include "EventSystem/EventManager.h"
#include "Utils/AudioChecker.h"
#include "Utils/FindRootDir.h"
#include "Utils/Timer.h"
#include "Log/Log.h"
#include "Parser/JsonParser.h"
#include "AudioSystem/MusicGenerator.h"

//#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <sndfile.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <string> 
#include <vector>
#include <random>
#include <filesystem>
#include <memory>
#include <thread>

using namespace Rocket;

void InsertEvents(std::vector<EventPtr>& events) {
    for(auto event : events) {
        EventManager::Instance()->QueueEvent(event);
    }
}

void GenerateEvent(const std::string& name) {
    Variant* data = new Variant[1];
    data[0].type = Variant::TYPE_STRING_ID;
    data[0].as_string_id = hash(name);
    EventPtr event = EventPtr(new Event("audio", data, 1));
    EventManager::Instance()->TriggerEvent(event);
}

bool AudioEventHandle(EventPtr& event) {
    auto name_id = event->GetStringId(0);
    auto buffer = AudioManager::Instance()->FindBuffer(name_id);
    AudioTaskPtr task = std::make_unique<AudioTask>();
    task->Initialize(buffer);
    AudioManager::Instance()->AddTask(std::move(task));
    return false;
}

MusicGenerator generator_g;

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

	GLFWwindow* window = glfwCreateWindow(640, 480, "Rocket", NULL, NULL);
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
        std::string type_name = name.substr(name.find_last_of('.')+1);
        if(type_name == "wav" || type_name == "ogg") {
            wav_files.push_back(root_path.string() + name);
            wav_file_names.push_back(name);
        }
    }

    std::string file_name = root + "/Logo/LOGO-ICON-02.png";

    // TODO : replace with engine load function
    int x,y,n;
    unsigned char* data = stbi_load(file_name.c_str(), &x, &y, &n, 4);
    GLFWimage image = {x, y, data};
    glfwSetWindowIcon(window, 1, &image);

    FileBuffer buffer;
    FileSystem::LoadSync(root, "/Asset/Config/music_name.txt", FileOperateMode::READ_TEXT, buffer);
    std::string music_name = (char*)buffer.buffer;

    JsonParserPtr parser = JsonParserPtr(new JsonParser);
    std::string note_name = "/Asset/Note/" + music_name + ".json";
    parser->Initialize(root, note_name);
    generator_g.Initialize(std::move(parser));

    EventManager::Create();
    auto result = EventManager::Instance()->Initialize();
    if(result) {
        return 1;
    }
    ChannelPtr channel = std::shared_ptr<EventChannel>(new EventChannel("audio_channel"));
    EventManager::Instance()->AddChannel("audio", channel);
    REGISTER_DELEGATE_FN(AudioEventHandle, "audio", "audio_channel");

    AudioManager::Create();
    result = AudioManager::Instance()->Initialize();
    if(result) {
        return 1;
    }

    for(int i = 0; i < wav_files.size(); ++i) {
        AudioFilePtr file_ptr = FileSystem::OpenAudio(root_path.string(), wav_file_names[i], FileOperateMode::READ_BINARY);
        AudioManager::Instance()->InsertBuffer(file_ptr.get());
        FileSystem::CloseAudio(std::move(file_ptr));
    }

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {glfwSetWindowShouldClose(window, true);}
        if (key == GLFW_KEY_1 && action == GLFW_PRESS) {GenerateEvent("Piano.ff.C2.wav");}
        if (key == GLFW_KEY_2 && action == GLFW_PRESS) {GenerateEvent("Piano.ff.D2.wav");}
        if (key == GLFW_KEY_3 && action == GLFW_PRESS) {GenerateEvent("Piano.ff.E2.wav");}
        if (key == GLFW_KEY_4 && action == GLFW_PRESS) {GenerateEvent("Piano.ff.F2.wav");}
        if (key == GLFW_KEY_5 && action == GLFW_PRESS) {GenerateEvent("Piano.ff.G2.wav");}
        if (key == GLFW_KEY_6 && action == GLFW_PRESS) {GenerateEvent("Piano.ff.A2.wav");}
        if (key == GLFW_KEY_7 && action == GLFW_PRESS) {GenerateEvent("Piano.ff.B2.wav");}
        if (key == GLFW_KEY_Q && action == GLFW_PRESS) {GenerateEvent("Piano.ff.C3.wav");}
        if (key == GLFW_KEY_W && action == GLFW_PRESS) {GenerateEvent("Piano.ff.D3.wav");}
        if (key == GLFW_KEY_E && action == GLFW_PRESS) {GenerateEvent("Piano.ff.E3.wav");}
        if (key == GLFW_KEY_R && action == GLFW_PRESS) {GenerateEvent("Piano.ff.F3.wav");}
        if (key == GLFW_KEY_T && action == GLFW_PRESS) {GenerateEvent("Piano.ff.G3.wav");}
        if (key == GLFW_KEY_Y && action == GLFW_PRESS) {GenerateEvent("Piano.ff.A3.wav");}
        if (key == GLFW_KEY_U && action == GLFW_PRESS) {GenerateEvent("Piano.ff.B3.wav");}
        if (key == GLFW_KEY_A && action == GLFW_PRESS) {GenerateEvent("Piano.ff.C4.wav");}
        if (key == GLFW_KEY_S && action == GLFW_PRESS) {GenerateEvent("Piano.ff.D4.wav");}
        if (key == GLFW_KEY_D && action == GLFW_PRESS) {GenerateEvent("Piano.ff.E4.wav");}
        if (key == GLFW_KEY_F && action == GLFW_PRESS) {GenerateEvent("Piano.ff.F4.wav");}
        if (key == GLFW_KEY_G && action == GLFW_PRESS) {GenerateEvent("Piano.ff.G4.wav");}
        if (key == GLFW_KEY_H && action == GLFW_PRESS) {GenerateEvent("Piano.ff.A4.wav");}
        if (key == GLFW_KEY_J && action == GLFW_PRESS) {GenerateEvent("Piano.ff.B4.wav");}
        if (key == GLFW_KEY_Z && action == GLFW_PRESS) {GenerateEvent("Piano.ff.C5.wav");}
        if (key == GLFW_KEY_X && action == GLFW_PRESS) {GenerateEvent("Piano.ff.D5.wav");}
        if (key == GLFW_KEY_C && action == GLFW_PRESS) {GenerateEvent("Piano.ff.E5.wav");}
        if (key == GLFW_KEY_V && action == GLFW_PRESS) {GenerateEvent("Piano.ff.F5.wav");}
        if (key == GLFW_KEY_B && action == GLFW_PRESS) {GenerateEvent("Piano.ff.G5.wav");}
        if (key == GLFW_KEY_N && action == GLFW_PRESS) {GenerateEvent("Piano.ff.A5.wav");}
        if (key == GLFW_KEY_M && action == GLFW_PRESS) {GenerateEvent("Piano.ff.B5.wav");}
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) { 
            std::vector<EventPtr> events;
            generator_g.GetMusicNotes(events);
            RK_INFO(App, "Event Count: {}", events.size());
            InsertEvents(events); 
        }
    });

    ElapseTimer timer;
    timer.Start();

    bool isStop = false;

    std::thread module_thread = std::thread([&](){
        static int32_t count = 0;
        static double count_dt = 0;
        while(!isStop) {
            double dt = timer.GetTickTime();
            //RK_TRACE(App, "dt: {}", dt);
            AudioManager::Instance()->Tick(dt);
            EventManager::Instance()->Tick(dt);
            count++;
            count_dt += dt;
            if(count_dt >= 1000.0) {
                RK_INFO(App, "Loop Count : {}", count);
                count_dt = 0;
                count = 0;
            }
        }
    });

	while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    isStop = true;
    module_thread.join();

    generator_g.Finalize();

    AudioManager::Instance()->Finalize();
    EventManager::Instance()->Finalize();

	glfwDestroyWindow(window);
	glfwTerminate();

    AudioManager::Destroy();
    EventManager::Destroy();
    Log::End();
    return 0;
}
