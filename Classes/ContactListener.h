

#ifndef ContactListener_h
#define ContactListener_h

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include <stdio.h>

USING_NS_CC;


class ContactListener:public b2ContactListener
{
private:
    virtual void BeginContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impluse);
    virtual void EndContact(b2Contact* contact);

};

#endif
