//
//  GameLogic.cpp
//  invader
//
//  Created by dormouse666 on 2016/07/20.
//
//

#include "GameLogic.h"

#include "Ball.h"
#include "Piece.h"

//敵と弾が衝突
int GameLogic::isCrashEnemy(std::vector<Piece*> pieceMap, std::vector<Ball*> ballList)
{
    //加算する点数
    int getPoint = 0;
    
    if(pieceMap.size() <= 0
       || ballList.size() <= 0)
    {
        return getPoint;
    }
    
    //ボールがpieceに当たったか
    for(int i = 0; i < ballList.size(); i++)
    {
        if(ballList[i]->getIsLiving() == false)
        {
            break;
        }
        
        auto ballPos = ballList[i]->getPosition();
        auto ballSize = ballList[i]->getContentSize();
        
        for(int k = 0; k < pieceMap.size(); k++)
        {
            if(pieceMap[k]->getIsLiving() == false)
            {
                break;
            }
            
            auto piecePos = pieceMap[k]->getParent()->convertToWorldSpace(pieceMap[k]->getPosition());
            auto pieceSize = pieceMap[k]->getContentSize();
            
            //pieceの左以上右以下かつ、下以上上以下なら衝突
            if(ballPos.x + ballSize.width / 2 >= piecePos.x
               && ballPos.x - ballSize.width / 2 <= piecePos.x + pieceSize.width
               && ballPos.y + ballSize.height / 2 >= piecePos.y
               && ballPos.y + ballSize.height / 2 <= piecePos.y + pieceSize.height)
            {
                //敵消えるフラグ立てる
                pieceMap[k]->setIsLiving(false);
                
                //自分のボール消すフラグ立てる
                ballList[i]->setIsLiving(false);
                
                //加算する点数
                getPoint = pieceMap[k]->getPoint();
                
                break; //1体しか消さない
            }
        }
    }
    return getPoint;
}

//弾と敵の弾が衝突
bool GameLogic::isCrashBall(std::vector<Ball*> ballList, std::vector<Ball*> enemyBallList)
{
    bool isCrash = false;
    
    if(ballList.size() <= 0
       || enemyBallList.size() <= 0)
    {
        return false;
    }
    
    for(int i = 0; i < ballList.size(); i++)
    {
        if(ballList[i]->getIsLiving() == false)
        {
            break;
        }
        
        auto ballPos = ballList[i]->getPosition();
        auto ballSize = ballList[i]->getContentSize();
        
        for(int k = 0; k < enemyBallList.size(); k++)
        {
            if(enemyBallList[k]->getIsLiving() == false)
            {
                break;
            }
            
            auto enemyBallPos = enemyBallList[k]->getPosition();
            auto enemyBallSize = enemyBallList[k]->getContentSize();
            
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
                //敵のボール消えるフラグ立てる
                enemyBallList[k]->setIsLiving(false);
                
                //自分のボール消すフラグ立てる
                ballList[i]->setIsLiving(false);
                
                isCrash = true;
                
                break; //消すのは1つだけ
            }
        }
    }
    return isCrash;
}

//敵の弾と自分が衝突したかどうか
bool GameLogic::isCrashPlayer(std::vector<Ball*> enemyBallList, cocos2d::Sprite* player)
{
    bool isCrash = false;
    
    if(enemyBallList.size() <= 0)
    {
        return false;
    }
    
    //敵のボールが自分に当たったか
    for(int i = 0; i < enemyBallList.size(); i++)
    {
        if(enemyBallList[i]->getIsLiving() == false)
        {
            break;
        }
        
        auto ballPos = enemyBallList[i]->getPosition();
        auto ballSize = enemyBallList[i]->getContentSize();
        
        //playerの左以上右以下かつ、下以上上以下なら衝突
        auto playerPos = player->getPosition();
        auto playerSize = player->getContentSize();
        
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
