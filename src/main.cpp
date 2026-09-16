#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(AITestMod, PlayLayer) {
    void update(float dt) {
        PlayLayer::update(dt);
        if (m_isPaused || m_gameState != 0) return;
        
        float px = m_player1->getPositionX();
        float py = m_player1->getPositionY();
        float nearDist = 999.0f;
        float nearDy = 0.0f;

        for (auto* obj : m_gameObjects) {
            if (!obj || !obj->isVisible()) continue;
            float dx = obj->getPositionX() - px;
            if (dx > 0.5f && dx < nearDist) {
                nearDist = dx;
                nearDy = obj->getPositionY() - py;
            }
        }

        if (nearDist < 7.0f && nearDy > 0.4f && !m_player1->m_isJumping) {
            this->handleButton(true, false);
        }
    }
};
