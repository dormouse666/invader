//
//  Piece.cpp
//  breakout
//
//  Created by dormouse666 on 2016/05/07.
//
//

#include "Piece.h"

USING_NS_CC;

//コンストラクタ
Piece::Piece()
: _pieceSpr(nullptr)
, _colorType(RED)
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
        {BLUE, "images/block1.png"},
        {GREEN, "images/block2.png"},
        {RED, "images/block3.png"},
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
