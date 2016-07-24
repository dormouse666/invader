//
//  GameLogic.h
//  invader
//
//  Created by dormouse666 on 2016/07/20.
//
//

#ifndef GameLogic_h
#define GameLogic_h

#include "cocos2d.h"

class Piece;
class Ball;

class GameLogic
{
public:
    
    //当たり判定
    static int isCrashEnemy(std::vector<Piece*> pieceMap, std::vector<Ball*> ballList); //点数返す
    static bool isCrashBall(std::vector<Ball*> ballList, std::vector<Ball*> enemyBallList);
    static bool isCrashPlayer(std::vector<Ball*> enemyBallList, cocos2d::Sprite* player);
    
};

#endif /* GameLogic_h */
