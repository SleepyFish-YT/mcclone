# cmake
cmake_minimum_required(VERSION 3.31)

# project
project(mcclone)

# c++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# opengl
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad REQUIRED)
find_package(glm REQUIRED)
# openal
find_package(OpenAL CONFIG REQUIRED)
# stb
find_package(stb REQUIRED)
# other
find_package(nlohmann_json REQUIRED)

# files
add_executable(${PROJECT_NAME}
        # main file
        main.cpp

        # src
        src/me/sleepyfish/client/main/Main.cpp
        src/me/sleepyfish/client/main/Main.h
        src/me/sleepyfish/client/main/GameConfiguration.cpp
        src/me/sleepyfish/client/main/GameConfiguration.h
        src/me/sleepyfish/render/RenderInformation.cpp
        src/me/sleepyfish/render/RenderInformation.h
        src/me/sleepyfish/render/OpenGLWindow.cpp
        src/me/sleepyfish/render/OpenGLWindow.h
        src/me/sleepyfish/debug/Logger.cpp
        src/me/sleepyfish/debug/Logger.h
        src/me/sleepyfish/client/Minecraft.cpp
        src/me/sleepyfish/client/Minecraft.h
        src/me/sleepyfish/util/Runnable.h
        src/me/sleepyfish/profiler/Profiler.cpp
        src/me/sleepyfish/profiler/Profiler.h
        src/me/sleepyfish/client/settings/KeyBinding.cpp
        src/me/sleepyfish/client/settings/KeyBinding.h
        src/me/sleepyfish/client/settings/GameSettings.cpp
        src/me/sleepyfish/client/settings/GameSettings.h
        src/me/sleepyfish/audio/SoundEngine.cpp
        src/me/sleepyfish/audio/SoundEngine.h

)

# linking libraries
target_link_libraries(${PROJECT_NAME}
        OpenGL::GL
        glfw
        glad::glad
        glm::glm
        OpenAL::OpenAL
        nlohmann_json::nlohmann_json
        winmm
)

# include the header only libraries
target_include_directories(${PROJECT_NAME} PRIVATE
        ${stb_INCLUDE_DIRS}
)

