//
// Created by SleepyFish on 06.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ATTRIBUTEMODIFIER_H
#define MCCLONE_ATTRIBUTEMODIFIER_H

#include "../../../../sava/UUID.h"

#include <string>
#include <stdexcept>
#include <sstream>

/**
 * @author SleepyFish
 */
class AttributeModifier {

private:

    const UUID id;
    const std::string name;
    const double amount;
    const int operation;
    bool isSaved_;

public:

    AttributeModifier(const std::string& nameIn, double amountIn, int operationIn) :
        AttributeModifier(UUID::random(), nameIn, amountIn, operationIn)
    {}

    AttributeModifier(const UUID& idIn, const std::string& nameIn, double amountIn, int operationIn) :
        id(idIn),
        name(nameIn),
        amount(amountIn),
        operation(operationIn),
        isSaved_(true)
    {
        if (nameIn.empty())
            throw std::invalid_argument("Modifier name cannot be empty");

        if (operationIn < 0 || operationIn > 2)
            throw std::invalid_argument("Invalid operation");
    }

    UUID getID() const {
        return this->id;
    }

    std::string getName() const {
        return this->name;
    }

    int getOperation() const {
        return this->operation;
    }

    double getAmount() const {
        return this->amount;
    }

    bool isSaved() const {
        return this->isSaved_;
    }

    AttributeModifier& setSaved(bool saved) {
        this->isSaved_ = saved;
        return *this;
    }

    bool operator==(const AttributeModifier& other) const {
        return this->id == other.id;
    }

    bool operator!=(const AttributeModifier& other) const {
        return !(*this == other);
    }

    std::size_t hashCode() const {
        return std::hash<UUID>{}(this->id);
    }

    std::string toString() const {
        std::ostringstream oss;
        oss << "AttributeModifier{"
            << "amount="    << this->amount   << ", "
            << "operation=" << this->operation<< ", "
            << "name='"     << this->name     << "', "
            << "id="        << this->id       << ", "
            << "serialize=" << std::boolalpha << this->isSaved_
            << "}";
        return oss.str();
    }

};


template<>
struct std::hash<AttributeModifier> {
    std::size_t operator()(const AttributeModifier& mod) const noexcept {
        return mod.hashCode();
    }
};


#endif //MCCLONE_ATTRIBUTEMODIFIER_H
