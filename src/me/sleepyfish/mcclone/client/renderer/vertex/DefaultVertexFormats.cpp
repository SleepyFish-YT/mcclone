//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

#include "DefaultVertexFormats.h"

VertexFormat DefaultVertexFormats::BLOCK;
VertexFormat DefaultVertexFormats::ITEM;
VertexFormat DefaultVertexFormats::OLDMODEL_POSITION_TEX_NORMAL;
VertexFormat DefaultVertexFormats::PARTICLE_POSITION_TEX_COLOR_LMAP;
VertexFormat DefaultVertexFormats::POSITION;
VertexFormat DefaultVertexFormats::POSITION_COLOR;
VertexFormat DefaultVertexFormats::POSITION_TEX;
VertexFormat DefaultVertexFormats::POSITION_NORMAL;
VertexFormat DefaultVertexFormats::POSITION_TEX_COLOR;
VertexFormat DefaultVertexFormats::POSITION_TEX_NORMAL;
VertexFormat DefaultVertexFormats::POSITION_TEX_LMAP_COLOR;
VertexFormat DefaultVertexFormats::POSITION_TEX_COLOR_NORMAL;

const VertexFormatElement DefaultVertexFormats::POSITION_3F{
        0, VertexFormatElement::EnumType::FLOAT, VertexFormatElement::EnumUsage::POSITION, 3};
const VertexFormatElement DefaultVertexFormats::COLOR_4UB{
        0, VertexFormatElement::EnumType::UBYTE, VertexFormatElement::EnumUsage::COLOR, 4};
const VertexFormatElement DefaultVertexFormats::TEX_2F{
        0, VertexFormatElement::EnumType::FLOAT, VertexFormatElement::EnumUsage::UV, 2};
const VertexFormatElement DefaultVertexFormats::TEX_2S{
        1, VertexFormatElement::EnumType::SHORT, VertexFormatElement::EnumUsage::UV, 2};
const VertexFormatElement DefaultVertexFormats::NORMAL_3B{
        0, VertexFormatElement::EnumType::BYTE, VertexFormatElement::EnumUsage::NORMAL, 3};
const VertexFormatElement DefaultVertexFormats::PADDING_1B{
        0, VertexFormatElement::EnumType::BYTE, VertexFormatElement::EnumUsage::PADDING, 1};

void DefaultVertexFormats::staticInit() {
    DefaultVertexFormats::BLOCK.addElement(DefaultVertexFormats::POSITION_3F);
    DefaultVertexFormats::BLOCK.addElement(DefaultVertexFormats::COLOR_4UB);
    DefaultVertexFormats::BLOCK.addElement(DefaultVertexFormats::TEX_2F);
    DefaultVertexFormats::BLOCK.addElement(DefaultVertexFormats::TEX_2S);
    DefaultVertexFormats::ITEM.addElement(DefaultVertexFormats::POSITION_3F);
    DefaultVertexFormats::ITEM.addElement(DefaultVertexFormats::COLOR_4UB);
    DefaultVertexFormats::ITEM.addElement(DefaultVertexFormats::TEX_2F);
    DefaultVertexFormats::ITEM.addElement(DefaultVertexFormats::NORMAL_3B);
    DefaultVertexFormats::ITEM.addElement(DefaultVertexFormats::PADDING_1B);
    DefaultVertexFormats::OLDMODEL_POSITION_TEX_NORMAL.addElement(DefaultVertexFormats::POSITION_3F);
    DefaultVertexFormats::OLDMODEL_POSITION_TEX_NORMAL.addElement(DefaultVertexFormats::TEX_2F);
    DefaultVertexFormats::OLDMODEL_POSITION_TEX_NORMAL.addElement(DefaultVertexFormats::NORMAL_3B);
    DefaultVertexFormats::OLDMODEL_POSITION_TEX_NORMAL.addElement(DefaultVertexFormats::PADDING_1B);
    DefaultVertexFormats::PARTICLE_POSITION_TEX_COLOR_LMAP.addElement(DefaultVertexFormats::POSITION_3F);
    DefaultVertexFormats::PARTICLE_POSITION_TEX_COLOR_LMAP.addElement(DefaultVertexFormats::TEX_2F);
    DefaultVertexFormats::PARTICLE_POSITION_TEX_COLOR_LMAP.addElement(DefaultVertexFormats::COLOR_4UB);
    DefaultVertexFormats::PARTICLE_POSITION_TEX_COLOR_LMAP.addElement(DefaultVertexFormats::TEX_2S);
    DefaultVertexFormats::POSITION.addElement(DefaultVertexFormats::POSITION_3F);
    DefaultVertexFormats::POSITION_COLOR.addElement(DefaultVertexFormats::POSITION_3F);
    DefaultVertexFormats::POSITION_COLOR.addElement(DefaultVertexFormats::COLOR_4UB);
    DefaultVertexFormats::POSITION_TEX.addElement(DefaultVertexFormats::POSITION_3F);
    DefaultVertexFormats::POSITION_TEX.addElement(DefaultVertexFormats::TEX_2F);
    DefaultVertexFormats::POSITION_NORMAL.addElement(DefaultVertexFormats::POSITION_3F);
    DefaultVertexFormats::POSITION_NORMAL.addElement(DefaultVertexFormats::NORMAL_3B);
    DefaultVertexFormats::POSITION_NORMAL.addElement(DefaultVertexFormats::PADDING_1B);
    DefaultVertexFormats::POSITION_TEX_COLOR.addElement(DefaultVertexFormats::POSITION_3F);
    DefaultVertexFormats::POSITION_TEX_COLOR.addElement(DefaultVertexFormats::TEX_2F);
    DefaultVertexFormats::POSITION_TEX_COLOR.addElement(DefaultVertexFormats::COLOR_4UB);
    DefaultVertexFormats::POSITION_TEX_NORMAL.addElement(DefaultVertexFormats::POSITION_3F);
    DefaultVertexFormats::POSITION_TEX_NORMAL.addElement(DefaultVertexFormats::TEX_2F);
    DefaultVertexFormats::POSITION_TEX_NORMAL.addElement(DefaultVertexFormats::NORMAL_3B);
    DefaultVertexFormats::POSITION_TEX_NORMAL.addElement(DefaultVertexFormats::PADDING_1B);
    DefaultVertexFormats::POSITION_TEX_LMAP_COLOR.addElement(DefaultVertexFormats::POSITION_3F);
    DefaultVertexFormats::POSITION_TEX_LMAP_COLOR.addElement(DefaultVertexFormats::TEX_2F);
    DefaultVertexFormats::POSITION_TEX_LMAP_COLOR.addElement(DefaultVertexFormats::TEX_2S);
    DefaultVertexFormats::POSITION_TEX_LMAP_COLOR.addElement(DefaultVertexFormats::COLOR_4UB);
    DefaultVertexFormats::POSITION_TEX_COLOR_NORMAL.addElement(DefaultVertexFormats::POSITION_3F);
    DefaultVertexFormats::POSITION_TEX_COLOR_NORMAL.addElement(DefaultVertexFormats::TEX_2F);
    DefaultVertexFormats::POSITION_TEX_COLOR_NORMAL.addElement(DefaultVertexFormats::COLOR_4UB);
    DefaultVertexFormats::POSITION_TEX_COLOR_NORMAL.addElement(DefaultVertexFormats::NORMAL_3B);
    DefaultVertexFormats::POSITION_TEX_COLOR_NORMAL.addElement(DefaultVertexFormats::PADDING_1B);
}
