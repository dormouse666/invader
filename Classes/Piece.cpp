//
//  Piece.cpp
//  breakout
//
//  Created by dormouse666 on 2016/05/07.
//
//

#include "Piece.h"

USING_NS_CC;

namespace  {
    static const char* IMG_RED_HANDS_UP = "images/invader_red_1.png";
    static const char* IMG_RED_HANDS_DOWN = "images/invader_red_2.png";
    static const char* IMG_BLUE_HANDS_UP = "images/invader_blue_1.png";
    static const char* IMG_BLUE_HANDS_DOWN = "images/invader_blue_2.png";
    static const char* IMG_GREEN_HANDS_UP = "images/invader_green_1.png";
    static const char* IMG_GREEN_HANDS_DOWN = "images/invader_green_2.png";
}

//コンストラクタ
Piece::Piece()
: _pieceSpr(nullptr)
, _colorType(RED)
, _point(0)
, _lookState(HANDS_UP)
{
}

// デストラクタ
Piece::~Piece()
{
}

// initは初期化
bool Piece::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    
    return true;
}

// onEnterは生成
void Piece::onEnter()
{
    Node::onEnter();
    
    //setUpPiece(_colorType); //TODO:色をランダムに
}

void Piece::onExit()
{
    Node::onExit();
}

void Piece::setUpPiece(ColorType type)
{
    _colorType = type; //まだ使ってないが多分どっかで使う
    
    //前のスプライト消す
    if(_pieceSpr)
    {
        _pieceSpr->removeFromParent();
        _pieceSpr = nullptr;
    }
    
    //テーブル化する
    static std::map<ColorType, const char*> imgPathTbl = {
        {BLUE, IMG_BLUE_HANDS_UP},
        {GREEN, IMG_GREEN_HANDS_UP},
        {RED, IMG_RED_HANDS_UP},
    };
    
    //要素チェック
    if(imgPathTbl.find(type) == imgPathTbl.end())
    {
        return;
    }

    //スプライト作る
    _pieceSpr = Sprite::create(imgPathTbl[type]);
    if(_pieceSpr)
    {
        this->setContentSize(_pieceSpr->getContentSize()); //piece自体の大きさを合わせる
        
        //pieceNodeの真ん中にsprを置く
        _pieceSpr->setAnchorPoint(Point(0.5f, 0.5f));
        _pieceSpr->setPosition(this->getContentSize().width/2, this->getContentSize().height/2);
        this->addChild(_pieceSpr);
    }
}

//見た目変える
void Piece::lookChange()
{
    //前のスプライト消す
    if(_pieceSpr)
    {
        _pieceSpr->removeFromParent();
        _pieceSpr = nullptr;
    }

    //スプライト作る
    switch (_colorType) {
        case BLUE:
            if(_lookState == HANDS_UP)
            {
                _pieceSpr = Sprite::create(IMG_BLUE_HANDS_UP);
            }
            else if(_lookState == HANDS_DOWN)
            {
                _pieceSpr = Sprite::create(IMG_BLUE_HANDS_DOWN);
            }
            break;
            
        case GREEN:
            if(_lookState == HANDS_UP)
            {
                _pieceSpr = Sprite::create(IMG_GREEN_HANDS_UP);
            }
            else if(_lookState == HANDS_DOWN)
            {
                _pieceSpr = Sprite::create(IMG_GREEN_HANDS_DOWN);
            }
            break;
            
        case RED:
            if(_lookState == HANDS_UP)
            {
                _pieceSpr = Sprite::create(IMG_RED_HANDS_UP);
            }
            else if(_lookState == HANDS_DOWN)
            {
                _pieceSpr = Sprite::create(IMG_RED_HANDS_DOWN);
            }
            break;
            
        default:
            break;
    }
    
    if(_pieceSpr)
    {
        this->setContentSize(_pieceSpr->getContentSize()); //piece自体の大きさを合わせる
        
        //pieceNodeの真ん中にsprを置く
        _pieceSpr->setAnchorPoint(Point(0.5f, 0.5f));
        _pieceSpr->setPosition(this->getContentSize().width/2, this->getContentSize().height/2);
        this->addChild(_pieceSpr);
    }
}
