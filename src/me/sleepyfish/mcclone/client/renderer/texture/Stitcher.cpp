//
// Created by SleepyFish on 14.09.2026.
// Project: mcclone
//

#include "Stitcher.h"

#include "TextureAtlasSprite.h"
#include "../StitcherException.h"
#include "../../../util/MathHelper.h"

#include <algorithm>
#include <stdexcept>
#include <string>

int Stitcher::getMipmapDimension(int value, int mipmapLevel) {
    int mask = (1 << mipmapLevel) - 1;
    return ((value >> mipmapLevel) + ((value & mask) == 0 ? 0 : 1)) << mipmapLevel;
}

void Stitcher::addSprite(TextureAtlasSprite* sprite) {
    Holder holder(sprite, this->mipmapLevelStitcher);

    if (this->maxTileDimension > 0)
        holder.setNewDimension(this->maxTileDimension);

    this->setStitchHolders.push_back(std::move(holder));
}

void Stitcher::doStitch() {
    std::vector<Holder*> sorted;
    sorted.reserve(this->setStitchHolders.size());
    for (Holder& h : this->setStitchHolders)
        sorted.push_back(&h);

    std::sort(sorted.begin(), sorted.end(), [](const Holder* a, const Holder* b) {
        return *b < *a; // descending - largest first
    });

    for (Holder* holder : sorted) {
        if (!this->allocateSlot(*holder)) {
            throw StitcherException(
                    "Unable to fit: " + holder->getAtlasSprite()->getIconName() +
                    " - size: " + std::to_string(holder->getAtlasSprite()->getIconWidth()) +
                    "x"         + std::to_string(holder->getAtlasSprite()->getIconHeight()) +
                    " - Maybe try a lower resolution resourcepack?"
            );
        }
    }

    if (this->forcePowerOf2) {
        this->currentWidth  = MathHelper::roundUpToPowerOfTwo(this->currentWidth);
        this->currentHeight = MathHelper::roundUpToPowerOfTwo(this->currentHeight);
    }
}

std::vector<TextureAtlasSprite*> Stitcher::getStitchSlots() {
    std::vector<Slot*> slots;
    for (Slot& slot : this->stitchSlots)
        slot.getAllStitchSlots(slots);

    std::vector<TextureAtlasSprite*> result;
    for (Slot* slot : slots) {
        Holder* holder = slot->getStitchHolder();
        TextureAtlasSprite* sprite = holder->getAtlasSprite();
        sprite->initSprite(
                this->currentWidth,
                this->currentHeight,
                slot->getOriginX(),
                slot->getOriginY(),
                holder->isRotated()
        );
        result.push_back(sprite);
    }

    return result;
}

bool Stitcher::allocateSlot(Holder& holder) {
    for (Slot& slot : this->stitchSlots) {
        if (slot.addSlot(&holder))
            return true;
        holder.rotate();
        if (slot.addSlot(&holder))
            return true;
        holder.rotate();
    }
    return this->expandAndAllocateSlot(holder);
}

bool Stitcher::expandAndAllocateSlot(Holder& holder) {
    int minDim = std::min(holder.getWidth(), holder.getHeight());
    bool empty = this->currentWidth == 0 && this->currentHeight == 0;
    bool expandWidth;

    if (this->forcePowerOf2) {
        int pw = MathHelper::roundUpToPowerOfTwo(this->currentWidth);
        int ph = MathHelper::roundUpToPowerOfTwo(this->currentHeight);
        int nw = MathHelper::roundUpToPowerOfTwo(this->currentWidth  + minDim);
        int nh = MathHelper::roundUpToPowerOfTwo(this->currentHeight + minDim);
        bool canW = nw <= this->maxWidth;
        bool canH = nh <= this->maxHeight;

        if (!canW && !canH)
            return false;

        bool growW = pw != nw;
        bool growH = ph != nh;

        if (growW ^ growH)
            expandWidth = !growW;
        else
            expandWidth = canW && pw <= ph;
    } else {
        bool canW = this->currentWidth  + minDim <= this->maxWidth;
        bool canH = this->currentHeight + minDim <= this->maxHeight;

        if (!canW && !canH) return false;

        expandWidth = canW && (empty || this->currentWidth <= this->currentHeight);
    }

    int maxDim = std::max(holder.getWidth(), holder.getHeight());
    int limit  = expandWidth ? this->maxHeight : this->maxWidth;
    int base   = expandWidth ? this->currentHeight : this->currentWidth;

    if (MathHelper::roundUpToPowerOfTwo(base + maxDim) > limit)
        return false;

    Slot newSlot(0, 0, 0, 0); // placeholder, set below

    if (expandWidth) {
        if (holder.getWidth() > holder.getHeight())
            holder.rotate();

        if (this->currentHeight == 0)
            this->currentHeight = holder.getHeight();

        newSlot = Slot(this->currentWidth, 0, holder.getWidth(), this->currentHeight);
        this->currentWidth += holder.getWidth();
    } else {
        newSlot = Slot(0, this->currentHeight, this->currentWidth, holder.getHeight());
        this->currentHeight += holder.getHeight();
    }

    newSlot.addSlot(&holder);
    this->stitchSlots.push_back(std::move(newSlot));
    return true;
}

Stitcher::Holder::Holder(TextureAtlasSprite* sprite, int mipmapLevel) :
        theTexture(sprite),
        width(sprite->getIconWidth()),
        height(sprite->getIconHeight()),
        mipmapLevelHolder(mipmapLevel),
        rotated(Stitcher::getMipmapDimension(sprite->getIconHeight(), mipmapLevel) >
                Stitcher::getMipmapDimension(sprite->getIconWidth(),  mipmapLevel))
{}

int Stitcher::Holder::getWidth() const {
    return Stitcher::getMipmapDimension(
            static_cast<int>((this->rotated ? this->height : this->width) * this->scaleFactor),
            this->mipmapLevelHolder
    );
}

int Stitcher::Holder::getHeight() const {
    return Stitcher::getMipmapDimension(
            static_cast<int>((this->rotated ? this->width : this->height) * this->scaleFactor),
            this->mipmapLevelHolder
    );
}

void Stitcher::Holder::setNewDimension(int maxDim) {
    if (this->width > maxDim && this->height > maxDim)
        this->scaleFactor = static_cast<float>(maxDim) / static_cast<float>(std::min(this->width, this->height));
}

bool Stitcher::Holder::operator<(const Holder& other) const {
    if (this->getHeight() != other.getHeight())
        return this->getHeight() < other.getHeight();
    if (this->getWidth() != other.getWidth())
        return this->getWidth() < other.getWidth();
    const std::string& a = this->theTexture->getIconName();
    const std::string& b = other.theTexture->getIconName();
    return a < b;
}

bool Stitcher::Slot::addSlot(Holder* holderIn) {
    if (this->holder != nullptr)
        return false;

    int w = holderIn->getWidth();
    int h = holderIn->getHeight();

    if (w > this->width || h > this->height)
        return false;

    if (w == this->width && h == this->height) {
        this->holder = holderIn;
        return true;
    }

    if (this->subSlots.empty()) {
        this->subSlots.reserve(3);
        this->subSlots.emplace_back(this->originX, this->originY, w, h);

        int remainW = this->width  - w;
        int remainH = this->height - h;

        if (remainH > 0 && remainW > 0) {
            if (std::max(this->height, remainW) >= std::max(this->width, remainH)) {
                this->subSlots.emplace_back(this->originX,     this->originY + h, w,            remainH);
                this->subSlots.emplace_back(this->originX + w, this->originY,     remainW,      this->height);
            } else {
                this->subSlots.emplace_back(this->originX + w, this->originY,     remainW,      h);
                this->subSlots.emplace_back(this->originX,     this->originY + h, this->width,  remainH);
            }
        } else if (remainW == 0) {
            this->subSlots.emplace_back(this->originX, this->originY + h, w, remainH);
        } else if (remainH == 0) {
            this->subSlots.emplace_back(this->originX + w, this->originY, remainW, h);
        }
    }

    for (Slot& sub : this->subSlots) {
        if (sub.addSlot(holderIn))
            return true;
    }

    return false;
}

void Stitcher::Slot::getAllStitchSlots(std::vector<Slot*>& result) {
    if (this->holder != nullptr) {
        result.push_back(this);
    } else {
        for (Slot& sub : this->subSlots) {
            sub.getAllStitchSlots(result);
        }
    }
}
