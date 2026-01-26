#pragma once
#include "DxLib.h"
#include <algorithm>
class GameTimer
{
public:

    explicit GameTimer(float limitSeconds = 60.0f)
        : m_limit(limitSeconds), m_remaining(limitSeconds), m_prevMs(GetNowCount()) {
    }

    // 制限時間の再設定
    void Reset(float limitSeconds) {
        m_limit = limitSeconds;
        m_remaining = limitSeconds;
        m_elapsed = 0.0f;
        m_prevMs = GetNowCount();
        m_paused = false;
    }

    // 毎フレーム呼ぶ
    void Update() {
        if (m_paused) return;

        const int now = GetNowCount();
        const int deltaMs = now - m_prevMs;
        m_prevMs = now;

        const float deltaSec = deltaMs / 1000.0f;
        m_elapsed += deltaSec;
        m_remaining = (std::max)(0.0f, m_limit - m_elapsed);
    }

    // 状態取得
    float Remaining() const { return m_remaining; }  // 残り秒
    float Elapsed()   const { return m_elapsed; }    // 経過秒
    
    bool  IsTimeUp()  const { return m_remaining <= 0.0f; }

    // ポーズ制御
    void SetPaused(bool paused) { m_paused = paused; }
    bool IsPaused() const { return m_paused; }

private:
    float m_limit = 60.0f;   // 制限時間（秒）
    float m_remaining = 60.0f;   // 残り時間（秒）
    float m_elapsed = 0.0f;    // 経過時間（秒）
    int   m_prevMs = 0;       // 前フレーム時刻（ミリ秒）
    bool  m_paused = false;   // ポーズ中は進めない
};


