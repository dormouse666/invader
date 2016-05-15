//
//  Ball.cpp
//  breakout
//
//  Created by dormouse666 on 2016/04/10.
//
//

#include "Ball.h"

USING_NS_CC;

static const char* BALL_IMG_NAME = "images/ball.png";

//コンストラクタ
Ball::Ball()
: _ball(nullptr)
, _vLength(0.0f)
, _hLength(0.0f)
{
}

// デストラクタ
Ball::~Ball()
{
}

// initは初期化
bool Ball::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    
    return true;
}

// onEnterは生成
void Ball::onEnter()
{
    Node::onEnter();
    
    setUpball();
}

void Ball::onExit()
{
    Node::onExit();
}

void Ball::setUpball()
{
    //前のスプライト消す
    if(_ball)
    {
        _ball->removeFromParent();
        _ball = nullptr;
    }
    
    //スプライト作る
    _ball = Sprite::create(BALL_IMG_NAME);
    if(_ball)
    {
        this->addChild(_ball);
    }
}
