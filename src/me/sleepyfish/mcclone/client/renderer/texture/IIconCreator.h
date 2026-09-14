//
// Created by SleepyFish on 14.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_IICONCREATOR_H
#define MCCLONE_IICONCREATOR_H

class TextureMap;

/**
 * @author SleepyFish
 * @brief Interface for creating icons
 */
class IIconCreator {

public:

    virtual ~IIconCreator() {}

    virtual void registerSprites(TextureMap& iconRegistry) = 0;

};


#endif //MCCLONE_IICONCREATOR_H
