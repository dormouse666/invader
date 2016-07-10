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

namespace  {
    //static const double PI = 3.141592653589793;
    static const int LENGTH = 1;
    static const int LENGTH_ENEMY = -1;
    static const char* PLAYER_IMG_NAME = "images/player.png";
    static const char* HIGH_SCORE_NAME = "highScore";
    static const float ENEMY_MOVE_INTERVAL_DEFAULT = 1.0f;
    static const int ENEMY_MOVE_DISTANCE = 8;
    static const float ENEMY_WIDTH = 10.0f;
    static const int ENEMY_NUM = 40; //敵の総数
    static const int ENEMY_ROW_NUM = 8; //横列に何匹置くか
}



// コンストラクタ
Top::Top()
: _visibleSize(0,0)
, _origin(0,0)
, _backGround(nullptr)
, _state(NOMAL)
, _player(nullptr)
, _isPlayerTap(false)
, _isPlayerMove(false)
, _firstTapPos(0,0)
, _piece(nullptr)
, _gameStartLabel(nullptr)
, _gameOverLabel(nullptr)
, _gameClearLabel(nullptr)
, _score(0)
, _scoreLabel(nullptr)
, _highScore(0)
, _highScoreLabel(nullptr)
, _userDefault(nullptr)
, _enemyMove(RIGHT)
, _elapse(0.0f)
, _enemyMoveInterval(ENEMY_MOVE_INTERVAL_DEFAULT)
, _isBump(false)
, _playerNum(0)
{
    if(_pieceMap.size() > 0)
    {
        _pieceMap.clear();
    }
    
    if(_ballList.size() > 0)
    {
        _ballList.clear();
    }
    
    if(_enemyBallList.size() > 0)
    {
        _enemyBallList.clear();
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
    
    //ユーザのハイスコア情報取得
    _userDefault = UserDefault::getInstance();
    _highScore = _userDefault->getIntegerForKey(HIGH_SCORE_NAME, 0); //defaultValueが入ってれば無い時はそれをreturnしてくれるぽい？
    
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
    
    //点数表示
    this->setScore();
    this->setHighScore();
    
    //block
    this->setBlock();
    
    //player
    this->setPlayer();
    
    //スタートラベル出す
    this->entryGameStart();
}

void Top::onExit()
{
    Layer::onExit();
}

//ゲームスタート
void Top::startGame()
{
    //update
    this->scheduleUpdate();
    
    //ラベル消す
    if(_gameStartLabel)
    {
        _gameStartLabel->removeFromParent();
        _gameStartLabel = nullptr;
    };
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
    //ボール作成
    auto ball = Ball::create();
    ball->setAnchorPoint(Point(0.5f, 0.5f));
    ball->setPosition(_player->getPosition().x,
                       _origin.y + (_backGround->getPosition().y - _backGround->getContentSize().height / 2));
    
    //進む距離セット まっすぐ進めるのでYのみ
    ball->setVerticalLength(LENGTH);
    //_ball->setHorizonLength(LENGTH);
    
    _ballList.push_back(ball);
    this->addChild(ball);
    
    /*
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
        case PLAYING:
        {
            //ボール消す
            //_ball->removeFromParent();
            ///_ball = nullptr;
            
            //_state = NOMAL; //state戻す
            
            break;
        }
        default:
            break;
    }
     */
}

//角度1〜2までの間でボールの進む距離をランダムでセット
void Top::setBallLengthRandom(double degreeA, double degreeB)
{
    /*
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
     */
}

//update
void Top::update(float dt)
{
    //上限
    auto topPos = _backGround->getPosition().y + _backGround->getContentSize().height/2;
    
    //CCLOG("rightPos: %f", rightPos);
    //CCLOG("leftPos: %f", leftPos);
    
    //経過時刻
    _elapse += dt;
    
    //敵動く
    this->enemyMove();
    
    //敵の弾が自分に当たったらゲームオーバー
    bool isCrashPlayer = this->isCrashPlayer();
    if(isCrashPlayer)
    {
        this->entryGameOver();
    }
    
    //ボール同士がぶつかったかどうか
    this->isCrashBall();

    //ボールがあれば動かす
    if(_ballList.size() > 0)
    {
        for (auto ball: _ballList)
        {
            ball->setPosition(ball->getPosition().x + ball->getHorizonLength(),
                              ball->getPosition().y + ball->getVerticalLength());
        }
    }
    
    //pieceと衝突したかどうか
    auto isCrash = this->isCrash();
    
    if(isCrash)
    {
        //crashした結果、敵が0になってたらクリア
        if(_pieceMap.size() <= 0)
        {
            this->entryGameClear();
        }
        
        return;
    }

    //上にぶつかったらボール消す
    if(_ballList.size() > 0)
    {
        for(int i = 0; i < _ballList.size(); i++)
        {
            if(_ballList[i]->getPosition().y >= topPos + _ballList[i]->getContentSize().height/2)
            {
                /*
                 _ballList[i]->removeFromParent();
                 _ballList.erase(_ballList.begin() + i);
                 */
                
                //消すフラグ立てる
                _ballList[i]->setIsLiving(false);
            }
        }
    }
    
    //生存チェック、死んでたら消す
    if(_ballList.size() > 0)
    {
        for(int i = 0; i < _ballList.size(); i++)
        {
            if(_ballList[i]->getIsLiving() == false)
            {
                _ballList[i]->removeFromParent();
                _ballList.erase(_ballList.begin() + i);
            }
        }
    }
    
    if(_pieceMap.size() > 0)
    {
        for(int i = 0; i < _pieceMap.size(); i++)
        {
            if(_pieceMap[i]->getIsLiving() == false)
            {
                _pieceMap[i]->removeFromParent();
                _pieceMap.erase(_pieceMap.begin() + i);
            }
        }
    }
    
    if(_enemyBallList.size() > 0)
    {
        for(int i = 0; i < _enemyBallList.size(); i++)
        {
            if(_enemyBallList[i]->getIsLiving() == false)
            {
                _enemyBallList[i]->removeFromParent();
                _enemyBallList.erase(_enemyBallList.begin() + i);
            }
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
    
    //残機設定
    _playerNum = 5;
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
        
        _isPlayerMove = true;
    }
}

//player
void Top::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(!_player)
    {
        return;
    }
    
    //ボール出す
    if(_isPlayerTap && !_isPlayerMove)
    {
        this->entryBallCallback(event);
    }
    
    //フラグ戻す
    _isPlayerTap = false;
    _isPlayerMove = false;
}

//敵のかたまり
void Top::setBlock()
{
    if(_piece)
    {
        _piece = nullptr;
    }
    
    //ブロック作成（仮）
    for(int i = 0; i < ENEMY_NUM; i++)
    {
        _piece = Piece::create();
        
        Piece::ColorType type; //色変え（仮）
        if(i % 3 == 0)
        {
            type = Piece::ColorType::BLUE;
        }
        else if(i % 3 == 1)
        {
            type = Piece::ColorType::GREEN;
        }
        else
        {
            type = Piece::ColorType::RED;
        }
        _piece->setUpPiece(type);
        
        //一旦、カラータイプで点数わけ
        int point = 0;
        switch (type) {
            case Piece::ColorType::BLUE:
                point = 10;
                break;
                
            case Piece::ColorType::GREEN:
                point = 20;
                break;
                
            case Piece::ColorType::RED:
                point = 30;
                break;
                
            default:
                break;
        }
        _piece->setPoint(point);
        
        //敵の配置
        auto pieceSize = _piece->getContentSize();

        int row = i / ENEMY_ROW_NUM; //何列目か(1少ない)
        int column = i % ENEMY_ROW_NUM; //左から何個目か
        float x = (_backGround->getPosition().x - _backGround->getContentSize().width / 2) + (ENEMY_WIDTH * column) + (column * 5); //ちょっと隙間開ける(5)
        float y =  _origin.y + (_visibleSize.height * 0.7) + (pieceSize.height * row);
        
        _piece->setPosition(x, y);
        
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

//自分の弾と敵が衝突したかどうか判定
bool Top::isCrash()
{
    bool isCrash = false;
    
    if(_pieceMap.size() <= 0
       || _ballList.size() <= 0)
    {
        isCrash = false;
    }
    
    //自分のボールが敵に当たったか
    for(int i = 0; i < _ballList.size(); i++)
    {
        if(_ballList[i]->getIsLiving() == false)
        {
            break;
        }
        
        auto ballPos = _ballList[i]->getPosition();
        auto ballSize = _ballList[i]->getContentSize();
        
        for(int k = 0; k < _pieceMap.size(); k++)
        {
            if(_pieceMap[k]->getIsLiving() == false)
            {
                break;
            }
            
            auto piecePos = _pieceMap[k]->getParent()->convertToWorldSpace(_pieceMap[k]->getPosition());
            auto pieceSize = _pieceMap[k]->getContentSize();
            
            //pieceの左以上右以下かつ、下以上上以下なら衝突
            if(ballPos.x + ballSize.width / 2 >= piecePos.x
               && ballPos.x - ballSize.width / 2 <= piecePos.x + pieceSize.width
               && ballPos.y + ballSize.height / 2 >= piecePos.y
               && ballPos.y + ballSize.height / 2 <= piecePos.y + pieceSize.height)
            {
                //点数加算＆更新
                _score = _score + _pieceMap[k]->getPoint();
                this->setScore();
                
                //ハイスコア更新
                if(_score > _highScore)
                {
                    _highScore = _score;
                    this->setHighScore();
                    
                    //ユーザ情報も更新
                    _userDefault->setIntegerForKey(HIGH_SCORE_NAME, _highScore);
                }
                
                /*
                //消す
                _pieceMap[k]->removeFromParent();
                _pieceMap.erase(_pieceMap.begin() + k);
                
                //ボールも消す
                _ballList[i]->removeFromParent();
                _ballList.erase(_ballList.begin() + i);
                 */
                
                //敵消えるフラグ立てる
                _pieceMap[k]->setIsLiving(false);
                
                //自分のボール消すフラグ立てる
                _ballList[i]->setIsLiving(false);
                
                isCrash = true;
                
                break; //1体しか消さない
            }
        }
    }
    return isCrash;
}

//自分の弾と敵の弾が衝突したかどうか判定
bool Top::isCrashBall()
{
    bool isCrash = false;
    
    if(_ballList.size() <= 0
       || _enemyBallList.size() <= 0)
    {
        return false;
    }
    
    for(int i = 0; i < _ballList.size(); i++)
    {
        if(_ballList[i]->getIsLiving() == false)
        {
            break;
        }
        
        auto ballPos = _ballList[i]->getPosition();
        auto ballSize = _ballList[i]->getContentSize();
        
        for(int k = 0; k < _enemyBallList.size(); k++)
        {
            if(_enemyBallList[k]->getIsLiving() == false)
            {
                break;
            }
            
            auto enemyBallPos = _enemyBallList[k]->getPosition();
            auto enemyBallSize = _enemyBallList[k]->getContentSize();
            
            //ballの左がenemyBallの右以下、
            //ballの右がenemyBallの左以上、
            //ballの上がenemyBallの下以上、
            //ballの上がenemyBallの上以下
            if(ballPos.x + ballSize.width / 2 >= enemyBallPos.x - enemyBallSize.width //敵弾左(広め)
               && ballPos.x - ballSize.width / 2 <= enemyBallPos.x + enemyBallSize.width //敵弾右(広め)
               && ballPos.y + ballSize.height / 2 >= enemyBallPos.y - enemyBallSize.height / 2 //敵弾下
                && ballPos.y + ballSize.height / 2 <= enemyBallPos.y + enemyBallSize.height / 2  //敵弾上
               )
            {
                /*
                //敵のボール消す
                _enemyBallList[k]->removeFromParent();
                _enemyBallList.erase(_enemyBallList.begin() + k);
                
                //自分のボール消す
                _ballList[i]->removeFromParent();
                _ballList.erase(_ballList.begin() + i);
                 */
                
                //敵のボール消えるフラグ立てる
                _enemyBallList[k]->setIsLiving(false);
                
                //自分のボール消すフラグ立てる
                _ballList[i]->setIsLiving(false);
                
                isCrash = true;
                
                break; //消すのは1つだけ
            }
        }
    }
    return isCrash;
}

//敵の弾と自分が衝突したかどうか
bool Top::isCrashPlayer()
{
    bool isCrash = false;
    
    if(_enemyBallList.size() <= 0)
    {
        return false;
    }
    
    //敵のボールが自分に当たったか
    for(int i = 0; i < _enemyBallList.size(); i++)
    {
        if(_enemyBallList[i]->getIsLiving() == false)
        {
            break;
        }
        
        auto ballPos = _enemyBallList[i]->getPosition();
        auto ballSize = _enemyBallList[i]->getContentSize();
        
        //playerの左以上右以下かつ、下以上上以下なら衝突
        auto playerPos = _player->getPosition();
        auto playerSize = _player->getContentSize();
        
        //playerはアンカーが(0.5,1.0)なので
        if(ballPos.x + ballSize.width/2 >= playerPos.x - playerSize.width/2
           && ballPos.x - ballSize.width/2 <= playerPos.x + playerSize.width/2
           //&& ballPos.y + ballSize.height/2 >= playerPos.y - playerSize.height
           && ballPos.y - ballSize.height/2 - 1 <= playerPos.y) //ちょっとゲタ履かせとく
        {
            isCrash = true;
        }
    }
    return isCrash;
}

//GameOver
void Top::entryGameOver()
{
    //updateを止める
    this->unscheduleUpdate();
    
    //ボール消す
    if(_ballList.size() > 0)
    {
        for(auto ball: _ballList)
        {
            ball->removeFromParent();
            ball = nullptr;
        }
        _ballList.clear();
    }
    
    //敵の弾も消す
    if(_enemyBallList.size() > 0)
    {
        for(auto ball: _enemyBallList)
        {
            ball->removeFromParent();
            ball = nullptr;
        }
        _enemyBallList.clear();
    }
    
    //ゲームオーバーしたよラベル生成
    _gameOverLabel = Label::createWithSystemFont("Game Over", "HiraKakuProN-W6", 25);
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
    listener->onTouchEnded     = [this, listener] (Touch* touch, Event* event)
    {
        //状態リセット
        this->reset();
        
        //リスナーをremove
        //auto dispatcher = Director::getInstance()->getEventDispatcher();
        //dispatcher->removeEventListener(listener);
    };
    listener->onTouchCancelled = listener->onTouchEnded;
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _gameOverLabel);
    
    //追加
    this->addChild(_gameOverLabel);
}

//リセット
void Top::reset()
{
    //ラベル類を消す
    if(_gameStartLabel)
    {
        _gameStartLabel->removeFromParent();
        _gameStartLabel = nullptr;
    };
    if(_gameOverLabel)
    {
        _gameOverLabel->removeFromParent();
        _gameOverLabel = nullptr;
    };
    if(_gameClearLabel)
    {
        _gameClearLabel->removeFromParent();
        _gameClearLabel = nullptr;
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
    if(_ballList.size() > 0)
    {
        for(auto ball: _ballList)
        {
            ball->removeFromParent();
            ball = nullptr;
        }
        _ballList.clear();
    }
    
    //敵の弾も消す
    if(_enemyBallList.size() > 0)
    {
        for(auto ball: _enemyBallList)
        {
            ball->removeFromParent();
            ball = nullptr;
        }
        _enemyBallList.clear();
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
    
    //点数0に戻す
    _score = 0;
    this->setScore();
    
    Top::onEnter();
}

//点数更新
void Top::setScore()
{
    if(_scoreLabel) //あるなら使う
    {
        //_scoreLabel->removeFromParent();
        //_scoreLabel = nullptr;
        
        _scoreLabel->setString(StringUtils::format("Score: %d", _score));
        
        return;
    }
    
    _scoreLabel = Label::createWithSystemFont(StringUtils::format("Score: %d", _score), "HiraKakuProN-W6", 10);
    _scoreLabel->setPosition(Point(_origin.x + _visibleSize.width / 2,
                                  (_origin.y + _visibleSize.height) * 0.98));
    this->addChild(_scoreLabel);
}

//ハイスコア更新
void Top::setHighScore()
{
    if(_highScoreLabel)  //あるなら使う
    {
        //_highScoreLabel->removeFromParent();
        //_highScoreLabel = nullptr;
        
        _highScoreLabel->setString(StringUtils::format("HighScore: %d", _highScore));
        
        return;
    }
    
    _highScoreLabel = Label::createWithSystemFont(StringUtils::format("HighScore: %d", _highScore), "HiraKakuProN-W6", 10);
    _highScoreLabel->setPosition(Point(_origin.x + _visibleSize.width / 2,
                                   (_origin.y + _visibleSize.height) * 0.93));
    this->addChild(_highScoreLabel);
}

//敵動かす
void Top::enemyMove()
{
    //右限 TODO:ちょい多くなってきたのでどっかで共通化
    auto rightPos = _backGround->getPosition().x + _backGround->getContentSize().width/2;
    //左限
    auto leftPos = _backGround->getPosition().x - _backGround->getContentSize().width/2;
    //下限
    auto underPos = _backGround->getPosition().y - _backGround->getContentSize().height/2;
    
    //敵の弾動かす
    if(_enemyBallList.size() > 0)
    {
        for (auto ball: _enemyBallList)
        {
            ball->setPosition(ball->getPosition().x + ball->getHorizonLength(),
                              ball->getPosition().y + ball->getVerticalLength());
        }
    }
    
    //下にぶつかったら敵の弾消す
    for(int i = 0; i < _enemyBallList.size(); i++)
    {
        if(_enemyBallList[i]->getPosition().y <= underPos + _enemyBallList[i]->getContentSize().height/2)
        {
            /*
            _enemyBallList[i]->removeFromParent();
            _enemyBallList.erase(_enemyBallList.begin() + i);
             */
            
            _enemyBallList[i]->setIsLiving(false);
        }
    }
    
    //経過時間がインターバル超えてたら敵動かす
    if (_elapse >= _enemyMoveInterval)
    {
        //限界判定
        for(auto piece : _pieceMap)
        {
            if (_enemyMove == RIGHT)
            {
                //どっかが右限界に行ってないか確認
                if(rightPos < piece->getPositionX() + piece->getContentSize().width + ENEMY_MOVE_DISTANCE)
                {
                    _enemyMove = LEFT;
                    _isBump = true;
                    break;
                }
            }
            else if(_enemyMove == LEFT)
            {
                //左限界に行ってないか確認
                if(leftPos > piece->getPositionX() - ENEMY_MOVE_DISTANCE)
                {
                    _enemyMove = RIGHT;
                    _isBump = true;
                    break;
                }
            }
        }
        
        //雑に動かす
        for(auto piece : _pieceMap)
        {
            //端にぶつかってたら前に移動
            if(_isBump)
            {
                //下限界に行ってたらgameOver
                if(underPos > piece->getPositionY() - ENEMY_MOVE_DISTANCE)
                {
                    this->entryGameOver();
                    break;
                }
                
                piece->setPositionY(piece->getPositionY() - ENEMY_MOVE_DISTANCE);
            }
            else
            {
                //左右
                if (_enemyMove == RIGHT)
                {
                    piece->setPositionX(piece->getPositionX() + ENEMY_MOVE_DISTANCE);
                }
                else if(_enemyMove == LEFT)
                {
                    piece->setPositionX(piece->getPositionX() - ENEMY_MOVE_DISTANCE);
                }
            }
            
            //見た目も変えるぞ
            auto lookState = piece->getLookState();
            switch (lookState) {
                case Piece::LookState::HANDS_UP:
                    piece->setLookState(Piece::LookState::HANDS_DOWN);
                    break;
                case Piece::LookState::HANDS_DOWN:
                    piece->setLookState(Piece::LookState::HANDS_UP);
                    break;
                default:
                    break;
            }
            piece->lookChange();
        }
        
        //敵も弾発射する
        this->enemyAttack();
        
        _elapse = 0.0f;
        _isBump = false;
    }
}

//ゲームクリア
void Top::entryGameClear()
{
    //updateを止める
    this->unscheduleUpdate();
    
    //ボール消す
    if(_ballList.size() > 0)
    {
        for(auto ball: _ballList)
        {
            ball->removeFromParent();
            ball = nullptr;
        }
        _ballList.clear();
    }
    
    //敵の弾も消す
    if(_enemyBallList.size() > 0)
    {
        for(auto ball: _enemyBallList)
        {
            ball->removeFromParent();
            ball = nullptr;
        }
        _enemyBallList.clear();
    }
    
    //ゲームクリアしたよラベル生成
    _gameClearLabel = Label::createWithSystemFont("Game Clear!!", "HiraKakuProN-W6", 25);
    _gameClearLabel->setPosition(Point(_origin.x + _visibleSize.width / 2, _origin.y + _visibleSize.height / 2));
    
    //ラベルにタッチイベント付ける
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan     = [this] (Touch* touch, Event* event)
    {
        return true;
    };
    listener->onTouchMoved     = [ ] (Touch* touch, Event* event) {  };
    listener->onTouchEnded     = [this] (Touch* touch, Event* event)
    {
        //状態リセット
        this->reset();
    };
    listener->onTouchCancelled = listener->onTouchEnded;
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _gameClearLabel);
    
    //追加
    this->addChild(_gameClearLabel);
}

//ゲームスタート
void Top::entryGameStart()
{
    //ラベル生成
    _gameStartLabel = Label::createWithSystemFont("Game Start", "HiraKakuProN-W6", 25);
    _gameStartLabel->setPosition(Point(_origin.x + _visibleSize.width / 2, _origin.y + _visibleSize.height / 2));
    
    //ラベルにタッチイベント付ける
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan     = [this] (Touch* touch, Event* event)
    {
        return true;
    };
    listener->onTouchMoved     = [ ] (Touch* touch, Event* event) {  };
    listener->onTouchEnded     = [this] (Touch* touch, Event* event)
    {
        //スタート
        this->startGame();
    };
    listener->onTouchCancelled = listener->onTouchEnded;
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _gameStartLabel);
    
    //追加
    this->addChild(_gameStartLabel);
}

//敵も弾発射
void Top::enemyAttack()
{
    //どいつが弾飛ばすかランダム
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_int_distribution<int> randomIntDistribution(0, static_cast<int>(_pieceMap.size()) - 1);
    int num = randomIntDistribution(mt);
    
    //座標
    float posX = _pieceMap[num]->getPositionX();
    float posY = _pieceMap[num]->getPositionY();
    for(int i = 0; i < _pieceMap.size(); i++)
    {
        if(i == num)
        {
            continue;
        }
        
        //横が同じで縦が手前側ならそっちが弾出す
        if(_pieceMap[i]->getPositionX() == posX)
        {
            if(_pieceMap[i]->getPositionY() < posY)
            {
                if(num > i) //数が小さい方が手前
                {
                    num = i;
                }
            }
        }
    }
    
    //ボール作成
    auto ball = Ball::create();
    ball->setAnchorPoint(Point(0.5f, 0.5f));
    ball->setPosition(_pieceMap[num]->getPositionX() + _pieceMap[num]->getContentSize().width / 2,
                      _pieceMap[num]->getPositionY());
    
    //進む距離セット まっすぐ進めるのでYのみ
    ball->setVerticalLength(LENGTH_ENEMY);
    
    _enemyBallList.push_back(ball);
    this->addChild(ball);

}