//
//  ii.m
//  IslandLegend
//
//  Created by pingjun fang on 16/3/15.
//
//

#import <Foundation/Foundation.h>
#include "SpineNode.h"
using namespace cocos2d;

struct _Entry
{
    int slotIndex;
    const char* name;
    spAttachment* attachment;
    _Entry* next;
};

typedef struct
{
    spSkin super;
    _Entry* entries;
} _spSkin;

SpineNode::SpineNode(spSkeletonData* skeletonData)
: SkeletonAnimation(skeletonData)
{
    
}

SpineNode::SpineNode(const std::string& skeletonDataFile, spAtlas* atlas, float scale)
: SkeletonAnimation(skeletonDataFile, atlas, scale)
{
    
}

SpineNode::SpineNode(const std::string& skeletonDataFile, const std::string& atlasFile, float scale)
: SkeletonAnimation(skeletonDataFile, atlasFile, scale)
{
    
}

SpineNode* SpineNode::createWithData(spSkeletonData* skeletonData)
{
    SpineNode* node = new SpineNode(skeletonData);
    node->autorelease();
    return node;
}

SpineNode* SpineNode::createWithFile(const std::string& skeletonDataFile, spAtlas* atlas, float scale)
{
    SpineNode* node = new SpineNode(skeletonDataFile, atlas, scale);
    node->autorelease();
    return node;
}

SpineNode* SpineNode::createWithFile(const std::string& skeletonDataFile, const std::string& atlasFile, float scale)
{
    SpineNode* node = new SpineNode(skeletonDataFile, atlasFile, scale);
    node->autorelease();
    return node;
}

bool SpineNode::isFlippedX() const
{
    return _skeleton->flipX != 0;
}

void SpineNode::setFlippedX(bool flippedX)
{
    _skeleton->flipX = flippedX ? 1 : 0;
}

bool SpineNode::isFlippedY() const
{
    return _skeleton->flipY != 0;
}

void SpineNode::setFlippedY(bool flippedY)
{
    _skeleton->flipY = flippedY ? 1 : 0;
}

bool SpineNode::replacementParts(const std::string& skinName, const std::string& attachmentName)
{
    if (skinName.empty())
    {
        return false;
    }
    
    spSkin *skin = spSkeletonData_findSkin(_skeleton->data, skinName.c_str());
    if (!skin) return false;
    
    if (_skeleton->skin)
    {
        const _Entry *entry = reinterpret_cast<_spSkin *>(_skeleton->skin)->entries;
        while (entry)
        {
            spSlot *slot = _skeleton->slots[entry->slotIndex];
            if (strcmp(slot->data->name, attachmentName.c_str()) == 0)
            {
                spAttachment *attachment = spSkin_getAttachment(skin, entry->slotIndex, entry->name);
                if (attachment) spSlot_setAttachment(slot, attachment);
                return true;
            }
            entry = entry->next;
        }
    }
    else
    {
        for (int i = 0; i < _skeleton->slotsCount; ++i)
        {
            spSlot* slot = _skeleton->slots[i];
            if (strcmp(slot->data->name, attachmentName.c_str()) == 0)
            {
                spAttachment* attachment = spSkin_getAttachment(skin, i, slot->data->attachmentName);
                if (attachment) spSlot_setAttachment(slot, attachment);
                return true;
            }
        }
    }
    
    return false;
}