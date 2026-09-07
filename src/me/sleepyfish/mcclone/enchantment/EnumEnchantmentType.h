//
// Created by SleepyFish on 07.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ENUMENCHANTMENTTYPE_H
#define MCCLONE_ENUMENCHANTMENTTYPE_H

/* not implemented yet.
#include "../item/Item.h"
#include "../item/ItemArmor.h"
#include "../item/ItemSword.h"
#include "../item/ItemTool.h"
#include "../item/ItemBow.h"
#include "../item/ItemFishingRod.h"
*/

/**
 * @author SleepyFish
 */
enum class EnumEnchantmentType {

    ALL = 0,
    ARMOR,
    ARMOR_FEET,
    ARMOR_LEGS,
    ARMOR_TORSO,
    ARMOR_HEAD,
    WEAPON,
    DIGGER,
    FISHING_ROD,
    BREAKABLE,
    BOW

};
/* not implemented yet.
inline bool canEnchantItem(EnumEnchantmentType type, const Item& item) {
    if (type == EnumEnchantmentType::ALL)
        return true;

    if (type == EnumEnchantmentType::BREAKABLE && item.isDamageable())
        return true;

    const ItemArmor* armor = dynamic_cast<const ItemArmor*>(&item);
    if (armor) {
        if (type == EnumEnchantmentType::ARMOR)
            return true;
        switch (armor->armorType) {
            case 0: return type == EnumEnchantmentType::ARMOR_HEAD;
            case 1: return type == EnumEnchantmentType::ARMOR_TORSO;
            case 2: return type == EnumEnchantmentType::ARMOR_LEGS;
            case 3: return type == EnumEnchantmentType::ARMOR_FEET;
            default: return false;
        }
    }

    if (dynamic_cast<const ItemSword*>(&item)) return type == EnumEnchantmentType::WEAPON;
    if (dynamic_cast<const ItemTool*>(&item)) return type == EnumEnchantmentType::DIGGER;
    if (dynamic_cast<const ItemBow*>(&item)) return type == EnumEnchantmentType::BOW;
    if (dynamic_cast<const ItemFishingRod*>(&item))return type == EnumEnchantmentType::FISHING_ROD;

    return false;
}
*/

#endif //MCCLONE_ENUMENCHANTMENTTYPE_H
