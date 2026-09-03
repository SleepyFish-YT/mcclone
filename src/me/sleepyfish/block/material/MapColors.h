//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_MAPCOLORS_H
#define MCCLONE_MAPCOLORS_H

#include "MapColor.h"

#include <array>

/**
 * @author SleepyFish
 * @brief Map color list
 */
class MapColors {

public:

    static inline const MapColor airColor = MapColor(0, 0);
    static inline const MapColor grassColor = MapColor(1, 8368696);
    static inline const MapColor sandColor = MapColor(2, 16247203);
    static inline const MapColor clothColor = MapColor(3, 10987431);
    static inline const MapColor tntColor = MapColor(4, 16711680);
    static inline const MapColor iceColor = MapColor(5, 10526975);
    static inline const MapColor ironColor = MapColor(6, 10987431);
    static inline const MapColor foliageColor = MapColor(7, 31744);
    static inline const MapColor snowColor = MapColor(8, 16777215);
    static inline const MapColor clayColor = MapColor(9, 10791096);
    static inline const MapColor dirtColor = MapColor(10, 12020271);
    static inline const MapColor stoneColor = MapColor(11, 7368816);
    static inline const MapColor waterColor = MapColor(12, 4210943);
    static inline const MapColor woodColor = MapColor(13, 6837042);
    static inline const MapColor quartzColor = MapColor(14, 16776437);
    static inline const MapColor adobeColor = MapColor(15, 14188339);
    static inline const MapColor magentaColor = MapColor(16, 11685080);
    static inline const MapColor lightBlueColor = MapColor(17, 6724056);
    static inline const MapColor yellowColor = MapColor(18, 15066419);
    static inline const MapColor limeColor = MapColor(19, 8375321);
    static inline const MapColor pinkColor = MapColor(20, 15892389);
    static inline const MapColor grayColor = MapColor(21, 5000268);
    static inline const MapColor silverColor = MapColor(22, 10066329);
    static inline const MapColor cyanColor = MapColor(23, 5013401);
    static inline const MapColor purpleColor = MapColor(24, 8339378);
    static inline const MapColor blueColor = MapColor(25, 3361970);
    static inline const MapColor brownColor = MapColor(26, 6704179);
    static inline const MapColor greenColor = MapColor(27, 6717235);
    static inline const MapColor redColor = MapColor(28, 10040115);
    static inline const MapColor blackColor = MapColor(29, 1644825);
    static inline const MapColor goldColor = MapColor(30, 16445005);
    static inline const MapColor diamondColor = MapColor(31, 6085589);
    static inline const MapColor lapisColor = MapColor(32, 4882687);
    static inline const MapColor emeraldColor = MapColor(33, 55610);
    static inline const MapColor obsidianColor = MapColor(34, 1381407);
    static inline const MapColor netherrackColor = MapColor(35, 7340544);

    static inline const std::array<MapColor, 36> mapColorArray = {
            airColor,
            grassColor,
            sandColor,
            clothColor,
            tntColor,
            iceColor,
            ironColor,
            foliageColor,
            snowColor,
            clayColor,
            dirtColor,
            stoneColor,
            waterColor,
            woodColor,
            quartzColor,
            adobeColor,
            magentaColor,
            lightBlueColor,
            yellowColor,
            limeColor,
            pinkColor,
            grayColor,
            silverColor,
            cyanColor,
            purpleColor,
            blueColor,
            brownColor,
            greenColor,
            redColor,
            blackColor,
            goldColor,
            diamondColor,
            lapisColor,
            emeraldColor,
            obsidianColor,
            netherrackColor
    };

};


#endif //MCCLONE_MAPCOLORS_H
