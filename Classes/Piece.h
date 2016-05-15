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
    
public:
    
    Piece();
    virtual ~Piece();
    
    virtual bool init() override;
    CREATE_FUNC(Piece);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
    //ボール生成
    void setUpPiece(ColorType type);
    
private:
    ColorType _colorType;
    cocos2d::Sprite* _pieceSpr;
};

#endif /* Piece_h */
