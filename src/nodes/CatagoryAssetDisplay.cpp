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

    if (!itemID.has_value()){
        this->setContentSize({0, 0});
        return;
    }

    this->retain();
    std::thread([this, itemID, catagoryName]() {
        geode::queueInMainThread([this, itemID, catagoryName]() {
            auto spritesRes = CatStats::getCatagoryAssetSprites(catagoryName, itemID.value());
            if (spritesRes.isErr()){
                // log::error("Failed to get sprites {}", spritesRes.unwrapErr());
                this->setContentSize({0, 0});
                this->release();
                return;
            }
            auto sprites = spritesRes.unwrap();

            currentMetadata = sprites.metadata;

            if (sprites.model){

                bool isSameModel = false;

                if (m_model) {
                    if (m_model->getModel() != sprites.model){
                        this->removeChild(m_model, true);
                        m_model = nullptr;
                    }
                    else{
                        isSameModel = true;
                    }
                }

                if (!isSameModel){
                    m_model = SkeletonPlayer::create();
                    m_model->loadFromGLTF(sprites.model);
                    m_model->setID("kitty-color-three-sprite-primary");
                    this->addChild(m_model);
                    m_model->setAnchorPoint({.5f, .5f});
                    m_model->setPosition(sprites.metadata.modelSize / 2);
                    this->setContentSize(sprites.metadata.modelSize);
                }

                for (const auto& [texName, spr] : sprites.assets)
                {
                    m_model->setTextureForMesh(texName, spr.primary->getTexture());
                }
                
                setPrimaryColor(currentAsset.primary);
            }
            else if (sprites.assets.size()){
                if (sprites.assets["tex"].primary) {
                    m_primarySprite = sprites.assets["tex"].primary;
                    initSprite(m_primarySprite, "kitty-color-three-sprite-primary");

                    this->setContentSize(m_primarySprite->getContentSize());

                    setPrimaryColor(currentAsset.primary);
                }

                if (sprites.assets["tex"].secondary.has_value()){
                    m_secondarySprite = sprites.assets["tex"].secondary.value();
                    initSprite(m_secondarySprite, "kitty-color-three-sprite-secondary");

                    setSecondaryColor(currentAsset.secondary);
                }

                if (sprites.assets["tex"].noncolor.has_value()){
                    m_noncolorSprite = sprites.assets["tex"].noncolor.value();
                    initSprite(m_noncolorSprite, "kitty-color-three-sprite-noncolor");
                }
            }
            
            
            if (onAssetUpdated != NULL)
                onAssetUpdated(this);

            this->release();
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

void CatagoryAssetDisplay::setCategory(const std::string& catagoryName){
    currentAsset.catagoryResoueceName = catagoryName;
}

std::optional<CosmeticBoneOffset> CatagoryAssetDisplay::isBoneChild(const std::string& categoryName){
    if (currentMetadata.boneOffsets.contains(categoryName)){
        return currentMetadata.boneOffsets[categoryName];
    }
    return std::nullopt;
}

CCNode* CatagoryAssetDisplay::getSkeletonBone(const std::string& boneName){
    if (!m_model) return nullptr;

    auto bones = m_model->getBoneNodes();
    for (const auto& bone : bones)
    {
        if (bone->getID() != boneName) continue;

        return bone;
    }
    
    return nullptr;
}

void CatagoryAssetDisplay::applyOffset(const CosmeticOffset& off){
    this->setPosition(this->getPosition() + off.posOffset);
    this->setRotationX(this->getRotationX() + off.rotationOffset.x);
    this->setRotationY(this->getRotationY() + off.rotationOffset.y);
    this->setScaleX(this->getScaleX() + off.scaleOffset.x);
    this->setScaleY(this->getScaleY() + off.scaleOffset.y);
}