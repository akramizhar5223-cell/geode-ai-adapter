#include <geode/Geode.hpp>
#include "ai_system.hpp"
#include "pause_menu_hook.hpp"
#include "player_analyzer.hpp"
#include "learning_engine.hpp"

using namespace geode::prelude;

// Global instances
static AISystem* g_aiSystem = nullptr;
static PlayerAnalyzer* g_playerAnalyzer = nullptr;
static LearningEngine* g_learningEngine = nullptr;

class AIAdapterMod : public Mod {
public:
    static AIAdapterMod* get() {
        return static_cast<AIAdapterMod*>(Mod::get());
    }

    bool hasUnresolvedDependencies() override {
        return false;
    }
};

$on_mod(Loaded) {
    log::info("AI Adapter Mod Loaded!");
    
    // Initialize systems
    g_aiSystem = AISystem::get();
    g_aiSystem->init();
    
    g_playerAnalyzer = new PlayerAnalyzer();
    g_learningEngine = new LearningEngine(0.5f);
    
    PauseMenuHook::init();
    
    log::info("All AI systems initialized successfully");
}

// Hook into gameplay to record player actions
$modify(PlayLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!PlayLayer::init(level, p1)) return false;
        
        if (g_aiSystem && g_aiSystem->isEnabled()) {
            log::info("AI Adapter: Level started with AI enabled");
            this->scheduleUpdate();
        }
        
        return true;
    }
    
    void update(float dt) {
        PlayLayer::update(dt);
        
        if (!g_aiSystem || !g_aiSystem->isEnabled()) return;
        
        // Record player position and state
        auto player = this->m_player1;
        if (player) {
            g_playerAnalyzer->recordPosition(
                player->getPositionX(),
                player->getPositionY(),
                player->m_yVelocity
            );
            
            g_aiSystem->updatePlayerPattern(
                player->getPositionX(),
                player->getPositionY(),
                player->m_isJumping,
                player->m_yVelocity
            );
        }
    }
};

// Hook into death to record failures
$modify(PlayerObject) {
    void playerDestroyed(bool p0) {
        if (g_aiSystem && g_aiSystem->isEnabled()) {
            g_aiSystem->recordGameEvent(
                GameEvent::COLLISION,
                this->getPositionX(),
                this->getPositionY(),
                0.0f
            );
            g_playerAnalyzer->recordCollision(0.0f);
        }
        
        PlayerObject::playerDestroyed(p0);
    }
};

$on_mod(Unloaded) {
    log::info("AI Adapter Mod Unloaded!");
    
    if (g_aiSystem) {
        g_aiSystem->shutdown();
    }
    
    if (g_playerAnalyzer) {
        delete g_playerAnalyzer;
        g_playerAnalyzer = nullptr;
    }
    
    if (g_learningEngine) {
        delete g_learningEngine;
        g_learningEngine = nullptr;
    }
}
