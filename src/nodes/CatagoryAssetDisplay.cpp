#include "CatagoryAssetDisplay.hpp"

CatagoryAssetDisplay* CatagoryAssetDisplay::create() {
    auto ret = new CatagoryAssetDisplay();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CatagoryAssetDisplay::init() {
    if (!CCMenu::init()) return false;

    this->setPosition({0, 0});
    this->ignoreAnchorPointForPosition(false);

    return true;
}

void CatagoryAssetDisplay::setAsset(const std::string& catagoryName, const std::optional<unsigned int>& itemID){
    currentAsset.assetID = itemID;
    currentAsset.catagoryResoueceName = catagoryName;
    if (m_primarySprite) {
        this->removeChild(m_primarySprite, true);
        m_primarySprite = nullptr;
    }
    if (m_secondarySprite) {
        this->removeChild(m_secondarySprite, true);
        m_secondarySprite = nullptr;
    }
    if (m_noncolorSprite) {
        this->removeChild(m_noncolorSprite, true);
        m_noncolorSprite = nullptr;
    }
    if (m_model) {
        this->removeChild(m_model, true);
        m_model = nullptr;
    }

    if (!itemID.has_value()){
        this->setContentSize({0, 0});
        return;
    }

    std::thread([this, itemID, catagoryName]() {
        geode::queueInMainThread([this, itemID, catagoryName]() {
            auto spritesRes = CatStats::getCatagoryAssetSprites(catagoryName, itemID.value());
            if (spritesRes.isErr()){
                this->setContentSize({0, 0});
                return;
            }
            auto sprites = spritesRes.unwrap();

            if (sprites.primary) {
                m_primarySprite = sprites.primary;
                initSprite(m_primarySprite, "kitty-color-three-sprite-primary");

                this->setContentSize(m_primarySprite->getContentSize());

                setPrimaryColor(currentAsset.primary);
            }

            log::info("stuff");

            if (sprites.model) {
                m_model = SkeletonPlayer::create();
                m_model->loadFromGLTF(sprites.model);
                m_model->setID("kitty-color-three-sprite-primary");
                m_model->setContentSize({50, 50});
                this->addChild(m_model);

                log::info("added model");

                this->setContentSize(m_model->getContentSize());

                setPrimaryColor(currentAsset.primary);
            }

            if (sprites.secondary.has_value()){
                m_secondarySprite = sprites.secondary.value();
                initSprite(m_secondarySprite, "kitty-color-three-sprite-secondary");

                setSecondaryColor(currentAsset.secondary);
            }

            if (sprites.noncolor.has_value()){
                m_noncolorSprite = sprites.noncolor.value();
                initSprite(m_noncolorSprite, "kitty-color-three-sprite-noncolor");
            }
            
            if (onAssetUpdated != NULL)
                onAssetUpdated(this);
        });

    }).detach();
}

void CatagoryAssetDisplay::setAsset(const CatagoryAssetInfo& catagoryName){
    setPrimaryColor(catagoryName.primary);
    setSecondaryColor(catagoryName.secondary);

    setAsset(catagoryName.catagoryResoueceName, catagoryName.assetID);
}

void CatagoryAssetDisplay::setPrimaryColor(const ccColor4B& primary){
    currentAsset.primary = primary;

    CCRGBAProtocol* rgba = m_model == nullptr ? static_cast<CCRGBAProtocol*>(m_primarySprite) : static_cast<CCRGBAProtocol*>(m_model);
    if (rgba == nullptr) return;

    rgba->setColor(ccColor3B{primary.r, primary.g, primary.b});
    rgba->setOpacity(primary.a);
}

void CatagoryAssetDisplay::setSecondaryColor(const ccColor4B& secondary){
    currentAsset.secondary = secondary;
    if (m_secondarySprite  == nullptr) return;

    m_secondarySprite->setColor({secondary.r, secondary.g, secondary.b});
    m_secondarySprite->setOpacity(secondary.a);
}

void CatagoryAssetDisplay::initSprite(CCSprite* spr, const char* id) {
    if (!spr) return;
    spr->setID(id);
    spr->setAnchorPoint({0, 0});
    this->addChild(spr);
}

void CatagoryAssetDisplay::setAssetUpdatedCallback(const std::function<void(CatagoryAssetDisplay*)>& onAssetUpdated){
    this->onAssetUpdated = onAssetUpdated;
}