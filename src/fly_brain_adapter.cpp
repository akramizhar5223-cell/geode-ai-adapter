#include "fly_brain_adapter.hpp"

FlyBrainAIAdapter* FlyBrainAIAdapter::get() {
    static FlyBrainAIAdapter instance;
    return &instance;
}

void FlyBrainAIAdapter::initializeFlyBrain() {
    log::info("Initializing Fly Brain AI for Geometry Dash");
    m_flyBrain = std::make_unique<FlyBrainNeuralNet>();
    AISystem::init();
}

void FlyBrainAIAdapter::processGameState(
    float playerX,
    float playerY,
    float playerVelocityY,
    float nextObstacleX,
    float nextObstacleY,
    float nextObstacleSize,
    float gameSpeed
) {
    if (!isEnabled() || !m_flyBrain) return;
    
    // Calculate approach speed (how fast obstacle is coming at player)
    float approachSpeed = gameSpeed * 100.0f; // Normalized to game units
    
    // Feed visual stimulus to fly brain
    m_flyBrain->feedVisualInput(
        nextObstacleX,
        nextObstacleY,
        nextObstacleSize,
        approachSpeed,
        playerY
    );
    
    // Simulate 10ms of neural activity (fly brain operates on ~10ms timescale)
    m_flyBrain->updateNeuralActivity(10.0f);
    
    // Record game state
    recordGameEvent(GameEvent::OBSTACLE_DETECTED, nextObstacleX, nextObstacleY, gameSpeed);
    updatePlayerPattern(playerX, playerY, playerVelocityY > 0.0f, playerVelocityY);
    
    // Store threat level for analysis
    m_currentThreatLevel = m_flyBrain->getPredictedJumpTiming() / 100.0f;
}

float FlyBrainAIAdapter::getFlyBrainJumpTiming() {
    if (!m_flyBrain) return 0.0f;
    return m_flyBrain->getPredictedJumpTiming();
}

float FlyBrainAIAdapter::getFlyBrainJumpIntensity() {
    if (!m_flyBrain) return 0.0f;
    return m_flyBrain->getMotorOutput();
}

float FlyBrainAIAdapter::getReactionTimeMs() {
    if (!m_flyBrain) return 1000.0f;
    return m_flyBrain->getReactionTime();
}

float FlyBrainAIAdapter::getThreatLevel() {
    return m_currentThreatLevel;
}

std::vector<float> FlyBrainAIAdapter::getVisualNeuralMap() {
    if (!m_flyBrain) return {};
    return m_flyBrain->getNeuralActivationMap();
}
