

# ======================== cmake minimum version ========================
cmake_minimum_required(VERSION 3.31)
# ======================== cmake minimum version ========================

# ======================== project variables ========================
set(PROJECT_NAME "mcclone")
set(PROJECT_VERSION "1.0.1")
set(PROJECT_AUTHOR "sleepyfish")
set(PROJECT_GITHUB "https://github.com/${PROJECT_AUTHOR}-yt/${PROJECT_NAME}")
# ======================== project variables ========================

# ======================== project ========================
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX C)
# ======================== project ========================

# ======================== cxx standard ========================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# ======================== cxx standard ========================

# ======================== c standard ========================
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
# ======================== c standard ========================

# ======================== render dependencies ========================
# used for rendering
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
# ======================== render dependencies ========================

# ======================== openal dependency ========================
# used for audio
find_package(OpenAL CONFIG REQUIRED)
# ======================== openal dependency ========================

# ======================== stb dependency ========================
# used for image loading
find_package(stb REQUIRED)
# ======================== stb dependency ========================

# ======================== json dependency ========================
# used for configuration
find_package(nlohmann_json CONFIG REQUIRED)
# ======================== json dependency ========================

# ======================== files ========================
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src/me/${PROJECT_AUTHOR}")

add_executable(${PROJECT_NAME}
        # main file
        main.cpp

        # client/main
        ${SRC_DIR}/client/main/Main.cpp
        ${SRC_DIR}/client/main/Main.h
        ${SRC_DIR}/client/main/GameConfiguration.cpp
        ${SRC_DIR}/client/main/GameConfiguration.h

        # client/
        ${SRC_DIR}/client/Minecraft.cpp
        ${SRC_DIR}/client/Minecraft.h
        ${SRC_DIR}/client/AnvilConverterException.h
        ${SRC_DIR}/client/ClientBrandRetriever.h

        # client/settings
        ${SRC_DIR}/client/settings/KeyBinding.cpp
        ${SRC_DIR}/client/settings/KeyBinding.h
        ${SRC_DIR}/client/settings/GameSettings.cpp
        ${SRC_DIR}/client/settings/GameSettings.h

        # client/audio - engine
        ${SRC_DIR}/client/audio/SoundEngine.cpp
        ${SRC_DIR}/client/audio/SoundEngine.h
        # client/audio - others
        ${SRC_DIR}/client/audio/ISound.h
        ${SRC_DIR}/client/audio/ISoundEventAccessor.h
        ${SRC_DIR}/client/audio/SoundEventAccessor.h
        ${SRC_DIR}/client/audio/ITickableSound.h
        ${SRC_DIR}/client/audio/SoundPoolEntry.h
        ${SRC_DIR}/client/audio/SoundEventAccessorComposite.h
        ${SRC_DIR}/client/audio/SoundRegistry.h
        ${SRC_DIR}/client/audio/PositionedSound.h
        ${SRC_DIR}/client/audio/MovingSound.h
        ${SRC_DIR}/client/audio/PositionedSoundRecord.h
        ${SRC_DIR}/client/audio/SoundCategory.h
        ${SRC_DIR}/client/audio/SoundList.h

        # client/renderer
        ${SRC_DIR}/client/renderer/StitcherException.h

        # client/renderer/culling
        ${SRC_DIR}/client/renderer/culling/ICamera.h

        # debug
        ${SRC_DIR}/debug/Logger.cpp
        ${SRC_DIR}/debug/Logger.h

        # block/material
        ${SRC_DIR}/block/material/MapColor.h
        ${SRC_DIR}/block/material/MapColors.h
        ${SRC_DIR}/block/material/Material.h
        ${SRC_DIR}/block/material/Materials.h
        ${SRC_DIR}/block/material/MaterialTransparent.h
        ${SRC_DIR}/block/material/MaterialPortal.h
        ${SRC_DIR}/block/material/MaterialLogic.h
        ${SRC_DIR}/block/material/MaterialLiquid.h

        # block/properties
        ${SRC_DIR}/block/properties/IProperty.h
        ${SRC_DIR}/block/properties/PropertyBool.h
        ${SRC_DIR}/block/properties/PropertyDirection.h
        ${SRC_DIR}/block/properties/PropertyEnum.h
        ${SRC_DIR}/block/properties/PropertyHelper.h
        ${SRC_DIR}/block/properties/PropertyInteger.h

        # profiler
        ${SRC_DIR}/profiler/Profiler.cpp
        ${SRC_DIR}/profiler/Profiler.h

        # window render
        ${SRC_DIR}/render/RenderInformation.cpp
        ${SRC_DIR}/render/RenderInformation.h
        ${SRC_DIR}/render/OpenGLWindow.cpp
        ${SRC_DIR}/render/OpenGLWindow.h

        # util
        ${SRC_DIR}/util/Runnable.h
        ${SRC_DIR}/util/Util.cpp
        ${SRC_DIR}/util/Util.h
        ${SRC_DIR}/util/Vec4b.h
        ${SRC_DIR}/util/Vector3d.h
        ${SRC_DIR}/util/WeightedRandom.cpp
        ${SRC_DIR}/util/WeightedRandom.h
        ${SRC_DIR}/util/Vec3i.h
        ${SRC_DIR}/util/Vec3.h
        ${SRC_DIR}/util/EnumFacing.h
        ${SRC_DIR}/util/MovingObjectPosition.h
        ${SRC_DIR}/util/BlockPos.h
        ${SRC_DIR}/util/AxisAlignedBB.h
        ${SRC_DIR}/util/Tuple.h
        ${SRC_DIR}/util/Timer.cpp
        ${SRC_DIR}/util/Timer.h
        ${SRC_DIR}/util/StringUtils.h
        ${SRC_DIR}/util/ReportedException.h
        ${SRC_DIR}/util/MovementInput.h
        ${SRC_DIR}/util/MovementInputFromOptions.cpp
        ${SRC_DIR}/util/MovementInputFromOptions.h
        ${SRC_DIR}/util/MouseFilter.h
        ${SRC_DIR}/util/McCloneError.h
        ${SRC_DIR}/util/Matrix4f.h
        ${SRC_DIR}/util/MathUtils.h
        ${SRC_DIR}/util/MathHelper.h
        ${SRC_DIR}/util/LazyLoadBase.h
        ${SRC_DIR}/util/ITickable.h
        ${SRC_DIR}/util/IStringSerializable.h
        ${SRC_DIR}/util/IProgressUpdate.h
        ${SRC_DIR}/util/IRegistry.h
        ${SRC_DIR}/util/RegistrySimple.h
        ${SRC_DIR}/util/FrameTimer.h
        ${SRC_DIR}/util/EnumWorldBlockLayer.h
        ${SRC_DIR}/util/EnchantmentNameParts.h
        ${SRC_DIR}/util/ChatComponentTranslationFormatException.h
        ${SRC_DIR}/util/ChatAllowedCharacters.h
        ${SRC_DIR}/util/ResourceLocation.h

        # nbt - i hate nbt...
        ${SRC_DIR}/nbt/NBTException.h
        ${SRC_DIR}/nbt/NBTBase.cpp
        ${SRC_DIR}/nbt/NBTBase.h
        ${SRC_DIR}/nbt/NBTPrimitive.h
        ${SRC_DIR}/nbt/NBTSizeTracker.cpp
        ${SRC_DIR}/nbt/NBTSizeTracker.h
        ${SRC_DIR}/nbt/NBTTagByte.h
        ${SRC_DIR}/nbt/NBTTagByteArray.h
        ${SRC_DIR}/nbt/NBTTagDouble.h
        ${SRC_DIR}/nbt/NBTTagEnd.h
        ${SRC_DIR}/nbt/NBTTagList.h
        ${SRC_DIR}/nbt/NBTTagCompound.h
        ${SRC_DIR}/nbt/NBTTagString.h
        ${SRC_DIR}/nbt/NBTTagShort.h
        ${SRC_DIR}/nbt/NBTTagLong.h
        ${SRC_DIR}/nbt/NBTTagInt.h
        ${SRC_DIR}/nbt/NBTTagIntArray.h
        ${SRC_DIR}/nbt/NBTTagFloat.h

        # command
        ${SRC_DIR}/command/ICommandSender.h
        ${SRC_DIR}/command/CommandException.h
        ${SRC_DIR}/command/ICommand.h
        ${SRC_DIR}/command/WrongUsageException.h
        ${SRC_DIR}/command/SyntaxErrorException.h
        ${SRC_DIR}/command/PlayerNotFoundException.h
        ${SRC_DIR}/command/NumberInvalidException.h
        ${SRC_DIR}/command/EntityNotFoundException.h
        ${SRC_DIR}/command/CommandNotFoundException.h

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
# Keep in mind:
# -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
#
#
# ======================== other information ========================

