//
//  Top.cpp
//  breakout
//
//  Created by dormouse666 on 2016/04/03.
//
//

#include "Top.h"
#include "Ball.h"
#include "Piece.h"
#include <math.h>

USING_NS_CC;

static const double PI = 3.141592653589793;
static const int LENGTH = 1;
static const char* PLAYER_IMG_NAME = "images/player.png";

// コンストラクタ
Top::Top()
: _visibleSize(0,0)
, _origin(0,0)
, _backGround(nullptr)
, _ball(nullptr)
, _state(NOMAL)
, _player(nullptr)
, _isPlayerTap(false)
, _firstTapPos(0,0)
, _piece(nullptr)
, _gameOverLabel(nullptr)
{
    if(_pieceMap.size() > 0)
    {
        _pieceMap.clear();
    }
}

// デストラクタ
Top::~Top()
{
}

Scene* Top::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Top::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// initは初期化
bool Top::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

// onEnterは生成
void Top::onEnter()
{
    Layer::onEnter();
    
    // 画面サイズ取得
    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();
    
    //CCLOG("_visibleSize: %f, %f", _visibleSize.width, _visibleSize.height);
    //CCLOG("_origin: %f, %f", _origin.x, _origin.y);
    
    /*
    // Homeに戻るボタン
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(Top::menuCloseCallback, this));
    closeItem->setPosition(Vec2(_origin.x + _visibleSize.width - closeItem->getContentSize().width/2 ,
                                _origin.y + closeItem->getContentSize().height/2));
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
     */
    
    // リセットボタン
    auto resetItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(Top::menuResetCallback, this));
    resetItem->setPosition(Vec2(_origin.x + _visibleSize.width - resetItem->getContentSize().width/2 ,
                                _origin.y + resetItem->getContentSize().height/2));
    auto resetMenu = Menu::create(resetItem, NULL);
    resetMenu->setPosition(Vec2::ZERO);
    this->addChild(resetMenu, 1);
    
    // 背景配置
    _backGround = Node::create();
    if(_backGround)
    {
        //CCLOG("ancherPos: %f, %f",_backGround->getAnchorPoint().x, _backGround->getAnchorPoint().y);
        //CCLOG("pos: %f, %f", _backGround->getPosition().x, _backGround->getPosition().y);
        
        // 背景サイズとか
        _backGround->setContentSize({_visibleSize.width * 0.8f, _visibleSize.height * 0.7f});
        _backGround->setAnchorPoint(Point(0.5f, 0.5f));
        _backGround->setPosition(_origin.x + _visibleSize.width / 2, _origin.y + _visibleSize.height * 0.55);
        
        // 適当に色つけとく
        auto color = LayerGradient::create(Color4B(200, 200, 200, 255), Color4B(50, 50, 50, 255), Point(1, 1));
        color->setContentSize(_backGround->getContentSize());
        _backGround->addChild(color);
        
        //CCLOG("ancherPos: %f, %f",_backGround->getAnchorPoint().x, _backGround->getAnchorPoint().y);
        //CCLOG("pos: %f, %f", _backGround->getPosition().x, _backGround->getPosition().y);
        
        this->addChild(_backGround);
    }
    
    //block
    this->setBlock();
    
    //player
    this->setPlayer();
    
    //update
    this->scheduleUpdate();
}

void Top::onExit()
{
    Layer::onExit();
}

// Homeに戻るボタン
void Top::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

// リセットボタン
void Top::menuResetCallback(Ref* pSender)
{
    this->reset();
}

/*
//ボール発射ボタン
void Top::entryBall()
{
    auto entryBallBtn = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(Top::entryBallCallback, this));
    auto menu = Menu::create(entryBallBtn, NULL);
    menu->setAnchorPoint(Point(0.5f, 0.5f));
    menu->setPosition(_origin.x + _visibleSize.width / 2, _origin.y + entryBallBtn->getContentSize().height);
    this->addChild(menu);
    
    CCLOG("menu: %f, %f", menu->getPosition().x, menu->getPosition().y);
}
*/

//ボール発射
void Top::entryBallCallback(Ref* pSender)
{
    switch (_state) {
        case NOMAL:
        {
            if(_ball)
            {
                return;
            }
            
            //ボール作成
            _ball = Ball::create();
            _ball->setAnchorPoint(Point(0.5f, 0.5f));
            _ball->setPosition(_player->getPosition().x,
                               _origin.y + (_backGround->getPosition().y - _backGround->getContentSize().height / 2));
            
            //角度と進む距離セット
            setBallLengthRandom(-89.0, 90.0);
            
            this->addChild(_ball);
            
            _state = PLAYING; //state戻す
            
            break;
        }
        case PLAYING:  //今ここにはこない
        {
            //ボール消す
            _ball->removeFromParent();
            _ball = nullptr;
            
            _state = NOMAL; //state戻す
            
            break;
        }
        default:
            break;
    }
    
}

//角度1〜2までの間でボールの進む距離をランダムでセット
void Top::setBallLengthRandom(double degreeA, double degreeB)
{
    if(!_ball)
    {
        return;
    }
    
    //進む距離
    int length = LENGTH;
    
    //角度をランダムに
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_real_distribution<double> randDegree(degreeA, degreeB); //memo: a以上b未満らしい
    double radian = randDegree(mt) * PI / 180.0; //ラジアン
    
    //x,y セット
    _ball->setVerticalLength(cos(radian)*length);
    _ball->setHorizonLength(sin(radian)*length);
}

//update
void Top::update(float dt)
{
    if(!_ball)
    {
        return;
    }
    
    //上限
    auto topPos = _backGround->getPosition().y + _backGround->getContentSize().height/2 - _ball->getContentSize().height/2;
    //下限
    auto underPos = _backGround->getPosition().y - _backGround->getContentSize().height/2  + _ball->getContentSize().height/2;
    //右限
    auto rightPos = _backGround->getPosition().x + _backGround->getContentSize().width/2  - _ball->getContentSize().width/2;
    //左限
    auto leftPos = _backGround->getPosition().x - _backGround->getContentSize().width/2 + _ball->getContentSize().width/2;
    
    //CCLOG("rightPos: %f", rightPos);
    //CCLOG("leftPos: %f", leftPos);
    
    //動かす
    _ball->setPosition(_ball->getPosition().x + _ball->getHorizonLength(),
                        _ball->getPosition().y + _ball->getVerticalLength());
    
    //pieceと衝突したかどうか
    auto isCrash = this->isCrash();
    
    //衝突してたら跳ね返す（雑）
    if(isCrash)
    {
        _ball->setVerticalLength(-(_ball->getVerticalLength()));
        _ball->setHorizonLength(-(_ball->getHorizonLength()));
    }

    //上にぶつかった時
    if(_ball->getPosition().y >= topPos)
    {
        //ボールの位置を調整
        _ball->setPositionY(topPos);
        
        //上に進んでるなら下向きに
        if(_ball->getVerticalLength() > 0)
        {
            _ball->setVerticalLength(-(_ball->getVerticalLength()));
        }
    }
    
    //下にぶつかった時
    if(_ball->getPosition().y <= underPos)
    {
        //ボールの位置を調整
        _ball->setPositionY(underPos);
        
        //playerに当たったら跳ね返す
        auto playerPos = _player->getParent()->convertToWorldSpace(_player->getPosition());
        auto right = playerPos.x + _player->getContentSize().width/2;  //右
        auto left = playerPos.x - _player->getContentSize().width/2;   //左
        if((right >= _ball->getPosition().x && left <= _ball->getPosition().x))
        {
            //下に進んでるなら上向きに
            if(_ball->getVerticalLength() < 0)
            {
                _ball->setVerticalLength(-(_ball->getVerticalLength()));
            }
        }
        else
        {
            //ボール消す
            _ball->removeFromParent();
            _ball = nullptr;
            _state = NOMAL; //state戻す
            
            //ゲームオーバー
            this->entryGameOver();
            
            return;
        }
    }
    
    //右にぶつかった時
    if(_ball->getPosition().x >= rightPos)
    {
        //ボールの位置を調整
        _ball->setPositionX(rightPos);
        
        //右に進んでるなら左向きに
        if(_ball->getHorizonLength() > 0)
        {
            _ball->setHorizonLength(-(_ball->getHorizonLength()));
        }
    }
    
    //左にぶつかった時
    if(_ball->getPosition().x <= leftPos)
    {
        //ボールの位置を調整
        _ball->setPositionX(leftPos);
        
        //左に進んでるなら右向きに
        if(_ball->getHorizonLength() < 0)
        {
            _ball->setHorizonLength(-(_ball->getHorizonLength()));
        }
    }
}

//プレイヤー
void Top::setPlayer()
{
    if(_player)
    {
        return;
    }
    
    _player = Sprite::create(PLAYER_IMG_NAME);
    _player->setPosition(_origin.x + _visibleSize.width / 2,
                         _origin.y + (_backGround->getPosition().y - _backGround->getContentSize().height / 2));
    _player->setAnchorPoint(Point(0.5f, 1.0f)); //上に合わせる
    this->addChild(_player);
    
    //EventListener作成
    auto listener = EventListenerTouchOneByOne::create();
    
    //メソッドの設定
    listener->onTouchBegan = CC_CALLBACK_2(Top::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(Top::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(Top::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(Top::onTouchCancelled, this);
    
    //dispatcherに登録
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

//playerをタップしてるかどうか
bool Top::isPlayerTapped(cocos2d::Vec2 tapPos)
{
    if(!_player)
    {
        return false;
    }
    
    auto playerPos = _player->getParent()->convertToWorldSpace(_player->getPosition());
    auto right = playerPos.x + _player->getContentSize().width/2;  //右
    auto left = playerPos.x - _player->getContentSize().width/2;   //左
    auto top = playerPos.y;                                        //上
    auto under = playerPos.y - _player->getContentSize().height;   //下
    if(right >= tapPos.x && left <= tapPos.x && top >= tapPos.y && under <= tapPos.y)
    {
        return true;
    }
    
    return false;
}

//player
bool Top::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if(!_player)
    {
        return false;
    }
    
    auto location = touch->getLocation();
    
    if(isPlayerTapped(location))
    {
        //フラグ立てる
        _isPlayerTap = true;
        
        //タップ位置保存
        _firstTapPos = location;
        
        //ボールがなければボール出す
        if(!_ball)
        {
            this->entryBallCallback(event);
        }
        
        return true;
    }
    
    return false;
}

//player
void Top::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if(!_player)
    {
        return;
    }
    
    //タップ位置
    auto location = touch->getLocation();
    
    //移動距離
    auto move = location - _firstTapPos;
    
    //書き換え
    _firstTapPos = location;
    
    //右限
    auto rightPos = _backGround->getPosition().x + _backGround->getContentSize().width/2;
    //左限
    auto leftPos = _backGround->getPosition().x - _backGround->getContentSize().width/2;
    
    if(_isPlayerTap)
    {
        auto now = _player->getPosition().x + move.x;
        if(now + _player->getContentSize().width/2 >= rightPos)
        {
            now = rightPos - _player->getContentSize().width/2;
        }
        if(now - _player->getContentSize().width/2  <= leftPos)
        {
            now = leftPos + _player->getContentSize().width/2;
        }
        _player->setPositionX(now); //yは変更しない
    }
}

//player
void Top::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(!_player)
    {
        return;
    }
    
    //フラグ戻す
    _isPlayerTap = false;
}

//block
void Top::setBlock()
{
    if(_piece)
    {
        _piece = nullptr;
    }
    
    //ブロック作成（仮）
    for(int i = 0; i < 15; i++)
    {
        _piece = Piece::create();
        
        Piece::ColorType type; //色変え（仮）
        if(i % 2 == 0)
        {
            type = Piece::ColorType::BLUE;
        }
        else
        {
            type = Piece::ColorType::RED;
        }
        _piece->setUpPiece(type);
        
        auto pieceSize = _piece->getContentSize();
        _piece->setPosition((_backGround->getPosition().x - _backGround->getContentSize().width / 2) + pieceSize.width * i,
                            _origin.y + _visibleSize.height / 2); //だいぶ適当
        
        //マップに入れる
        _pieceMap.push_back(_piece);
    }
    
    //表示
    if(_pieceMap.size() > 0)
    {
        for(auto piece : _pieceMap)
        {
            this->addChild(piece);
        }
    }
}

//衝突したかどうか判定
bool Top::isCrash()
{
    auto ballPos = _ball->getPosition(); //TODO:ballのサイズを勘案
    
    if(_pieceMap.size() <= 0)
    {
        return false;
    }
    
    for(int i = 0; i < _pieceMap.size(); i++)
    {
        auto piecePos = _pieceMap[i]->getParent()->convertToWorldSpace(_pieceMap[i]->getPosition());
        auto pieceSize = _pieceMap[i]->getContentSize();
        
        //pieceの左以上右以下かつ、下以上上以下なら衝突
        if(ballPos.x >= piecePos.x
           && ballPos.x <= piecePos.x + pieceSize.width
           && ballPos.y >= piecePos.y
           && ballPos.y <= piecePos.y + pieceSize.height)
        {
            _pieceMap[i]->removeFromParent();
            _pieceMap.erase(_pieceMap.begin() + i);
            
            CCLOG("i = %d", i);
            
            return true;
        }
    }
    
    return false;
}

//GameOver
void Top::entryGameOver()
{
    //ゲームオーバーしたよラベル生成
    _gameOverLabel = Label::createWithSystemFont("Game Over", "HiraKakuProN-W6", 30);
    _gameOverLabel->setPosition(Point(_origin.x + _visibleSize.width / 2, _origin.y + _visibleSize.height / 2));
    
    //ラベルにタッチイベント付ける
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan     = [this] (Touch* touch, Event* event)
    {
        /* ラベルタッチだけに反応させたい場合はこう
        auto pt = _gameOverLabel->convertTouchToNodeSpace(touch);
        auto size = _gameOverLabel->getContentSize();
        auto boundingBox = Rect(0, 0, size.width, size.height);
        bool onoff = boundingBox.containsPoint(pt);
        return onoff;
         */
        
        return true;
    };
    listener->onTouchMoved     = [ ] (Touch* touch, Event* event) {  };
    listener->onTouchEnded     = [this] (Touch* touch, Event* event)
    {
        //状態リセット
        this->reset();
    };
    listener->onTouchCancelled = listener->onTouchEnded;
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _gameOverLabel);
    
    //追加
    this->addChild(_gameOverLabel);
}

//リセット
void Top::reset()
{
    if(_gameOverLabel)
    {
        _gameOverLabel->removeFromParent();
        _gameOverLabel = nullptr;
    };
    
    //ブロック消す
    if(_pieceMap.size() > 0)
    {
        for(auto piece : _pieceMap)
        {
            piece->removeFromParent();
            piece = nullptr;
        }
        _pieceMap.clear();
    }
    
    //ボール消す
    if(_ball)
    {
        _ball->removeFromParent();
        _ball = nullptr;
    }
    
    //背景消す
    if(_backGround)
    {
        _backGround->removeFromParent();
        _backGround = nullptr;
    }
    
    //player消す
    if(_player)
    {
        _player->removeFromParent();
        _player = nullptr;
    }
    
    //_state戻す
    _state = NOMAL;
    
    Top::onEnter();
}