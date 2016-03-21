//
//  SINENODE.hpp
//  IslandLegend
//
//  Created by pingjun fang on 16/3/15.
//
//

#ifndef SINENODE_H
#define SINENODE_H
#include <spine/spine-cocos2dx.h>
//#include <spine/SkeletonRenderer.h>
//#include "spine/SkeletonAnimation.h"

#include <stdio.h>

class SpineNode final:public spine::SkeletonAnimation
{

public:
    static SpineNode* createWithData(spSkeletonData* skeletonData);
    static SpineNode* createWithFile(const std::string& skeletonDataFile,spAtlas* atlas,float scale = 1);
    static SpineNode* createWithFile(const std::string& skeletonDataFile,const std::string&atlasFile,float scale = 1);
public:
    bool isFlippedX() const;
    void setFlippedX(bool flippedX);
    bool isFlippedY() const;
    void setFlippedY(bool flippedY);
    //bool replacementParts(SpineNode* sourceSpineNode);
    bool replacementPart(const std::string& replacedAttachment,spAttachment* attachment);
    bool replacementPart(spSlot* slot,spAttachment* attachment);
    spAttachment* getAttachment(const std::string& attachmentName);
    spSlot* getSlot(const std::string& slotName);
//    bool changeAttachment(const std::string& slotName, const std::string& attachmentName, const std::string& atlasFile);
private:
    SpineNode(spSkeletonData* skeletonData);
    SpineNode(const std::string& skeletonDataFile,spAtlas* atlas,float scale = 1);
    SpineNode(const std::string& skeletonDataFile,const std::string& atlasFile,float scale = 1);
private:
    cocos2d::CustomCommand _customCommand;
};

#endif
