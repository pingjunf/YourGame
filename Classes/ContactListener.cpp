//
//  ContactListener.cpp
//  IslandLegend
//
//  Created by pingjun fang on 16/3/21.
//
//

#include "ContactListener.h"

void ContactListener::BeginContact(b2Contact* contact){
    b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();
    auto spriteA = (Sprite*)bodyA->GetUserData();
    auto spriteB = (Sprite*)bodyB->GetUserData();
    if (spriteA != nullptr && spriteB != nullptr) {
        spriteB->setColor(Color3B::YELLOW);
        spriteA->setColor(Color3B::YELLOW);
    }
}

void ContactListener::EndContact(b2Contact* contact){
    b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();
    auto spriteA = (Sprite*)bodyA->GetUserData();
    auto spriteB = (Sprite*)bodyB->GetUserData();
    if (spriteA != nullptr && spriteB != nullptr) {
        spriteB->setColor(Color3B::WHITE);
        spriteA->setColor(Color3B::WHITE);
    }
}
void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    log("PreSolve");
}

void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
    log("PostSolve");
}
