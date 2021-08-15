#include "Memory/MemoryCheck.h"
#include "Utils/AudioChecker.h"
#include "Utils/FindRootDir.h"
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

using namespace Rocket;

void processInput(GLFWwindow *window);

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
    std::filesystem::path root_path = root + "\\Asset\\Music\\";
    std::filesystem::directory_iterator dir_list(root_path);
    std::vector<std::string> wav_files;
    for (auto& it:dir_list) {
        std::string name = it.path().filename().string();
        //std::cout << "File Name: " << root_path.string() + name << std::endl;
        wav_files.push_back(root_path.string() + name);
    }

    ALCdevice* openALDevice = alcOpenDevice(nullptr);
    if(!openALDevice)
        return 0;

    ALCcontext* openALContext;
    if(!alcCall(alcCreateContext, openALContext, openALDevice, openALDevice, nullptr) || !openALContext) {
        std::cerr << "ERROR: Could not create audio context" << std::endl;
        return 0;
    }
    ALCboolean contextMadeCurrent = false;
    if(!alcCall(alcMakeContextCurrent, contextMadeCurrent, openALDevice, openALContext) 
        || contextMadeCurrent != ALC_TRUE) {
        std::cerr << "ERROR: Could not make audio context current" << std::endl;
        return 0;
    }

    std::vector<ALuint> buffers;
    std::vector<ALuint> sources;

    for(int i = 0; i < 88; i++) {
        auto name = wav_files[i];
        SNDFILE* file = nullptr;
        SF_INFO info;
        file = sf_open(name.c_str(), SFM_READ, &info);

        // TODO : use info.format to decode bit per sample and eiden etc.
        ALenum format = AL_NONE;
        if(info.channels == 1)
            format = AL_FORMAT_MONO16;
        else if(info.channels == 2)
            format = AL_FORMAT_STEREO16;
        else if(info.channels == 3) {
            if(sf_command(file, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
                format = AL_FORMAT_BFORMAT2D_16;
        }
        else if(info.channels == 4) {
            if(sf_command(file, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
                format = AL_FORMAT_BFORMAT3D_16;
        }

        // Decode the whole audio file to a buffer
        int16_t* data = new int16_t[info.frames * info.channels];
        sf_count_t num_frames = sf_readf_short(file, data, info.frames);
        ALsizei num_bytes = (ALsizei)(num_frames * info.channels) * (ALsizei)sizeof(int16_t);

        ALuint buffer;
        alCall(alGenBuffers, 1, &buffer);
        alCall(alBufferData, buffer, format, data, num_bytes, info.samplerate);
        delete[] data;
        sf_close(file);

        auto err = alGetError();
        if(err != AL_NO_ERROR) {
            fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
            if(buffer && alIsBuffer(buffer))
                alDeleteBuffers(1, &buffer);
            return 0;
        }

        ALuint source;
        alCall(alGenSources, 1, &source);
        alCall(alSourcei, source, AL_BUFFER, buffer);
        assert(alGetError()==AL_NO_ERROR && "Failed to setup sound source");

        buffers.push_back(buffer);
        sources.push_back(source);
    }

    std::default_random_engine rand_engine;
    std::uniform_int_distribution<int32_t> gen_rand(0, buffers.size() - 1);

	while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        int x = gen_rand(rand_engine);

        alCall(alSourcePlay, sources[x]);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    for(int i = 0; i < sources.size(); ++i) {
        alSourceStop(sources[i]);
        alCall(alDeleteSources, 1, &sources[i]);
        alCall(alDeleteBuffers, 1, &buffers[i]);
    }

    alcCall(alcMakeContextCurrent, contextMadeCurrent, openALDevice, nullptr);
    alcCall(alcDestroyContext, openALDevice, openALContext);

    ALCboolean closed;
    alcCall(alcCloseDevice, closed, openALDevice, openALDevice);

	glfwDestroyWindow(window);
	glfwTerminate();

    Log::End();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
