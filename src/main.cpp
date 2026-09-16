#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

// ==========================================
// FLY BRAIN AI — SIMPLE & COMPILABLE ✅
// ==========================================
struct SimpleAI {
    float jumpWeight = 1.0f;
    float holdWeight = 1.0f;
    int fails = 0;
    int attempts = 0;
    bool isHolding = false;

    bool shouldJump(float dist, float heightDiff) {
        if (dist > 12.0f) return false; // straight fly
        if (dist < 8.0f && heightDiff > 0.4f) {
            return true;
        }
        return false;
    }

    bool shouldHold(float dist, float heightDiff) {
        if (dist > 12.0f && std::abs(heightDiff) < 0.5f) return true;
        if (dist < 8.0f && heightDiff < -0.4f) {
            return true;
        }
        return false;
    }

    void learned(bool success) {
        attempts++;
        if (!success) {
            fails++;
            jumpWeight = std::max(0.6f, jumpWeight - 0.05f);
            holdWeight = std::max(0.6f, holdWeight - 0.03f);
        } else {
            jumpWeight = std::min(1.4f, jumpWeight + 0.02f);
            holdWeight = std::min(1.4f, holdWeight + 0.02f);
        }
    }
};

static SimpleAI s_ai;
static bool s_aiActive = true;

// ==========================================
// PAUSE LAYER — 5-SECOND RESUME HOLD ✅
// ==========================================
class $modify(ModPauseLayer, PauseLayer) {
    bool m_holding = false;
    bool m_done = false;
    CCLabelBMFont* m_label = nullptr;

    void customSetup() {
        PauseLayer::customSetup();
        auto resumeBtn = this->getChildByID("resume-button");
        if (!resumeBtn) return;

        resumeBtn->removeFromParent();
        auto newBtn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_playBtn_001.png"),
            this,
            menu_selector(ModPauseLayer::onHoldStart)
        );
        newBtn->setID("resume-button");
        newBtn->setPosition({0, -60});

        m_label = CCLabelBMFont::create("Hold 5s to unpause", "bigFont.fnt");
        m_label->setPosition({0, 40});
        m_label->setScale(0.5f);
        m_label->setVisible(false);
        this->addChild(m_label, 10);

        if (auto menu = this->getChildByType<CCMenu>(0)) {
            menu->addChild(newBtn);
        }
    }

    void onHoldStart(CCObject*) {
        m_holding = true;
        m_done = false;
        m_label->setVisible(true);
        this->schedule(schedule_selector(ModPauseLayer::onUpdate), 0.05f);
    }

    void onUpdate(float) {
        if (!m_holding) {
            this->unschedule(schedule_selector(ModPauseLayer::onUpdate));
            return;
        }
        static float elapsed = 0;
        elapsed += 0.05f;
        
        if (elapsed >= 5.0f && !m_done) {
            m_done = true;
            m_holding = false;
            elapsed = 0;
            this->unschedule(schedule_selector(ModPauseLayer::onUpdate));
            PauseLayer::onResume(nullptr);
        } else if (m_label) {
            float rem = std::max(0.0f, 5.0f - elapsed);
            m_label->setString(CCString::createWithFormat("Wait %.1fs...", rem)->getCString());
        }
    }

    void cancelHold() {
        if (m_holding && !m_done) {
            m_holding = false;
            this->unschedule(schedule_selector(ModPauseLayer::onUpdate));
            m_label->setString("Cancelled — hold 5s");
        }
    }

    void mouseUp(CCEvent* e) { cancelHold(); PauseLayer::mouseUp(e); }
    void ccTouchEnded(CCTouch* t, CCEvent* e) { cancelHold(); PauseLayer::ccTouchEnded(t, e); }
};

// ==========================================
// PLAY LAYER — AI PLAYER ✅
// ==========================================
class $modify(ModPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level) {
        if (!PlayLayer::init(level)) return false;
        s_aiActive = Mod::get()->getSettingValue<bool>("enable-ai");
        return true;
    }

    void update(float dt) {
        PlayLayer::update(dt);
        if (!s_aiActive || m_isPaused || m_gameState != 0) return;

        float px = m_player1->getPositionX();
        float py = m_player1->getPositionY();
        float nearestDist = 999.0f, nearestDy = 0.0f;

        for (auto* obj : m_gameObjects) {
            if (!obj || !obj->isVisible()) continue;
            float dx = obj->getPositionX() - px;
            if (dx > 0.5f && dx < nearestDist) {
                nearestDist = dx;
                nearestDy = obj->getPositionY() - py;
            }
        }

        if (s_ai.shouldJump(nearestDist, nearestDy) && !m_player1->m_isJumping) {
            this->handleButton(true, false);
        }

        bool wantHold = s_ai.shouldHold(nearestDist, nearestDy);
        if (wantHold != s_ai.isHolding) {
            s_ai.isHolding = wantHold;
            this->handleButton(wantHold, true);
        }
    }

    void levelComplete(GJLevelResultInfo* info) {
        s_ai.learned(true);
        PlayLayer::levelComplete(info);
    }

    void playerDeath(GameObject* p) {
        s_ai.learned(false);
        PlayLayer::playerDeath(p);
    }
};
