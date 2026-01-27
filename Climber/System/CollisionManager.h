
#pragma once
#include "Rect.h"
#include <vector>
#include "Vec2.h"
#include <memory>

class Player;
class Rabbit;
class Bat;
class Stage;

class CollisionManager
{
public:
    void Init();
    void Update();

    //引数名を分かりやすく
    void Draw();

    // 全ての当たり判定をチェックする
    static int CheckCollisions(
        std::shared_ptr<Player>& m_pPlayer,
        std::vector<std::shared_ptr<Rabbit>>& m_pRabbits,
        std::vector<std::shared_ptr<Bat>>& m_pBats,
        std::shared_ptr<Stage>& m_pStage,
        int& killCount,
        CollisionManager* effectSink = nullptr
    );

    //カメラオフセットを外部から設定したい場合
    void SetCameraOffset(int camX, int camY) { m_cameraX = camX; m_cameraY = camY; }
    // 下から当たった瞬間に演出を積むAPI

    void AddTileBreakEffect(float px, float py,
        int startCol, int startRow,
        int frames,
        int frameStep = 4);

private:
    //ここだけのものにする
    // エフェクト定義

    struct EffectInstance {
        int x = 0;          // ワールド座標(px)
        int y = 0;          // ワールド座標(px)
        int startCol = 0;   // 開始コマ（列）
        int startRow = 0;   // 開始コマ（行）
        int frames = 1;     // 再生枚数
        int frameStep = 4;  // コマ送り間隔（フレーム）
        int localFrame = 0; // 経過フレーム（この演出インスタンス内）
        bool finished = false;
    };

    // 画像ハンドル
    int m_effectHandle = -1;

    // 1コマのサイズ（固定：16×16）
    static constexpr int kCutW = 16;
    static constexpr int kCutH = 16;

    // シートの総列・総行（画像から自動算出）
    int m_sheetCols = 0;
    int m_sheetRows = 0;

    // 管理中の演出
    std::vector<EffectInstance> m_tileBreakEffects;

    // カメラ（スクロール）
    int m_cameraX = 0;
    int m_cameraY = 0;

};

