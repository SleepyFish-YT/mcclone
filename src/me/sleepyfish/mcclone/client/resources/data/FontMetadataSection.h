//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_FONTMETADATASECTION_H
#define MCCLONE_FONTMETADATASECTION_H

#include <vector>

/**
 * @author SleepyFish
 * @brief Font metadata section
 */
class FontMetadataSection {

private:

    const std::vector<float> charWidths;
    const std::vector<float> charLefts;
    const std::vector<float> charSpacings;

public:

    FontMetadataSection(const std::vector<float>& charWidths, const std::vector<float>& charLefts, const std::vector<float>& charSpacings) :
        charWidths(charWidths),
        charLefts(charLefts),
        charSpacings(charSpacings)
    {}

    const std::vector<float>& getCharWidths() const noexcept {
        return this->charWidths;
    }

    const std::vector<float>& getCharLefts() const noexcept {
        return this->charLefts;
    }

    const std::vector<float>& getCharSpacings() const noexcept {
        return this->charSpacings;
    }

};


#endif //MCCLONE_FONTMETADATASECTION_H
