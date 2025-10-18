#pragma once

using namespace geode::prelude;

class AdvancedScrollLayer : public CCLayer {
    public:

        static AdvancedScrollLayer* create(CCSize size, CCSize limits);

        bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        void ccTouchMoved(CCTouch* touch, CCEvent* event) override;
        void ccTouchesMoved(CCSet* touches, CCEvent* event) override;
        void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;

        virtual void keyDown(enumKeyCodes key)  override;
        virtual void keyUp(enumKeyCodes key)  override;
        
        CCNode* content;

        bool scrollMovement = true;

        void setLimits(CCSize newLimits);

        const CCSize getAccurateContentSize() {
            auto viewSize = this->getContentSize();
            auto contentSize = zoomParent->getContentSize();
            float scale = zoomParent->getScale();

            auto computeMin = [&](float view, float content) {
                float scaleFactor = view / content;
                float diff = view - content;
                return ((scale - scaleFactor) / (1 - scaleFactor)) * (diff / 2) / scale;
            };

            return ccp(computeMin(viewSize.width, contentSize.width), computeMin(viewSize.height, contentSize.height));
        }

        void drawGrid(float squaresAmount, float lineWidth, ccColor4B color);

        void addFromCenter(CCNode* child, CCPoint offset = {0, 0});

        void toggleHorizontalScrollbar(bool b);
        void toggleVerticalScrollbar(bool b);
        
        void setScrollSensetivity(float sense);

        void moveBy(CCPoint amount);
        void moveTo(CCPoint pos);
        
        void resetPositioning();

        void moveToCorner(bool left, bool bottom);

        void zoomBy(float zoomAmount);
        void zoomTo(float zoomAmount);

        void zoomToMinimum();
        void zoomToMaximum();

        float getCurrentZoom() const { return zoomParent->getScale(); };

        void setEnabled(bool b);

        Scrollbar* horizontalBar;
        Scrollbar* verticalBar;

        float minZoom = 2;
        float maxZoom = .5f;

    protected:

        bool init(CCSize size, CCSize limits);

        CCNode* zoomParent;

        float scrollSense = 1;

        float prevTouchDelta = -1;

        bool isEnabled = true;

        CCScrollLayerExt* widthScrollExt;
        CCScrollLayerExt* heightScrollExt;

        float getMinimumPosition(bool horizontal);
        float getMaximumPosition(bool horizontal);
        void updateProgressBars();
        void updatePositionByProgBars(float dt);

        bool touchDown = false;

        void scrollWheel(float y, float x) override;
        void registerWithTouchDispatcher() override;

        CCDrawNode* gridNode = nullptr;
        float gridsquareSize;
        float gridLineWidth;
        ccColor4B gridColor;

        bool holdingShift;
};