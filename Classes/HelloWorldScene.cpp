#include "HelloWorldScene.h"

USING_NS_CC;

extern int direction = -1;

Scene* HelloWorld::createScene()
{	
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	
	auto layer = HelloWorld::create();
	layer->SetPhysicsWorld(scene->getPhysicsWorld());

    // add layer as a child to scene
    scene->addChild(layer);

    return scene;
}


// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	add = false;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
	auto background = Sprite::create("greenerGrass.jpg");
	background->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	background->setScale(1.25);
	
	this->addChild(background);

	auto edgeNode = Node::create();
    edgeNode ->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ) );

	this->addChild(edgeNode);
	
	snakeHead = Sprite::create("flirts.png", Rect(256, 1, 62, 62));
	snakeHead->setPosition(310,310);
	snakeHead->setAnchorPoint(Vec2(0.0, 0.0));
	snakeHead->setTag(1);

	auto snakeHeadPBody = PhysicsBody::createBox( snakeHead->getContentSize( ), PhysicsMaterial( 0, 0, 0 ) );
   snakeHeadPBody->setCollisionBitmask(1);
   snakeHeadPBody->setContactTestBitmask(true);
	snakeHeadPBody->setGravityEnable(false);
   snakeHead->setPhysicsBody(snakeHeadPBody);

	this->addChild(snakeHead);


	auto snakeSeg = Sprite::create("flirts.png", Rect(65, 0, 62, 62));
	snakeSeg->setPosition(248, 310);
	snakeSeg->setAnchorPoint(Vec2(0.0, 0.0));
	snakeSeg->setTag(2);

	auto snakeSegPBody = PhysicsBody::createBox(Size(snakeSeg->getContentSize().width / 2, snakeSeg->getContentSize().height / 2), PhysicsMaterial( 0, 0, 0 ) );
    snakeSegPBody->setCollisionBitmask(1);
    snakeSegPBody->setContactTestBitmask(true);
	snakeSegPBody->setGravityEnable(false);
    snakeSeg->setPhysicsBody(snakeSegPBody);

	snakeVector.push_back(snakeSeg);
	
	this->addChild(snakeSeg);


	auto snakeTail = Sprite::create("flirts.png", Rect(258, 128, 62, 62));
	snakeTail->setPosition(186, 310);
	snakeTail->setAnchorPoint(Vec2(0.0, 0.0));
	snakeTail->setTag(2);

	auto spriteTailPBody = PhysicsBody::createBox(Size(snakeTail->getContentSize().width / 2, snakeTail->getContentSize().height / 2), PhysicsMaterial( 0, 0, 0 ) );
    spriteTailPBody->setCollisionBitmask(1);
    spriteTailPBody->setContactTestBitmask(true);
	spriteTailPBody->setGravityEnable(false);
    snakeTail->setPhysicsBody(spriteTailPBody);
	
	snakeVector.push_back(snakeTail);
	
	this->addChild(snakeTail);


	apfel = Sprite::create("flirts.png", Rect(1, 193, 62, 62));
	apfel->setPosition(62,310);
	apfel->setAnchorPoint(Vec2(0.0,0.0));
	apfel->setTag(3);

	auto apfelPBody = PhysicsBody::createBox(Size(apfel->getContentSize().width / 2, apfel->getContentSize().height / 2), PhysicsMaterial( 0, 0, 0 ) );
    apfelPBody->setCollisionBitmask(1);
    apfelPBody->setContactTestBitmask(true);
	apfelPBody->setGravityEnable(false);
    apfel->setPhysicsBody(apfelPBody);

	this->addChild(apfel);
	
	direction = -1;


	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);



	auto eventListener = EventListenerKeyboard::create();


    eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){
		bool moved = false; //check if ever used
		
		auto moveSnakeBody = CallFunc::create([=]()
		{
			if (add)
			{
				newSprite = Sprite::create("flirts.png");
				newSprite->setPosition(Vec2(-124,-124));
				snakeVector.push_back(newSprite);
			add = false;
			}

			int headprevx = snakeHead->getPositionX(); //the prev x position of the head
			int headprevy = snakeHead->getPositionY(); //the prev y position of the head
			int realheadx = headprevx;
			int realheady = headprevy;

			switch(direction)
			{
				case 0: headprevy -= 62;
					break;
				case 1: headprevx -= 62;
					break;
				case 2: headprevy += 62;
					break;
				case 3: headprevx += 62;
					break;
			}

			int currIndex = 0;
			int lastIndex = snakeVector.size() - 1;
			int tempx;
			int tempy;

			

			
			for (int x = 0; x <snakeVector.size(); x++)
			{
				Sprite *snakeBody = snakeVector.at(x);
			//}
			//for (auto snakeBody: snakeVector)
			//{
				CCLOG("%d is curindex",currIndex);
				tempx = snakeBody->getPositionX(); //curr xPos of the body
				tempy = snakeBody->getPositionY(); //curr yPos of the body
				

				snakeBody->setPosition(headprevx, headprevy);
				
				if(currIndex != lastIndex)
				{
					if(realheadx < headprevx && realheady == headprevy && headprevx == tempx && headprevy > tempy) 
					{
						snakeBody->setTextureRect(Rect(128, 1, 62, 62));
					}
					
					if(realheadx == headprevx && headprevx == tempx && realheady != headprevy && headprevy != tempy)
					{
						snakeBody->setTextureRect(Rect(127, 63, 62, 62));
					}

					if(realheadx > headprevx && realheady == headprevy && headprevx == tempx && headprevy > tempy) 
					{
						snakeBody->setTextureRect(Rect(3, 1, 62, 62));
					}

					if(realheadx == headprevx && realheady > headprevy && headprevx > tempx && headprevy == tempy) 
					{
						snakeBody->setTextureRect(Rect(126, 125, 62, 62));
					}

					if(realheady == headprevy && headprevy == tempy && realheadx != headprevx && headprevx != tempx)
					{
						snakeBody->setTextureRect(Rect(65, 1, 62, 62));
					}

					if(realheadx == headprevx && realheady < headprevy && headprevx > tempx && headprevy == tempy) 
					{
						snakeBody->setTextureRect(Rect(129, 1, 62, 62));
					}

					if(realheadx > headprevx && realheady == headprevy && headprevx == tempx && headprevy < tempy) 
					{
						snakeBody->setTextureRect(Rect(3, 63, 62, 62));
					}

					if(realheadx < headprevx && realheady == headprevy && headprevx == tempx && headprevy < tempy) 
					{
						snakeBody->setTextureRect(Rect(127, 125, 62, 62));
					}

					if(realheadx == headprevx && realheady < headprevy && headprevx < tempx && headprevy == tempy) 
					{
						snakeBody->setTextureRect(Rect(3, 1, 62, 62));
					}

					if(realheadx == headprevx && realheady > headprevy && headprevx < tempx && headprevy == tempy) 
					{
						snakeBody->setTextureRect(Rect(3, 63, 62, 62));
					}

				}
				else
				{
					if(realheadx == headprevx && realheady > headprevy)
					{
						snakeBody->setTextureRect(Rect(193, 128, 62, 62));
					}

					if(realheadx == headprevx && realheady < headprevy)
					{
						snakeBody->setTextureRect(Rect(257, 194, 62, 62));
					}

					if(realheadx > headprevx && realheady == headprevy)
					{
						snakeBody->setTextureRect(Rect(258, 129, 62, 62));
					}

					if(realheadx < headprevx && realheady == headprevy)
					{
						snakeBody->setTextureRect(Rect(192, 193, 62, 62));
					}
				}

				realheadx = headprevx;
				realheady = headprevy;
				headprevx = tempx;
				headprevy = tempy;


				currIndex++;
				
	//prev = Point(snakeHead->getPositionX(), snakeHead->getPositionY());

			}
		});


		auto faceLeft = CallFunc::create([=]()
		{
			snakeHead->setTextureRect(Rect(195, 65, 62, 62));
		});

		auto faceRight = CallFunc::create([=]()
		{
			snakeHead->setTextureRect(Rect(256, 1, 62, 62));
		});

		auto faceUp = CallFunc::create([=]()
		{
			snakeHead->setTextureRect(Rect(193, 2, 62, 62));
		});

		auto faceDown = CallFunc::create([=]()
		{
			snakeHead->setTextureRect(Rect(257, 64, 62, 62));
		});

		

		auto up = MoveBy::create(0, Point(0, 62));
		auto down = MoveBy::create(0, Point(0, -62));
		auto left = MoveBy::create(0, Point(-62, 0));
		auto right = MoveBy::create(0, Point(62, 0));

		DelayTime *delayAction = DelayTime::create(1.0);

		auto delayedUp = Sequence::create(delayAction, up, faceUp, moveSnakeBody, NULL);
		auto delayedDown = Sequence::create(delayAction, down, faceDown, moveSnakeBody, NULL);
		auto delayedLeft = Sequence::create(delayAction, left, faceLeft, moveSnakeBody, NULL);
		auto delayedRight = Sequence::create(delayAction, right, faceRight, moveSnakeBody, NULL);
		
		auto foreverUp = cocos2d::RepeatForever::create(delayedUp);
		auto foreverDown = cocos2d::RepeatForever::create(delayedDown);
		auto foreverLeft = cocos2d::RepeatForever::create(delayedLeft);
		auto foreverRight = cocos2d::RepeatForever::create(delayedRight);


        //Vec2 loc = event->getCurrentTarget()->getPosition();

        switch(keyCode){
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            case EventKeyboard::KeyCode::KEY_A:

				//event->getCurrentTarget()->setAnchorPoint(Vec2(0, 0));

				//sprite = Sprite::create("flirts.png", Rect(194, 65, 62, 62))s;
				//event->getCurrentTarget()->stopAllActions();


				if((direction != 1 && direction != 3) || direction == -1)
				{
					event->getCurrentTarget()->stopAllActions();
					direction = 3;
					event->getCurrentTarget()->runAction(foreverLeft);
				}
                break;
            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            case EventKeyboard::KeyCode::KEY_D:
               // event->getCurrentTarget()->setPosition(++loc.x,loc.y);
				if((direction != 1 && direction != 3) || direction == -1)
				{
					event->getCurrentTarget()->stopAllActions();
					direction = 1;
					event->getCurrentTarget()->runAction(foreverRight);
				}
                break;
            case EventKeyboard::KeyCode::KEY_UP_ARROW:
            case EventKeyboard::KeyCode::KEY_W:
                //event->getCurrentTarget()->setPosition(loc.x,++loc.y);
				if((direction != 0 && direction != 2) || direction == -1)
				{
					event->getCurrentTarget()->stopAllActions();
					direction = 0;
					event->getCurrentTarget()->runAction(foreverUp);
				}
                break;
            case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            case EventKeyboard::KeyCode::KEY_S:
                //event->getCurrentTarget()->setPosition(loc.x,--loc.y);
				if((direction != 0 && direction != 2) || direction == -1)
				{
					event->getCurrentTarget()->stopAllActions();
					direction = 2;
					event->getCurrentTarget()->runAction(foreverDown);
				}
                break;
        }
    };

    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, snakeHead);

	//float a = this->convertToWorldSpace(sprite->getPositionX);

	//while(sprite->getPos  < 558)
	//sprite->runAction(repeat);
	
	this->schedule(schedule_selector(HelloWorld::update));

	this->scheduleUpdate();
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

bool HelloWorld::onContactBegin(PhysicsContact &contact) {
	//PhysicsBody 
	auto a = contact.getShapeA()->getBody()->getNode();
	//PhysicsBody
	auto b = contact.getShapeB()->getBody()->getNode();

	int x;
	int y;

	int headsegx;
	int headsegy;
	int currsegx;
	int currsegy;
	int prevsegx;
	int prevsegy;

	if ((a->getTag()==1&&b->getTag()==2)||(a->getTag()==2&&b->getTag()==1)) {
		Director::getInstance()->pause();
	}

	if ((a->getTag()==1&&b->getTag()==3)||(a->getTag()==3&&b->getTag()==1)) {

		Sprite newSeg;

		if (b->getTag()==3) {
			x = a->getPosition().x;
			y = a->getPosition().y;

			while (x==a->getPositionX())
				x = cocos2d::RandomHelper::random_int(1,8)*62;

			while (y==a->getPositionY())
				y = cocos2d::RandomHelper::random_int(1,8)*62;

			b->runAction(MoveTo::create(0,Point(x,y)));


			/*switch(direction)
			{
				case 0:
						currsegx = a->getPositionX();
						currsegy = a->getPositionY();

						a->runAction(Sequence::create(DelayTime::create(1.0), MoveBy::create(0, Point(0, 62)), NULL));

						prevsegx = snakeVector.at(0)->getPositionX();
						prevsegy = snakeVector.at(0)->getPositionY();


						if(currsegx == prevsegx && currsegy != prevsegy)
						{
							auto newSeg = Sprite::create("flirts.png", Rect(127, 63, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
							//snakeVector.insert(snakeVector.begin(), newSeg);
						}

						if(currsegx > prevsegx && currsegy == prevsegy)
						{

							auto newSeg = Sprite::create("flirts.png", Rect(127, 125, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
						}

						if(currsegx < prevsegx && currsegy == prevsegy)
						{
							auto newSeg = Sprite::create("flirts.png", Rect(3, 63, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
						}


					break;
				case 1:
						currsegx = a->getPositionX();
						currsegy = a->getPositionY();

						a->runAction(Sequence::create(DelayTime::create(1.0), MoveBy::create(0, Point(62, 0)), NULL));

						prevsegx = snakeVector.at(0)->getPositionX();
						prevsegy = snakeVector.at(0)->getPositionY();

						if(currsegy == prevsegy && currsegx != prevsegx)
						{
							auto newSeg = Sprite::create("flirts.png", Rect(65, 1, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
						}

						if(currsegy > prevsegy && currsegx == prevsegx)
						{

							auto newSeg = Sprite::create("flirts.png", Rect(3, 1, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
						}

						if(currsegy < prevsegy && currsegx == prevsegx)
						{
							auto newSeg = Sprite::create("flirts.png");
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_back(newSeg);
						}

					break;
				case 2:
						currsegx = a->getPositionX();
						currsegy = a->getPositionY();

						a->runAction(Sequence::create(DelayTime::create(1.0), MoveBy::create(0, Point(0, -62)), NULL));

						prevsegx = snakeVector.at(0)->getPositionX();
						prevsegy = snakeVector.at(0)->getPositionY();


						if(currsegx == prevsegx && currsegy != prevsegy)
						{
							auto newSeg = Sprite::create("flirts.png", Rect(127, 63, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
						}

						if(currsegx > prevsegx && currsegy == prevsegy)
						{

							auto newSeg = Sprite::create("flirts.png", Rect(129, 1, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
						}

						if(currsegx < prevsegx && currsegy == prevsegy)
						{
							auto newSeg = Sprite::create("flirts.png", Rect(3, 1, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
						}



					break;
				case 3:
						currsegx = a->getPositionX();
						currsegy = a->getPositionY();

						a->runAction(Sequence::create(DelayTime::create(1.0), MoveBy::create(0, Point(-62, 0)), NULL));

						prevsegx = snakeVector.at(0)->getPositionX();
						prevsegy = snakeVector.at(0)->getPositionY();

						if(currsegy == prevsegy && currsegx != prevsegx)
						{
							auto newSeg = Sprite::create("flirts.png", Rect(65, 1, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
						}

						if(currsegy > prevsegy && currsegx == prevsegx)
						{

							auto newSeg = Sprite::create("flirts.png", Rect(129, 1, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
						}

						if(currsegy < prevsegy && currsegx == prevsegx)
						{
							auto newSeg = Sprite::create("flirts.png", Rect(127, 125, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
						}
					break;*/
add = true;

		}
		else
		{
			x = b->getPosition().x;
			y = b->getPosition().y;

			while(x == b->getPosition().x)
				x = cocos2d::RandomHelper::random_int(1, 8) * 62;

			while(y == b->getPosition().y)
				y = cocos2d::RandomHelper::random_int(1, 8) * 62;

			a->runAction(MoveTo::create(0, Point(x, y)));

			/*switch(direction)
			{
				case 0:
						currsegx = b->getPositionX();
						currsegy = b->getPositionY();

						b->runAction(Sequence::create(DelayTime::create(1.0), MoveBy::create(0, Point(0, 62)), NULL));

						prevsegx = snakeVector.at(0)->getPositionX();
						prevsegy = snakeVector.at(0)->getPositionY();


						if(currsegx == prevsegx && currsegy != prevsegy)
						{
							auto newSeg = Sprite::create("flirts.png", Rect(127, 63, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
						}

						if(currsegx > prevsegx && currsegy == prevsegy)
						{

							auto newSeg = Sprite::create("flirts.png", Rect(127, 125, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
						}

						if(currsegx < prevsegx && currsegy == prevsegy)
						{
							auto newSeg = Sprite::create("flirts.png", Rect(3, 63, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
						}


					break;
				case 1:
						currsegx = b->getPositionX();
						currsegy = b->getPositionY();

						b->runAction(Sequence::create(DelayTime::create(1.0), MoveBy::create(0, Point(62, 0)), NULL));

						prevsegx = snakeVector.at(0)->getPositionX();
						prevsegy = snakeVector.at(0)->getPositionY();

						if(currsegy == prevsegy && currsegx != prevsegx)
						{
							auto newSeg = Sprite::create("flirts.png", Rect(65, 1, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
						}

						if(currsegy > prevsegy && currsegx == prevsegx)
						{

							auto newSeg = Sprite::create("flirts.png", Rect(3, 1, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
						}

						if(currsegy < prevsegy && currsegx == prevsegx)
						{
							auto newSeg = Sprite::create("flirts.png", Rect(3, 63, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
						}

					break;
				case 2:
						currsegx = b->getPositionX();
						currsegy = b->getPositionY();

						b->runAction(Sequence::create(DelayTime::create(1.0), MoveBy::create(0, Point(0, -62)), NULL));

						prevsegx = snakeVector.at(0)->getPositionX();
						prevsegy = snakeVector.at(0)->getPositionY();


						if(currsegx == prevsegx && currsegy != prevsegy)
						{
							auto newSeg = Sprite::create("flirts.png", Rect(127, 63, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
						}

						if(currsegx > prevsegx && currsegy == prevsegy)
						{

							auto newSeg = Sprite::create("flirts.png", Rect(129, 1, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
						}

						if(currsegx < prevsegx && currsegy == prevsegy)
						{
							auto newSeg = Sprite::create("flirts.png", Rect(3, 1, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
						}



					break;
				case 3:
						currsegx = b->getPositionX();
						currsegy = b->getPositionY();

						b->runAction(Sequence::create(DelayTime::create(1.0), MoveBy::create(0, Point(-62, 0)), NULL));

						prevsegx = snakeVector.at(0)->getPositionX();
						prevsegy = snakeVector.at(0)->getPositionY();

						if(currsegy == prevsegy && currsegx != prevsegx)
						{
							auto newSeg = Sprite::create("flirts.png", Rect(65, 1, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
						}

						if(currsegy > prevsegy && currsegx == prevsegx)
						{

							auto newSeg = Sprite::create("flirts.png", Rect(129, 1, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
						}

						if(currsegy < prevsegy && currsegx == prevsegx)
						{
							auto newSeg = Sprite::create("flirts.png", Rect(127, 125, 62, 62));
							newSeg->setPosition(currsegx, currsegy);
							snakeVector.push_front(newSeg);
						}
					break;
			}*/

add = true;

		}

		if ((a->getTag()==2&&b->getTag()==3)||(a->getTag()==3&&b->getTag()==2)) {
			if (b->getTag()==3) {
				x = a->getPosition().x;
				y = a->getPosition().y;

				while (x==a->getPosition().x)
					x = cocos2d::RandomHelper::random_int(1,8)*62;

				while (y==a->getPosition().y)
					y = cocos2d::RandomHelper::random_int(1,8)*62;

				b->runAction(MoveTo::create(0,Point(x,y)));

			} else {
				x = b->getPosition().x;
				y = b->getPosition().y;

				while (x==b->getPosition().x)
					x = cocos2d::RandomHelper::random_int(1,8)*62;

				while (y==b->getPosition().y)
					y = cocos2d::RandomHelper::random_int(1,8)*62;

				a->runAction(MoveTo::create(0,Point(x,y)));
			}
		}

		return false;
	}
}

void HelloWorld::update(float dt)
{



	if(snakeHead->getPosition().x == -62)
	{
		snakeHead->setPositionX(558);
	}

	if(snakeHead->getPosition().x == 620)
	{
		snakeHead->setPositionX(0);
	}

	if(snakeHead->getPosition().y == -62)
	{
		snakeHead->setPositionY(558);
	}

	if(snakeHead->getPosition().y == 620)
	{
		snakeHead->setPositionY(0);
	}
}