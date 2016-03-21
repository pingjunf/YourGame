#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "SpineNode.h"
#include "cocos2d.h"

#include "Box2D/Box2D.h"
#include <spine/spine-cocos2dx.h>
#define RATIO 80.0f  //转换比例
class HelloWorld : public cocos2d::Layer
{
private:
    spine::SkeletonAnimation* skeletonNode;
    SpineNode* spineNode;
    SpineNode* spineWeaponNode;
    SpineNode* spineSheildNode;
    int inum;
    
    //hpysic world
    b2World* phyWorld;
    b2Body* floorBody;
    b2ContactListener *listener;
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuLeftCallback(cocos2d::Ref* pSender);
    void menuRightCallback(cocos2d::Ref* pSender);
    void menuChangeShieldCallback(cocos2d::Ref* pSender);
    void menuChangeWeaponCallback(cocos2d::Ref* pSender);
    void changeAttachment(SpineNode* spineNode,const char *atlasAttachment, const char *replacedAttachment,const char *replaceAttachment);
    
    
    //physic world
    virtual void update(float);
    virtual void BeginContact(b2Contact* contact);
    void addRect(float positionX, float positionY, b2BodyType type);
    void addFloor();
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
