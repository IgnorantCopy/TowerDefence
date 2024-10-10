#include "Level1Scene.h"
#include "SelectLevelScene.h"

USING_NS_CC;

Scene* Level1Scene::createScene()
{
    return Level1Scene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in Level1Scene.cpp\n");
}

bool Level1Scene::init()
{
    if ( !Scene::init() ){
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto background = Sprite::create("images/level1_background.png",Rect(0,0,2500,1500));
    if(background == nullptr) {
        problemLoading("'images/level1_background.png'");
    } else {
        background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        this->addChild(background, 0);
    }
    
    auto frameBase = Sprite::create("images/frame_base.png");
    if(frameBase == nullptr) {
        problemLoading("'images/frame_base.png'");
    } else {
        frameBase->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 1500 - visibleSize.height));
        this->addChild(frameBase, 1);
    }
    
    float gap = 300;
    this->selectedTower = Sprite::create("images/towers/archer_base_onblock.png");
    this->selectedTower->setOpacity(0);
    this->selectedTower->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->addChild(this->selectedTower, 5);
    auto archerBaseSelector = MenuItemImage::create(
            "images/towers/archer_base.png",
            "images/towers/archer_base.png",
            [this](Ref *ref) {
                this->isSelecting = true;
                this->selectedTower->setTexture("images/towers/archer_base_onblock.png");
            }
    );
    archerBaseSelector->setPosition(Vec2(origin.x + visibleSize.width / 2 - 2 * gap,
                                         origin.y + 1680 - visibleSize.height));
    
    auto magicianBaseSelector = MenuItemImage::create(
            "images/towers/magician_base.png",
            "images/towers/magician_base.png",
            [this](Ref *ref) {
                this->isSelecting = true;
                this->selectedTower->setTexture("images/towers/magician_base_onblock.png");
            }
    );
    magicianBaseSelector->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                           origin.y + 1680 - visibleSize.height));
    
    auto helperBaseSelector = MenuItemImage::create(
            "images/towers/helper_base.png",
            "images/towers/helper_base.png",
            [this](Ref *ref) {
                this->isSelecting = true;
                this->selectedTower->setTexture("images/towers/helper_base_onblock.png");
            }
    );
    helperBaseSelector->setPosition(Vec2(origin.x + visibleSize.width / 2 + 2 * gap,
                                         origin.y + 1680 - visibleSize.height));
    
    // the back button to go back to the SelectLevel scene
    auto Back=Label::createWithTTF("Back", "fonts/Bender/BENDER.OTF", 75);
    auto backItem=MenuItemLabel::create(
            Back,
            [this](Ref *ref){
                Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4f, SelectLevelScene::createScene()));
            }
    );
    backItem->setPosition(Vec2(origin.x + visibleSize.width - 100,
                               origin.y + visibleSize.height - 50));

    //create map
    float delta = 140;
    float x = origin.x + 350 + delta;
    float y = origin.y + visibleSize.height - delta;
    createMap(1);
    for(size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            Grid::Type type_ = map->grids[map->shape.index_of(i, j)].type;
            if(type_ != Grid::Type::None) {
                grid[i][j] = ui::Button::create(images[type_], images[type_]);
                grid[i][j]->setPosition(Vec2(x + j * SIZE, y - i * SIZE));
                this->addChild(grid[i][j], 2);
            }
        }
    }
    
    auto blockBackground = Sprite::create("images/block_background.png", Rect(0, 0, 1680, 980));
    if(blockBackground == nullptr) {
        problemLoading("'images/block_background.png'");
    } else {
        blockBackground->setPosition(Vec2(x + 5.5f * delta, y - 3 * delta));
        this->addChild(blockBackground, 1);
    }

    Vector<MenuItem*> MenuItems;
    MenuItems.pushBack(backItem);
    MenuItems.pushBack(archerBaseSelector);
    MenuItems.pushBack(magicianBaseSelector);
    MenuItems.pushBack(helperBaseSelector);
    auto menu = Menu::createWithArray(MenuItems);
    this->addChild(menu, MenuItems.size());
    menu->setPosition(Vec2::ZERO);
    
    // add a mouse click event listener
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(Level1Scene::onMouseDown, this);
    mouseListener->onMouseMove = CC_CALLBACK_1(Level1Scene::onMouseMove, this);
    mouseListener->onMouseUp = CC_CALLBACK_1(Level1Scene::onMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    return true;
}

void Level1Scene::onMouseDown(cocos2d::Event *event) {
    EventMouse const* e = (EventMouse*)event;
    float x = e->getCursorX();
    float y = e->getCursorY();
    
    auto particle = ParticleSystemQuad::create("particles/mouse.plist");
    if (particle == nullptr) {
        problemLoading("'particles/mouse.plist'");
    } else {
        particle->setPosition(Vec2(x, y));
        this->addChild(particle, 5);
    }
}

void Level1Scene::onMouseUp(cocos2d::Event *event) {
    EventMouse const* e = (EventMouse*)event;
    if (this->isSelecting && this->selectedTower && e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
        this->selectedTower->setOpacity(0);
        this->isSelecting = false;
    }
}

void Level1Scene::onMouseMove(cocos2d::Event *event) {
    EventMouse const* e = (EventMouse*)event;
    float x = e->getCursorX();
    float y = e->getCursorY();
    
    if (this->isSelecting && this->selectedTower) {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        float delta = 140;
        float typeX = origin.x + 350 + delta;
        float typeY = origin.y + visibleSize.height - delta;
        this->selectedTower->setOpacity(255);
        if (x >= typeX - 0.5f * delta && x <= typeX + 11.5f * delta &&
            y >= typeY - 6.5f * delta && y <= typeY + 0.5f * delta) {
            int indexX = (int)((x - typeX + 0.5f * delta) / delta);
            int indexY = (int)((typeY - y + 0.5f * delta) / delta);
            this->selectedTower->setPosition(Vec2(typeX + indexX * delta, typeY - indexY * delta));
        } else {
            this->selectedTower->setPosition(Vec2(x, y));
        }
    }
}