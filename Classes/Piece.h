//
//  Piece.h
//  breakout
//
//  Created by dormouse666 on 2016/05/07.
//
//

#ifndef Piece_h
#define Piece_h

#include "cocos2d.h"

class Piece : public cocos2d::Node
{
public:
    enum ColorType {
        BLUE,
        GREEN,
        RED,
    };
    
    enum LookState {
        HANDS_UP,
        HANDS_DOWN,
    };
    
public:
    
    Piece();
    virtual ~Piece();
    
    virtual bool init() override;
    CREATE_FUNC(Piece);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
    //ボール生成
    void setUpPiece(ColorType type);
    
    //点数を返す処理
    int getPoint(){return _point;};
    void setPoint(int point){_point = point;};
    
    //見た目
    LookState getLookState(){return _lookState;};
    void setLookState(LookState lookState){_lookState = lookState;};
    void lookChange();
    
private:
    ColorType _colorType;
    int _point;
    LookState _lookState;
    
    cocos2d::Sprite* _pieceSpr;
};

#endif /* Piece_h */
