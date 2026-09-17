//
// Created by SleepyFish on 17.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_PACKMETADATASECTION_H
#define MCCLONE_PACKMETADATASECTION_H

#include "IMetadataSection.h"

#include <string>

/**
 * @author SleepyFish
 * @brief Metadata section for a resource pack
 */
class PackMetadataSection : public IMetadataSection {

private:

    const std::string packDescription;
    const int packFormat;

public:

    PackMetadataSection(const std::string &packDescription, int packFormat) :
        packDescription(packDescription),
        packFormat(packFormat)
    {}

    ~PackMetadataSection() = default;

    const std::string &getPackDescription() const noexcept {
        return this->packDescription;
    }

    int getPackFormat() const noexcept {
        return this->packFormat;
    }

};


#endif //MCCLONE_PACKMETADATASECTION_H
