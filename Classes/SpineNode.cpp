//
//  SINENODE.cpp
//  IslandLegend
//
//  Created by pingjun fang on 16/3/15.
//
//

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
}_spSkin;

SpineNode::SpineNode(spSkeletonData* skeletonData)
:spine::SkeletonAnimation(skeletonData)
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

spAttachment* SpineNode::getAttachment(const std::string& attachmentName)
{
    spAttachment* attachment;
    for (int i = 0; i < _skeleton->slotsCount; ++i)
    {
        spSlot* slot = _skeleton->slots[i];
        if (strcmp(slot->data->name, attachmentName.c_str()) == 0)
        {
            attachment = spine::SkeletonAnimation::getAttachment(slot->data->name, attachmentName);
            break;
        }
    }
    return attachment;
}
spSlot* SpineNode::getSlot(const std::string& slotName)
{
    for (int i = 0; i < _skeleton->slotsCount; ++i)
    {
        spSlot* slot = _skeleton->slots[i];
        if (strcmp(slot->data->name, slotName.c_str()) == 0)
        {
            return slot;
        }
    }
    return NULL;
}

bool SpineNode::replacementPart(const std::string& replacedAttachment, spAttachment* attachment)
{
//    if (skinName.empty())
//    {
//        return false;
//    }
    
//    spSkin *skin = spSkeletonData_findSkin(_skeleton->data, skinName.c_str());
//    if (!skin) return false;
//    
//    if (_skeleton->skin)
//    {
//        const _Entry *entry = reinterpret_cast<_spSkin *>(_skeleton->skin)->entries;
//        while (entry)
//        {
//            spSlot *slot = _skeleton->slots[entry->slotIndex];
//            if (strcmp(slot->data->name, attachmentName.c_str()) == 0)
//            {
//                spAttachment *attachment = spSkin_getAttachment(skin, entry->slotIndex, entry->name);
//                if (attachment) spSlot_setAttachment(slot, attachment);
//                return true;
//            }
//            entry = entry->next;
//        }
//    }
//    else
//    {
    
    //spSkin *skin = spSkeletonData_findSkin(spineNode->_skeleton->data, skinName.c_str());

    for (int i = 0; i < _skeleton->slotsCount; ++i)
    {
        spSlot* slot = _skeleton->slots[i];
        if (strcmp(slot->data->name, replacedAttachment.c_str()) == 0)
        {
             if (attachment) spSlot_setAttachment(slot, attachment);
        }
    }

    return false;
}
bool SpineNode::replacementPart(spSlot* slot, spAttachment* attachment)
{
    //    if (skinName.empty())
    //    {
    //        return false;
    //    }
    
    //    spSkin *skin = spSkeletonData_findSkin(_skeleton->data, skinName.c_str());
    //    if (!skin) return false;
    //
    //    if (_skeleton->skin)
    //    {
    //        const _Entry *entry = reinterpret_cast<_spSkin *>(_skeleton->skin)->entries;
    //        while (entry)
    //        {
    //            spSlot *slot = _skeleton->slots[entry->slotIndex];
    //            if (strcmp(slot->data->name, attachmentName.c_str()) == 0)
    //            {
    //                spAttachment *attachment = spSkin_getAttachment(skin, entry->slotIndex, entry->name);
    //                if (attachment) spSlot_setAttachment(slot, attachment);
    //                return true;
    //            }
    //            entry = entry->next;
    //        }
    //    }
    //    else
    //    {
    
    //spSkin *skin = spSkeletonData_findSkin(spineNode->_skeleton->data, skinName.c_str());
    
 
    if (attachment) {
        spSlot_setAttachment(slot, attachment);
        return true;
    }else{
        return false;
    }
}