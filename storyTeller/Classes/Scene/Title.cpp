//
//  Title.cpp
//  storyTeller
//
//  Created by vantan on 2016/07/22.
//
//

#include "Title.hpp"
#include "Home.hpp"
#include "Result.hpp"
#include <fstream>

#include <fstream>

Scene* Title::CreateScene()
{
    Scene* scene = Scene::create();
    Layer* layer = Title::create();
    scene->addChild(layer);
    return  scene;
}

bool Title::init()
{
    if(!Layer::init()){return  false;}
    
    {
        SetImageInfo();
        sprite = Sprite::create("ImageFile/TapEventImage.png");
        sprite->setPosition(Vec2(960,200));
        sprite->setScale(0.4,0.4);
        this->addChild(sprite);
        
        SetBgm();
        SetBlink(2);
    }
        
    //イベントリスナーの生成
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch* touch, Event* event)
    {
        this->getEventDispatcher()->removeAllEventListeners();
       
        se = AudioEngine::play2d("Sound/SE/TouchToButton.wav");
        delay = DelayTime::create(2);
        SetBlink(0.1);
        
        auto startGame = CallFunc::create([]
        {
            Scene* scene = Home::CreateScene();
            TransitionFade* trasition = TransitionFade::create(1, scene);
            Director::getInstance()->replaceScene(trasition);
        });
        
        this->runAction(Sequence::create(delay, startGame, NULL));
        return true;
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

//画像の情報をテキストファイルから呼び出す
void Title::SetImageInfo()
{
    auto filePath = FileUtils::getInstance()->fullPathForFilename("DataFile/TitleImagePath.txt");
    std::ifstream inFile(filePath);
    assert(inFile);
    
    size_t imageValue;
    std::string _filePath;
    Vec2 pos, size;
    
    inFile >> imageValue;
    Sprite* bgSprite[imageValue];
    
    for(int i = 0; i < imageValue; i++)
    {
        //画像のパス、横の座標、縦の座標、
        //横のサイズの比率、縦のサイズの比率の順にデータを読み込む
        inFile >> _filePath >> pos.x >> pos.y >> size.x >> size.y;
        
        //変数に読み込んだデータをもとに情報をセットしていく
        bgSprite[i] = Sprite::create(_filePath);
        bgSprite[i]->setPosition(Vec2(pos.x, pos.y));
        bgSprite[i]->setScale(size.x, size.y);
        this->addChild(bgSprite[i]);
    }
}

void Title::SetBlink(float blinkSpeed)
{
    auto blink = Sequence::create(FadeTo::create(blinkSpeed, 60),FadeTo::create(blinkSpeed, 255), NULL);
     this->scheduleUpdate();
    sprite->runAction(RepeatForever::create(blink));
}

//BGMの設定
void Title::SetBgm()
{
    titleBgm = AudioEngine::play2d("Sound/BGM/title_bgm.wav");
    AudioEngine::setLoop(titleBgm, true);
}

void Title::update(float deltaTime)
{
    CCLOG("ボタンが押されました");
}