//
//  Top.h
//  breakout
//
//  Created by dormouse666 on 2016/04/03.
//
//

#ifndef __breakout__Top__
#define __breakout__Top__

#include "cocos2d.h"

class Ball;
class Piece;

class Top : public cocos2d::Layer
{
public:
    //仮
    enum State {
        NOMAL,   //通常
        PLAYING, //ボールがある状態
    };
    
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init() override;
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Top);
    
    // ここから
    Top();
    virtual ~Top();
    
    void onEnter() override;
    void onExit() override;
    
    //void entryBall();
    void entryBallCallback(cocos2d::Ref* pSender);
    void menuResetCallback(cocos2d::Ref* pSender);
    
    void update(float dt) override;
    
    //player
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;
    //void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void setPlayer();
    bool isPlayerTapped(cocos2d::Vec2 pos);
    
    //piece
    void setBlock();
    
    //start
    void startGame();
    void entryGameStart();
    
    //gameOver
    void entryGameOver();
    void reset();
    
    //クリア
    void entryGameClear();
    
public:
    //角度A〜Bまでの間でボールの進む距離をランダムでセット
    void setBallLengthRandom(double degreeA, double degreeB);
    
    //自分の弾と敵が衝突したかどうか判定
    bool isCrash();
    
    //自分の弾と敵の弾が衝突したかどうか判定
    bool isCrashBall();
    
    //敵の弾と自分が衝突したかどうか判定
    bool isCrashPlayer();

public:
    std::vector<Ball*> _ballList;
    std::vector<Piece*> _pieceMap;
    
    //敵の弾リスト
    std::vector<Ball*> _enemyBallList;
    
private:
    //点数更新
    void setScore();
    void setHighScore();
    
    //敵動かす
    void enemyMove();
    void enemyAttack(); //攻撃

private:
    State _state;
    
    cocos2d::Size _visibleSize;
    cocos2d::Vec2 _origin;
    
    cocos2d::Node* _backGround;
    
    cocos2d::Sprite* _player;
    bool _isPlayerTap;
    cocos2d::Vec2 _firstTapPos;
    int _playerNum;
    bool _isPlayerMove;
    
    Piece* _piece;
    
    cocos2d::Label* _gameStartLabel;
    cocos2d::Label* _gameOverLabel;
    cocos2d::Label* _gameClearLabel;
    
    //点数
    int _score;
    cocos2d::Label* _scoreLabel;
    int _highScore;
    cocos2d::Label* _highScoreLabel;
    
    //アプリに保存する情報
    cocos2d::UserDefault* _userDefault;
    
    //敵がどっち方向に進んでるか
    enum EnemyMove {
        RIGHT,
        LEFT,
    };
    
    EnemyMove _enemyMove;
    
    //敵の動くタイミング制御
    float _elapse;
    float _enemyMoveInterval;
    
    //敵が端にぶつかったか
    bool _isBump;
};

#endif /* defined(__breakout__Top__) */
