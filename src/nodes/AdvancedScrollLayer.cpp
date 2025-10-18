#include "AdvancedScrollLayer.hpp"

AdvancedScrollLayer* AdvancedScrollLayer::create(CCSize size, CCSize limits) {
    auto ret = new AdvancedScrollLayer();
    if (ret && ret->init(size, limits)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}


bool AdvancedScrollLayer::init(CCSize size, CCSize limits){
    if (!CCLayer::init()) return false;
    
    this->setContentSize(size);

    CCDrawNode* stencil = CCDrawNode::create();

    CCPoint rect[4] = {
        ccp(0, 0),
        ccp(size.width, 0),
        ccp(size.width, size.height),
        ccp(0, size.height)
    };

    stencil->drawPolygon(rect, 4, ccc4f(1, 1, 1, 1), 0, ccc4f(1, 1, 1, 1));

    CCClippingNode* clip = CCClippingNode::create(stencil);
    clip->setAnchorPoint(ccp(0, 0));
    clip->setPosition(ccp(0, 0));
    clip->setContentSize(size);

    zoomParent = CCNode::create();
    zoomParent->setContentSize(limits);
    zoomParent->setAnchorPoint({.5f, .5f});
    zoomParent->setPosition(size / 2);
    clip->addChild(zoomParent);

    content = CCNode::create();
    content->setContentSize(limits);
    zoomParent->addChild(content);

    widthScrollExt = new CCScrollLayerExt({0, 0, size.height, size.width});
    widthScrollExt->autorelease();
    widthScrollExt->setMouseEnabled(false);
    widthScrollExt->setTouchEnabled(false);
    widthScrollExt->setKeyboardEnabled(false);
    widthScrollExt->setKeypadEnabled(false);
    
    horizontalBar = Scrollbar::create(widthScrollExt);
    horizontalBar->setPositionX(size.width / 2);
    horizontalBar->setPositionY(-4);
    horizontalBar->setRotation(90);
    this->addChild(horizontalBar);

    heightScrollExt = new CCScrollLayerExt({0, 0, size.width, size.height});
    heightScrollExt->autorelease();
    heightScrollExt->setMouseEnabled(false);
    heightScrollExt->setTouchEnabled(false);
    heightScrollExt->setKeyboardEnabled(false);
    heightScrollExt->setKeypadEnabled(false);

    verticalBar = Scrollbar::create(heightScrollExt);
    verticalBar->setPositionY(size.height / 2);
    verticalBar->setPositionX(size.width + 4);
    this->addChild(verticalBar);

    updateProgressBars();

    resetPositioning();

    schedule(schedule_selector(AdvancedScrollLayer::updatePositionByProgBars));

    this->addChild(clip);

    this->setKeypadEnabled(true);
    this->setKeyboardEnabled(true);
    this->setMouseEnabled(true);
    this->setTouchEnabled(true);
    
    return true;
}

float AdvancedScrollLayer::getMinimumPosition(bool horizontal){
    CCSize viewSize = this->getContentSize();
    CCSize contentSize = zoomParent->getContentSize();
    
    float scaleFactor;

    if (horizontal)
        scaleFactor = viewSize.width / contentSize.width;
    else
        scaleFactor = viewSize.height / contentSize.height;

    float length;

    if (horizontal)
        length = viewSize.width - contentSize.width;
    else
        length = viewSize.height - contentSize.height;
    
    return (zoomParent->getScale() - scaleFactor) / (1 - scaleFactor) * (length / 2) / zoomParent->getScale();
}

float AdvancedScrollLayer::getMaximumPosition(bool horizontal){
    return -getMinimumPosition(horizontal);
}

void AdvancedScrollLayer::updateProgressBars(){
    widthScrollExt->m_contentLayer->setPositionY(content->getPositionX() - getMinimumPosition(true) * -1);
    widthScrollExt->m_contentLayer->setContentHeight(abs(getMinimumPosition(true) * 2) + zoomParent->getPositionX() * 2);

    heightScrollExt->m_contentLayer->setPositionY(content->getPositionY() - getMinimumPosition(false) * -1);
    heightScrollExt->m_contentLayer->setContentHeight(abs(getMinimumPosition(false) * 2) + zoomParent->getPositionY() * 2);
}

void AdvancedScrollLayer::updatePositionByProgBars(float dt){
    content->setPositionX(widthScrollExt->m_contentLayer->getPositionY() - getMinimumPosition(true));

    content->setPositionY(heightScrollExt->m_contentLayer->getPositionY() - getMinimumPosition(false));
}

bool AdvancedScrollLayer::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    if (!isEnabled) return true;

    auto const size = this->getContentSize();
    auto const pos = this->convertToNodeSpace(touch->getLocation());
    auto const rect = CCRect{0, 0, size.width, size.height};

    if (!rect.containsPoint(pos)) return false;

    this->ccTouchMoved(nullptr, event);

    touchDown = true;

    return true;
}

void AdvancedScrollLayer::ccTouchEnded(CCTouch*, CCEvent*) {
    touchDown = false;
}

void AdvancedScrollLayer::ccTouchCancelled(CCTouch*, CCEvent*) {
    touchDown = false;
}

void AdvancedScrollLayer::ccTouchMoved(CCTouch* touch, CCEvent*){
    if (!isEnabled || touch == nullptr) return;

    if (!this->boundingBox().containsPoint(this->getParent()->convertToNodeSpace(touch->getLocation()))) return;

    CCPoint curr = this->convertToNodeSpace(touch->getLocationInView());
    CCPoint prev = this->convertToNodeSpace(touch->getPreviousLocationInView());
    CCPoint delta = curr - prev;
    
    moveBy(delta * scrollSense * ccp(1, -1) / zoomParent->getScale());
}

void AdvancedScrollLayer::ccTouchesMoved(CCSet* touches, CCEvent* event){
    if (!isEnabled || touches == nullptr) return;
    
    if (touches->count() != 2){
        ccTouchMoved(static_cast<CCTouch*>(touches->anyObject()), event);
        return;
    }
    CCSetIterator it = touches->begin();
    CCTouch* touch1 = static_cast<CCTouch*>(*it);
    ++it;
    CCTouch* touch2 = static_cast<CCTouch*>(*it);

    if (!this->boundingBox().containsPoint(this->getParent()->convertToNodeSpace(touch1->getLocation()))) return;

    float distance = touch1->getLocation().getDistance(touch2->getLocation());

    zoomBy(prevTouchDelta - distance);

    prevTouchDelta = distance;
}

void AdvancedScrollLayer::scrollWheel(float y, float x) {
    if (!isEnabled) return;

    if (!scrollMovement)
        zoomBy(-y * 0.01f);
    else{
        if (CCKeyboardDispatcher::get()->getControlKeyPressed())
            zoomBy(-y * 0.01f);
        else if (!holdingShift)
            moveBy(ccp(x, y) * scrollSense);
        else
            moveBy(ccp(y, x) * scrollSense);
    }

    //log::info("{}", ccp(y, x));
}

void AdvancedScrollLayer::keyDown(enumKeyCodes key){
    if (key == enumKeyCodes::KEY_LeftShift){
        holdingShift = true;
    }

    CCLayer::keyDown(key);
}

void AdvancedScrollLayer::keyUp(enumKeyCodes key){
    if (key == enumKeyCodes::KEY_LeftShift){
        holdingShift = false;
    }
}

void AdvancedScrollLayer::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void AdvancedScrollLayer::setLimits(CCSize newLimits){
    CCSize newLimitedLimits = content->getContentSize();
    if (newLimits.width > this->getContentWidth()) newLimitedLimits.width = newLimits.width;
    if (newLimits.height > this->getContentHeight()) newLimitedLimits.height = newLimits.height;

    content->setContentSize(newLimitedLimits);
    zoomParent->setContentSize(newLimitedLimits);

    if (gridNode != nullptr)
        drawGrid(gridsquareSize, gridLineWidth, gridColor);

    moveTo(content->getPosition());
}

void AdvancedScrollLayer::drawGrid(float squareSize, float lineWidth, ccColor4B color){
    if (gridNode != nullptr) gridNode->clear();
    else{
        gridNode = CCDrawNode::create();
        gridNode->m_bUseArea = false;
        content->addChild(gridNode);
    }

    gridsquareSize = squareSize;
    gridLineWidth = lineWidth;
    gridColor = color;

    std::vector<CCPoint> gridLines{};

    float sizeX = content->getContentWidth();
    float sizeY = content->getContentHeight();

    float stepX = squareSize;
    for (float i = 0; i <= sizeX; i += stepX)
    {
        gridNode->drawSegment({i, 0}, {i, sizeY}, lineWidth, ccc4FFromccc4B(color));
    }

    float stepY = squareSize;
    for (float i = 0; i <= sizeY; i += stepY)
    {
        gridNode->drawSegment({0, i}, {sizeX, i}, lineWidth, ccc4FFromccc4B(color));
    }
}

void AdvancedScrollLayer::addFromCenter(CCNode* child, CCPoint offset){
    content->addChild(child);
    child->setPosition(content->getContentSize() / 2 + offset);
}

void AdvancedScrollLayer::toggleHorizontalScrollbar(bool b){
    horizontalBar->setVisible(b);
}

void AdvancedScrollLayer::toggleVerticalScrollbar(bool b){
    verticalBar->setVisible(b);
}

void AdvancedScrollLayer::setScrollSensetivity(float sense){
    scrollSense = sense;
}

void AdvancedScrollLayer::moveBy(CCPoint amount){
    moveTo(content->getPosition() + amount);
}

void AdvancedScrollLayer::moveTo(CCPoint newPos){
    CCSize viewSize = this->getContentSize();
    CCSize contentSize = zoomParent->getContentSize();

    float scaleFactorX = viewSize.width / contentSize.width;
    float lengthX = viewSize.width - contentSize.width;
    float minX = (zoomParent->getScale() - scaleFactorX) / (1 - scaleFactorX) * (lengthX / 2) / zoomParent->getScale();
    float maxX = -minX;

    float scaleFactorY = viewSize.height / contentSize.height;
    float lengthY = viewSize.height - contentSize.height;
    float minY = (zoomParent->getScale() - scaleFactorY) / (1 - scaleFactorY) * (lengthY / 2) / zoomParent->getScale();
    float maxY = -minY;

    float clampedX = std::clamp(newPos.x, minX, maxX);
    float clampedY = std::clamp(newPos.y, minY, maxY);

    //log::info("new pos {} | {} - {} | min ({}, {}) | max ({}, {}) | clamped ({}, {})", newPos, viewSize, contentSize, minX, minY, maxX, maxY, clampedX, clampedY);

    content->setPosition({clampedX, clampedY});

    updateProgressBars();
}

void AdvancedScrollLayer::resetPositioning(){
    content->setPosition({0, 0});
    zoomTo(1);
}

void AdvancedScrollLayer::moveToCorner(bool left, bool bottom){
    CCSize corners = zoomParent->getScaledContentSize() / 2;

    float XToGoTo = corners.width * (left ? 1 : -1);
    float YToGoTo = corners.height * (bottom ? 1 : -1);
    
    //log::info("{} | {} || {}", XToGoTo, YToGoTo, corners);

    moveTo({XToGoTo, YToGoTo});
}

void AdvancedScrollLayer::zoomBy(float zoomAmount){
    zoomTo(zoomParent->getScale() + zoomAmount);
}

void AdvancedScrollLayer::zoomTo(float zoomAmount){
    if (zoomParent->getContentHeight() * zoomAmount < this->getContentHeight()) zoomAmount = this->getContentHeight() / zoomParent->getContentHeight();
    if (zoomParent->getContentWidth() * zoomAmount < this->getContentWidth()) zoomAmount = this->getContentWidth() / zoomParent->getContentWidth();
    if (zoomAmount > minZoom) zoomAmount = minZoom;
    if (zoomAmount < maxZoom) zoomAmount = maxZoom;

    zoomParent->setScale(zoomAmount);

    moveTo(content->getPosition());
}

void AdvancedScrollLayer::zoomToMinimum(){
    CCPoint toUse;
    if (zoomParent->getContentWidth() > zoomParent->getContentHeight())
        toUse = ccp(this->getContentWidth(), zoomParent->getContentWidth());
    else
        toUse = ccp(this->getContentHeight(), zoomParent->getContentHeight());

    zoomTo(toUse.x / toUse.y);
}

void AdvancedScrollLayer::zoomToMaximum(){
    zoomTo(minZoom);
}

void AdvancedScrollLayer::setEnabled(bool b){
    isEnabled = b;
}