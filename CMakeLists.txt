# ======================== project variables ========================
set(PROJECT_NAME "mcclone")
set(PROJECT_VERSION "1.0.1")
set(PROJECT_AUTHOR "sleepyfish")
set(PROJECT_GITHUB "https://github.com/${PROJECT_AUTHOR}-yt/${PROJECT_NAME}")
# ======================== project variables ========================

# ======================== cmake ========================
cmake_minimum_required(VERSION 3.31)
# ======================== cmake ========================

# ======================== project ========================
project(${PROJECT_NAME})
# ======================== project ========================

# ======================== cxx ========================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# ======================== cxx ========================

# ======================== c ========================
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
# ======================== c ========================

# ======================== render packages ========================
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad REQUIRED)
find_package(glm REQUIRED)
# ======================== render packages ========================

# ======================== openal ========================
find_package(OpenAL CONFIG REQUIRED)
# ======================== openal ========================

# ======================== stb ========================
find_package(stb REQUIRED)
# ======================== stb ========================

# ======================== json ========================
find_package(nlohmann_json REQUIRED)
# ======================== json ========================

# ======================== files ========================
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src/me/${PROJECT_AUTHOR}")

add_executable(${PROJECT_NAME}
        # main file
        main.cpp

        # src
        ${SRC_DIR}/client/main/Main.cpp
        ${SRC_DIR}/client/main/Main.h
        ${SRC_DIR}/client/main/GameConfiguration.cpp
        ${SRC_DIR}/client/main/GameConfiguration.h

        ${SRC_DIR}/client/Minecraft.cpp
        ${SRC_DIR}/client/Minecraft.h

        ${SRC_DIR}/client/settings/KeyBinding.cpp
        ${SRC_DIR}/client/settings/KeyBinding.h
        ${SRC_DIR}/client/settings/GameSettings.cpp
        ${SRC_DIR}/client/settings/GameSettings.h
        ${SRC_DIR}/client/audio/SoundEngine.cpp
        ${SRC_DIR}/client/audio/SoundEngine.h

        ${SRC_DIR}/render/RenderInformation.cpp
        ${SRC_DIR}/render/RenderInformation.h
        ${SRC_DIR}/render/OpenGLWindow.cpp
        ${SRC_DIR}/render/OpenGLWindow.h
        ${SRC_DIR}/debug/Logger.cpp
        ${SRC_DIR}/debug/Logger.h

        ${SRC_DIR}/util/Runnable.h
        ${SRC_DIR}/profiler/Profiler.cpp
        ${SRC_DIR}/profiler/Profiler.h


)
# ======================== files ========================

# ======================== linking libraries ========================
target_link_libraries(${PROJECT_NAME}
        OpenGL::GL
        glfw
        glad::glad
        glm::glm
        OpenAL::OpenAL
        nlohmann_json::nlohmann_json
        winmm
)
# ======================== linking libraries ========================

# ======================== include the header only libraries ========================
target_include_directories(${PROJECT_NAME} PRIVATE
        ${stb_INCLUDE_DIRS}
)
# ======================== include the header only libraries ========================

# ======================== other information ========================
# IntelliJ IDEA Project: src (C:/Users/Luca/Desktop/-/-/java/mcp1.8.9op/src)
#
#
#
#
#
# ======================== other information ========================
