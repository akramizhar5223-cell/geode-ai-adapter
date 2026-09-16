#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(ModPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level) {
        if (!PlayLayer::init(level)) return false;
        return true;
    }

    void update(float dt) {
        PlayLayer::update(dt);
        
        // Simple test: auto-jump when close to something
        if (m_isPaused || m_gameState != 0) return;
        
        float px = m_player1->getPositionX();
        float py = m_player1->getPositionY();
        float nearestDist = 999.0f;
        float nearestDy = 0.0f;

        for (auto* obj : m_gameObjects) {
            if (!obj || !obj->isVisible()) continue;
            float dx = obj->getPositionX() - px;
            if (dx > 0.5f && dx < nearestDist) {
                nearestDist = dx;
                nearestDy = obj->getPositionY() - py;
            }
        }

        // Jump if obstacle ahead and above
        if (nearestDist < 8.0f && nearestDy > 0.5f && !m_player1->m_isJumping) {
            this->handleButton(true, false);
        }
        
        // Hold if obstacle ahead and below
        if (nearestDist < 8.0f && nearestDy < -0.5f) {
            this->handleButton(true, true);
        }
    }
};
