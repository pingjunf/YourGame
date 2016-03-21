#include "HelloWorldScene.h"
#include "spine/spine.h"
#include "ContactListener.h"
USING_NS_CC;

int ball1Mask = 1 << 0;
int ball2Mask = 1 << 1;
int wallMask = 1 << 2;




Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
//    cur_global_init()
    // 定义重力加速
    b2Vec2 gravity;
    
    // 设定垂直方向的加速度，这里采用真实的物理数据
    gravity.Set(0.0f, -9.8f);
    
    // 使用刚刚定义好的加速度生成物理世界对象，这样世界中的所有对象都会受到重力加速度的影响
    phyWorld = new b2World(gravity);
    
    // 物理世界的对象都参与碰撞检测，无休眠对象
    //phyWorld->SetAllowSleeping(false);
    
    //连续碰撞检测，避免发生物体穿过另一个物体的事件
    //phyWorld->SetContinuousPhysics(true);
    
    // 设置碰撞监听器
    listener = new ContactListener();
    phyWorld->SetContactListener(listener);
    
    addFloor();
    addRect(5, 3, b2_dynamicBody);
    scheduleUpdate();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width*2 ,
                                60));
    
    closeItem->setScale(2);
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    
    this->addChild(menu, 1);
    
    auto left = MenuItemImage::create(
                                           "b1.png",
                                           "b1.png",
                                           CC_CALLBACK_1(HelloWorld::menuLeftCallback, this));
    left->setPosition(Vec2(50,60));
    left->setScale(2);
    auto leftMenu = Menu::create(left, NULL);
    leftMenu->setPosition(Vec2::ZERO);
    
    this->addChild(leftMenu, 1);
    
    auto right = MenuItemImage::create(
                                      "f1.png",
                                      "f1.png",
                                      CC_CALLBACK_1(HelloWorld::menuRightCallback, this));
    
    right->setPosition(Vec2(100,60));
    
    auto rightMenu = Menu::create(right, NULL);
    rightMenu->setPosition(Vec2::ZERO);
    right->setScale(2);
    
    this->addChild(rightMenu, 1);
    inum = 1;
    auto changeShield = MenuItemImage::create(
                                       "changeShield.png",
                                       "changeShield.png",
                                       CC_CALLBACK_1(HelloWorld::menuChangeShieldCallback, this));
    
    changeShield->setPosition(Vec2(100,200));
    
    auto changeShieldMenu = Menu::create(changeShield, NULL);
    changeShieldMenu->setPosition(Vec2::ZERO);
    //changeShield->setScale(2);
    
    this->addChild(changeShieldMenu, 1);
    
    auto changeWeapon = MenuItemImage::create(
                                       "changeWeapon.png",
                                       "changeWeapon.png",
                                       CC_CALLBACK_1(HelloWorld::menuChangeWeaponCallback, this));
    
    changeWeapon->setPosition(Vec2(200,200));
    
    auto changeWeaponMenu = Menu::create(changeWeapon, NULL);
    changeWeaponMenu->setPosition(Vec2::ZERO);
    //changeWeapon->setScale(2);
    
    this->addChild(changeWeaponMenu, 1);
    
    spineNode = SpineNode::createWithFile("res/spineboy.json", "res/spineboy.atlas");
    
    spineNode->setScale(0.2);
    spineNode->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/4 + origin.y));
    this->addChild(spineNode);
    

    spineSheildNode = SpineNode::createWithFile("res/shield.json", "res/shield.atlas");
    spineSheildNode->setScale(0.2);
    spineSheildNode->setPosition(Vec2(visibleSize.width/2 + origin.x, 0));
    this->addChild(spineSheildNode);
    
    return true;
}


void HelloWorld::BeginContact(b2Contact *contact){
    if (contact->GetFixtureA()->GetBody()==floorBody||contact->GetFixtureB()->GetBody()==floorBody) {
        printf("%s","Something on the floor");
    }
}

void HelloWorld::update(float dt){
    phyWorld->Step(dt, 8, 3);
    Sprite * s;
    for (b2Body * b = phyWorld->GetBodyList(); b; b=b->GetNext()) {
        if (b->GetUserData()) {
            s = (Sprite*)b->GetUserData();
            s->setPosition(b->GetPosition().x*RATIO, b->GetPosition().y*RATIO);
        }
    }
}

void HelloWorld::addRect(float positionX, float positionY, b2BodyType type){

    b2BodyDef def;
    def.position = b2Vec2(positionX,positionY);
    def.linearVelocity = b2Vec2(0,10);
    def.type = type;
    b2Body* body = phyWorld->CreateBody(&def);
    b2PolygonShape shape;
    shape.SetAsBox(0.5, 0.5);
    b2FixtureDef fixDef;
    fixDef.density = 1;
    fixDef.friction = 0.3;
    fixDef.shape = &shape;
    body->CreateFixture(&fixDef);
    auto s = Sprite::create();
    s->setTextureRect(Rect(0,0,0.5*2*RATIO,0.5*2*RATIO));
    addChild(s);
    body->SetUserData(s);

}

void HelloWorld::addFloor(){
    b2BodyDef def;
    def.position = b2Vec2(0,0);
    def.type = b2_staticBody;
    floorBody = phyWorld->CreateBody(&def);
    
    b2PolygonShape floorShape;
    floorShape.SetAsBox(400/RATIO, 0.5);
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1;
    fixtureDef.friction = 0.3;
    fixtureDef.shape = &floorShape;
    floorBody->CreateFixture(&fixtureDef);

}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    spineNode->setMix("jump", "walk", 0.2f);
    spineNode->setMix("walk", "jump", 0.4f);
    spineNode->setAnimation(0, "jump", false);
    
    //spineNode->runAction(action);
}

void HelloWorld::menuChangeShieldCallback(Ref* pSender)
{
//    inum = ++inum;
//    //int a = 2;
//    char *prefix_shield = "shi_01_01_01_00";
//    char *s;
//    char * ss;
//    printf(s, "%s", inum);
//    sprintf(ss, "%.7s%.7s", prefix_shield,s);
//    printf("%s",ss);
//    const char *n = inum;
//    std::stringstream ss;
//    ss << inum;
//    printf("%s",ss.str().c_str());
   

    changeAttachment(spineNode, "res/shield.atlas", "shi_01_01_01_002", "shi_01_01_01_007");
}

void HelloWorld::menuChangeWeaponCallback(Ref* pSender)
{
    changeAttachment(spineNode, "res/weapon.atlas", "ico_wea_01_18_01_045", "ico_wea_01_18_01_007");
}

void HelloWorld::menuRightCallback(Ref* pSender)
{
    Action * move = MoveBy::create(1, Point(100,0));
    spineNode->setFlippedX(false);
    spineNode->setAnimation(0, "walk", false);
    spineNode->runAction(move);
}

void HelloWorld::menuLeftCallback(Ref* pSender)
{
    Action * move = MoveBy::create(1, Point(-100,0));
    spineNode->setFlippedX(true);
    spineNode->setAnimation(0, "walk", false);
    spineNode->runAction(move);
}

void HelloWorld::changeAttachment(SpineNode* spineNode,const char *atlasAttachment, const char *replacedAttachment,const char *replaceAttachment){
    spSlot* slot = spineNode->findSlot(replacedAttachment);
    if (NULL != slot)
    {
        int nType = slot->attachment->type;
        spAtlas* atlas = spAtlas_createFromFile(atlasAttachment, nullptr);
        CCASSERT(atlas, "SkeletonRenderer loading atlas file error");
        spAtlasAttachmentLoader* atlasAttachmentLoader = spAtlasAttachmentLoader_create(atlas);
        spAttachmentLoader* attachmentLoader = &(atlasAttachmentLoader->super);
        spSkin *skin = spSkin_create("default");
        spRegionAttachment* regionAttachmentSrc = (spRegionAttachment*)(slot->attachment);
        spAttachment* attachment = spAttachmentLoader_newAttachment(
                                                                    attachmentLoader, skin, SP_ATTACHMENT_REGION, replacedAttachment, replaceAttachment);
        
        spRegionAttachment* regionAttachment = (spRegionAttachment*)attachment;
        regionAttachment->width = regionAttachmentSrc->width;
        regionAttachment->height = regionAttachmentSrc->height;
        regionAttachment->rotation = regionAttachmentSrc->rotation;
        regionAttachment->x = regionAttachmentSrc->x;
        regionAttachment->y = regionAttachmentSrc->y;
        regionAttachment->scaleX = regionAttachmentSrc->scaleX;
        regionAttachment->scaleY = regionAttachmentSrc->scaleY;
        regionAttachment->a = regionAttachmentSrc->a;
        regionAttachment->b = regionAttachmentSrc->b;
        regionAttachment->r = regionAttachmentSrc->r;
        regionAttachment->g = regionAttachmentSrc->g;
        spRegionAttachment_updateOffset(regionAttachment);
        
        spineNode->replacementPart(slot,attachment);
        
        //spineNode->replacementPart("shi_01_01_01_002",attachment);
    }
}


