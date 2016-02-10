#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"


class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
	 cocos2d::Sprite *newSprite;

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	cocos2d::Sprite *snakeHead;
	cocos2d::Sprite *apfel;

	void HelloWorld::update(float dt);

	std::vector<cocos2d::Sprite *> snakeVector;

	bool add;


private:
	cocos2d::PhysicsWorld *sceneWorld;
    
    void SetPhysicsWorld( cocos2d::PhysicsWorld *world ) { sceneWorld = world; };
    
    bool onContactBegin( cocos2d::PhysicsContact &contact );


};

#endif // __HELLOWORLD_SCENE_H__
