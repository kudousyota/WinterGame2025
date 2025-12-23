#pragma once
#include <array>
#include <map>
#include <string>
#include <vector>

/// <summary>
/// 周辺機器種別
/// </summary>
enum class PeripheralType
{
	keyboard,
	pad1
};

/// <summary>
/// 入力対応情報
/// </summary>
struct InputState
{
	PeripheralType type;	// 入力された機器の種類
	int id;	// 入力情報が入る(キーボードの場合はインデックス、パッドの場合はビット)
};

class KeyConfigScene;

/// <summary>
///  入力を抽象化するためのクラス
/// </summary>
class Input
{
	friend KeyConfigScene;	// キーコンフィグだけはInputTable等をいじることができる
private:
	std::map<std::string, std::vector<InputState>>inputTable_; // イベント名と実際の入力の対応表

	// 仮の入力テーブル
	// キーコンフィグが終わったら破棄される
	std::map<std::string, std::vector<InputState>>tempInputTable_;	// 一時的な入力対応テーブル

	std::map<std::string, bool> inputData_;	// 実際に入力されたかどうかのデータ
	std::map<std::string, bool> lastInputData_;	// 実際に入力されたかどうかのデータ
	std::vector<std::string> editableEventNames_;	// キーコンフィグできるイベント名配列

	void InitializeInputTable();
public:
	Input();

	/// <summary>
	/// これを毎フレーム呼び出すことで
	/// 入力情報が更新されます
	/// </summary>
	void Update();

	/// <summary>
	/// 特定のボタンが押されているか
	/// </summary>
	/// <param name="name">イベント名</param>
	/// <returns>押されている:true / 押されてない:false</returns>
	bool IsPressed(const char* name)const;

	/// <summary>
	/// 特定のボタンが今押されたか(押された瞬間にしか反応しない)
	/// </summary>
	/// <param name="name">イベント名</param>
	/// <returns>今押された瞬間:true / 押されてないor押しっぱなし:false</returns>
	bool IsTriggered(const char* name)const;

	void Save();
	void Load();
};

