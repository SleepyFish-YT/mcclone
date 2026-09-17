//
// Created by SleepyFish on 15.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_IMETADATASERIALIZER_H
#define MCCLONE_IMETADATASERIALIZER_H

#include "IMetadataSectionSerializer.h"

#include <unordered_map>
#include <string>
#include <any>
#include <memory>
#include <functional>
#include <stdexcept>

#include <nlohmann/json.hpp>

/**
 * @author SleepyFish
 * @brief Metadata serializer
 */
class IMetadataSerializer {

private:

    struct IRegistration {

        virtual ~IRegistration() = default;

        virtual std::any parse(const nlohmann::json &json) const = 0;

    };

    template<typename T>
    struct Registration : public IRegistration {

        IMetadataSectionSerializer<T> *serializer;

        explicit Registration(IMetadataSectionSerializer<T> *serializer) :
            serializer(serializer)
        {}

        std::any parse(const nlohmann::json &json) const override {
            return serializer->deserialize(json);
        }

    };

    std::unordered_map<std::string, std::unique_ptr<IRegistration>> registry;

public:

    IMetadataSerializer() = default;

    ~IMetadataSerializer() = default;

    template<typename T>
    void registerMetadataSectionType(IMetadataSectionSerializer<T> *serializer) {
        const std::string &sectionName = serializer->getSectionName();
        this->registry[sectionName] = std::make_unique<Registration<T>>(serializer);
    }

    std::any parseMetadataSection(const std::string &sectionName, const nlohmann::json &json) const {
        if (sectionName.empty()) {
            throw std::invalid_argument("Metadata section name cannot be empty");
        }

        if (!json.contains(sectionName)) {
            return std::any{};
        }

        if (!json.at(sectionName).is_object()) {
            throw std::invalid_argument("Invalid metadata for '" + sectionName + "' - expected object, found " + json.at(sectionName).dump());
        }

        auto it = this->registry.find(sectionName);
        if (it == this->registry.end()) {
            throw std::invalid_argument("Don't know how to handle metadata section '" + sectionName + "'");
        }

        return it->second->parse(json.at(sectionName));
    }

};


#endif //MCCLONE_IMETADATASERIALIZER_H
