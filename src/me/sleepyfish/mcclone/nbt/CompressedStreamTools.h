//
// Created by SleepyFish on 15.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_COMPRESSEDSTREAMTOOLS_H
#define MCCLONE_COMPRESSEDSTREAMTOOLS_H

#include <filesystem>

class NBTTagCompound;

/**
 * @author SleepyFish
 * @brief Compressed stream tools
 */
class CompressedStreamTools {

public:

    static NBTTagCompound* read(std::filesystem::path path);

    static void safeWrite(NBTTagCompound* nbt, std::filesystem::path path);

};


#endif //MCCLONE_COMPRESSEDSTREAMTOOLS_H
