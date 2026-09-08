//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_RENDERINFORMATION_H
#define MCCLONE_RENDERINFORMATION_H

#include <string>
#include <glad/glad.h>

/**
 * @author SleepyFish - SleepyAVA
 * @version 1.2
 * @brief This class contains information about the graphics context
 */
class RenderInformation {

private:

    std::string vendor;

    std::string renderer;

    std::string version;



    int majorVersion;

    int minorVersion;

public:

    void init();

    void print() const;



    const std::string &getVendor() const noexcept;

    const std::string &getRenderer() const noexcept;

    const std::string &getVersion() const noexcept;



    int getMajorVersion() const noexcept;

    int getMinorVersion() const noexcept;



    bool isVersionAtLeast(int major, int minor) const noexcept;

};


#endif //MCCLONE_RENDERINFORMATION_H
