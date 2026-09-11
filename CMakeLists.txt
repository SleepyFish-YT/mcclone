

# ======================== cmake minimum version ========================
cmake_minimum_required(VERSION 3.31)
# ======================== cmake minimum version ========================

# ======================== project variables ========================
set(PROJECT_NAME "mcclone")
set(PROJECT_VERSION "1.0.2")
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

# ======================== export commands ========================
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# ======================== export commands ========================

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
find_package(Stb REQUIRED)
# ======================== stb dependency ========================

# ======================== json dependency ========================
# used for configuration
find_package(nlohmann_json CONFIG REQUIRED)
# ======================== json dependency ========================

# ======================== files ========================
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src/me/${PROJECT_AUTHOR}/${PROJECT_NAME}")
set(SVA_DIR "${CMAKE_SOURCE_DIR}/src/me/${PROJECT_AUTHOR}/sava")

add_executable(${PROJECT_NAME}
        # main file
        main.cpp

        # defines
        pcb.h

        # sava
        ${SVA_DIR}/SavaUtil.cpp
        ${SVA_DIR}/SavaUtil.h
        ${SVA_DIR}/Runnable.h
        ${SVA_DIR}/Tuple.h
        ${SVA_DIR}/UUID.h
        ${SVA_DIR}/FutureTaskQueue.h
        ${SVA_DIR}/BufferedImage.h

        # window render
        ${SVA_DIR}/window/RenderInformation.cpp
        ${SVA_DIR}/window/RenderInformation.h
        ${SVA_DIR}/window/OpenGLWindow.cpp
        ${SVA_DIR}/window/OpenGLWindow.h

        # client/main
        ${SRC_DIR}/client/main/Main.cpp
        ${SRC_DIR}/client/main/Main.h
        ${SRC_DIR}/client/main/GameConfiguration.cpp
        ${SRC_DIR}/client/main/GameConfiguration.h

        # client/model
        ${SRC_DIR}/client/model/PositionTextureVertex.h
        ${SRC_DIR}/client/model/TexturedQuad.h
        ${SRC_DIR}/client/model/ModelBox.h
        ${SRC_DIR}/client/model/TextureOffset.h
        ${SRC_DIR}/client/model/ModelBase.h
        ${SRC_DIR}/client/model/ModelRenderer.cpp
        ${SRC_DIR}/client/model/ModelRenderer.h
        ${SRC_DIR}/client/model/ModelSprite.cpp
        ${SRC_DIR}/client/model/ModelSprite.h

        # client/
        ${SRC_DIR}/client/Minecraft.cpp
        ${SRC_DIR}/client/Minecraft.h
        ${SRC_DIR}/client/AnvilConverterException.h
        ${SRC_DIR}/client/ClientBrandRetriever.h

        # client/shader
        ${SRC_DIR}/client/shader/Framebuffer.cpp
        ${SRC_DIR}/client/shader/Framebuffer.h

        # client/gui
        ${SRC_DIR}/client/gui/ScaledResolution.cpp
        ${SRC_DIR}/client/gui/ScaledResolution.h
        ${SRC_DIR}/client/gui/IProgressMeter.h
        ${SRC_DIR}/client/gui/GuiYesNoCallback.h

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
        ${SRC_DIR}/client/renderer/GLAllocation.h
        ${SRC_DIR}/client/renderer/ActiveRenderInfo.h
        ${SRC_DIR}/client/renderer/StitcherException.h
        ${SRC_DIR}/client/renderer/IImageBuffer.h
        ${SRC_DIR}/client/renderer/GlStateManager.cpp
        ${SRC_DIR}/client/renderer/GlStateManager.h
        ${SRC_DIR}/client/renderer/OpenGlHelper.cpp
        ${SRC_DIR}/client/renderer/OpenGlHelper.h
        ${SRC_DIR}/client/renderer/Tessellator.cpp
        ${SRC_DIR}/client/renderer/Tessellator.h
        ${SRC_DIR}/client/renderer/WorldVertexBufferUploader.cpp
        ${SRC_DIR}/client/renderer/WorldVertexBufferUploader.h
        ${SRC_DIR}/client/renderer/WorldRenderer.cpp
        ${SRC_DIR}/client/renderer/WorldRenderer.h

        # client/renderer/vertex
        ${SRC_DIR}/client/renderer/vertex/DefaultVertexFormats.cpp
        ${SRC_DIR}/client/renderer/vertex/DefaultVertexFormats.h
        ${SRC_DIR}/client/renderer/vertex/VertexFormatElement.cpp
        ${SRC_DIR}/client/renderer/vertex/VertexFormatElement.h
        ${SRC_DIR}/client/renderer/vertex/VertexFormat.cpp
        ${SRC_DIR}/client/renderer/vertex/VertexFormat.h

        # client/renderer/texture
        ${SRC_DIR}/client/renderer/texture/TextureAtlasSprite.cpp
        ${SRC_DIR}/client/renderer/texture/TextureAtlasSprite.h
        ${SRC_DIR}/client/renderer/texture/TextureUtil.cpp
        ${SRC_DIR}/client/renderer/texture/TextureUtil.h
        ${SRC_DIR}/client/renderer/texture/ITickableTexture.h
        ${SRC_DIR}/client/renderer/texture/TextureClock.cpp
        ${SRC_DIR}/client/renderer/texture/TextureClock.h
        ${SRC_DIR}/client/renderer/texture/TextureCompass.cpp
        ${SRC_DIR}/client/renderer/texture/TextureCompass.h

        # client/renderer/culling
        ${SRC_DIR}/client/renderer/culling/ICamera.h
        ${SRC_DIR}/client/renderer/culling/ClippingHelper.h
        ${SRC_DIR}/client/renderer/culling/ClippingHelperImpl.h
        ${SRC_DIR}/client/renderer/culling/Frustum.h

        # client/resources/data
        ${SRC_DIR}/client/resources/data/IMetadataSection.h
        ${SRC_DIR}/client/resources/data/TextureMetadataSection.h
        ${SRC_DIR}/client/resources/data/AnimationFrame.h
        ${SRC_DIR}/client/resources/data/AnimationMetadataSection.h
        ${SRC_DIR}/client/resources/data/FontMetadataSection.h

        # debug
        ${SRC_DIR}/debug/Logger.cpp
        ${SRC_DIR}/debug/Logger.h

        # config
        ${SRC_DIR}/config/Config.h

        # block/material
        ${SRC_DIR}/block/material/MapColor.h
        ${SRC_DIR}/block/material/MapColors.h
        ${SRC_DIR}/block/material/Material.h
        ${SRC_DIR}/block/material/Materials.h
        ${SRC_DIR}/block/material/MaterialTransparent.h
        ${SRC_DIR}/block/material/MaterialPortal.h
        ${SRC_DIR}/block/material/MaterialLogic.h
        ${SRC_DIR}/block/material/MaterialLiquid.h

        # stats
        ${SRC_DIR}/stats/IStatStringFormat.h
        ${SRC_DIR}/stats/IStatType.h

        # world
        ${SRC_DIR}/world/McCloneException.h
        ${SRC_DIR}/world/EnumDifficulty.h
        ${SRC_DIR}/world/ColorizerFoliage.h
        ${SRC_DIR}/world/WorldType.h
        ${SRC_DIR}/world/WorldSettings.h
        ${SRC_DIR}/world/LockCode.h
        ${SRC_DIR}/world/ChunkCoordIntPair.h

        # world/border
        ${SRC_DIR}/world/border/EnumBorderStatus.h

        # world/gen
        ${SRC_DIR}/world/gen/INoiseGenerator.h

        # world/gen/layer
        ${SRC_DIR}/world/gen/layer/IntCache.h

        # world/storage
        ${SRC_DIR}/world/storage/IThreadedFileIO.h

        # block/properties
        ${SRC_DIR}/block/properties/IProperty.h
        ${SRC_DIR}/block/properties/PropertyBool.h
        ${SRC_DIR}/block/properties/PropertyDirection.h
        ${SRC_DIR}/block/properties/PropertyEnum.h
        ${SRC_DIR}/block/properties/PropertyHelper.h
        ${SRC_DIR}/block/properties/PropertyInteger.h

        # block/state
        ${SRC_DIR}/block/state/IBlockState.h

        # profiler
        ${SRC_DIR}/profiler/Profiler.cpp
        ${SRC_DIR}/profiler/Profiler.h

        # network
        ${SRC_DIR}/network/ThreadQuickExitException.h
        ${SRC_DIR}/network/EnumPacketDirection.h
        ${SRC_DIR}/network/INetHandler.h
        ${SRC_DIR}/network/Packet.h

        # network/handshake
        ${SRC_DIR}/network/handshake/INetHandlerHandshakeServer.h

        # network/handshake/client
        ${SRC_DIR}/network/handshake/client/C00Handshake.h

        # network/login
        ${SRC_DIR}/network/login/INetHandlerLoginClient.h
        ${SRC_DIR}/network/login/INetHandlerLoginServer.h

        # network/play
        ${SRC_DIR}/network/play/INetHandlerPlayClient.h
        ${SRC_DIR}/network/play/INetHandlerPlayServer.h

        # network/status
        ${SRC_DIR}/network/status/INetHandlerStatusClient.h
        ${SRC_DIR}/network/status/INetHandlerStatusServer.h

        # network/status/server
        ${SRC_DIR}/network/status/server/S00PacketServerInfo.h
        ${SRC_DIR}/network/status/server/S01PacketPong.h

        # network/status/client
        ${SRC_DIR}/network/status/client/C00PacketServerQuery.h
        ${SRC_DIR}/network/status/client/C01PacketPing.h

        # util
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
        ${SRC_DIR}/util/EnumChatFormatting.h
        ${SRC_DIR}/util/EnumParticleTypes.h
        ${SRC_DIR}/util/IObjectIntIterable.h
        ${SRC_DIR}/util/RegistryNamespaced.h
        ${SRC_DIR}/util/ObjectIntIdentityMap.h
        ${SRC_DIR}/util/RegistryNamespacedDefaultedByKey.h

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

        # entity
        ${SRC_DIR}/entity/EnumCreatureAttribute.h
        ${SRC_DIR}/entity/INpc.h
        ${SRC_DIR}/entity/IEntityLivingData.h
        ${SRC_DIR}/entity/IProjectile.h

        # entity/passive
        ${SRC_DIR}/entity/passive/IAnimals.h

        # entity/ai/attributes
        ${SRC_DIR}/entity/ai/attributes/IAttribute.h
        ${SRC_DIR}/entity/ai/attributes/IAttributeInstance.h
        ${SRC_DIR}/entity/ai/attributes/BaseAttribute.h
        ${SRC_DIR}/entity/ai/attributes/AttributeModifier.h
        ${SRC_DIR}/entity/ai/attributes/BaseAttributeMap.h
        ${SRC_DIR}/entity/ai/attributes/ModifiableAttributeInstance.h
        ${SRC_DIR}/entity/ai/attributes/ServersideAttributeMap.h
        ${SRC_DIR}/entity/ai/attributes/RangedAttribute.h

        # entity/player
        ${SRC_DIR}/entity/player/PlayerCapabilities.h
        ${SRC_DIR}/entity/player/EnumPlayerModelParts.h

        # item
        ${SRC_DIR}/item/EnumAction.h
        ${SRC_DIR}/item/EnumRarity.h

        # enchantment
        ${SRC_DIR}/enchantment/EnumEnchantmentType.h

        # command
        ${SRC_DIR}/command/ICommandSender.h
        ${SRC_DIR}/command/CommandException.h
        ${SRC_DIR}/command/ICommandManager.h
        ${SRC_DIR}/command/ICommand.h
        ${SRC_DIR}/command/IAdminCommand.h
        ${SRC_DIR}/command/CommandBase.h
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
        ${Stb_INCLUDE_DIRS}
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

