//
// Created by SleepyFish on 15.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_IMETADATASECTIONSERIALIZER_H
#define MCCLONE_IMETADATASECTIONSERIALIZER_H

#include <string>

#include <nlohmann/json.hpp>

/**
 * @author SleepyFish
 * @brief Metadata section serializer interface
 */
template<typename T>
class IMetadataSectionSerializer {

public:

    virtual ~IMetadataSectionSerializer() = default;

    virtual std::string getSectionName() const = 0;

    virtual T deserialize(const ::nlohmann::json& json) const = 0;

};


#endif //MCCLONE_IMETADATASECTIONSERIALIZER_H
