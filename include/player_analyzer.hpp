#pragma once

#include <vector>
#include "ai_system.hpp"

class PlayerAnalyzer {
public:
    struct PlayerMetrics {
        float averageReactionTime;
        float jumpAccuracy;
        float successRate;
        float speedConsistency;
        std::vector<float> recentJumpTimings;
    };
    
    PlayerAnalyzer();
    
    // Record player actions
    void recordJump(float timestamp, float height);
    void recordCollision(float timestamp);
    void recordPosition(float x, float y, float velocityY);
    
    // Analyze collected data
    PlayerMetrics analyzePerformance();
    
    // Predict next optimal action
    float predictOptimalJumpTiming();
    float predictOptimalJumpHeight();
    
    // Get player skill level (0.0 - 1.0)
    float getSkillLevel() const;
    
private:
    std::vector<float> m_jumpTimestamps;
    std::vector<float> m_jumpHeights;
    std::vector<float> m_collisionTimestamps;
    std::vector<std::pair<float, float>> m_positionHistory;
    
    float calculateAverageReactionTime();
    float calculateJumpAccuracy();
    float calculateSuccessRate();
};
