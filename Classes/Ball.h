//
//  Ball.h
//  breakout
//
//  Created by dormouse666 on 2016/04/10.
//
//

#ifndef __breakout__Ball__
#define __breakout__Ball__

#include "cocos2d.h"

class Ball : public cocos2d::Node
{
public:
    
    Ball();
    virtual ~Ball();
    
    virtual bool init() override;
    CREATE_FUNC(Ball);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
    //ボール生成
    void setUpball();
    
    //移動距離
    void setVerticalLength(float length){_vLength = length;};
    void setHorizonLength(float length){_hLength = length;};
    float getVerticalLength(){return _vLength;};
    float getHorizonLength(){return _hLength;};
    
private:
    cocos2d::Sprite* _ball;
    float _vLength; //vertical
    float _hLength;  //horizon
    
};

#endif /* defined(__breakout__Ball__) */
