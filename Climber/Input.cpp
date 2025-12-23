#include "Input.h"
#include "DxLib.h"
#include "StringFunctions.h"

constexpr char config_file_name[] = "keyconf.kcnf";

namespace
{
	struct KeyConfHeader
	{
		char signature[4];	// シグネチャ4バイト
		float version;	// バージョン番号4バイト
		int dataNum;	// データ数4バイト
	};	// 12バイト
}

void Input::InitializeInputTable()
{
	inputTable_["ok"] = { { PeripheralType::keyboard,KEY_INPUT_RETURN},
					  { PeripheralType::pad1,PAD_INPUT_L } };	// PADのSELECTボタン
	inputTable_["pause"] = { { PeripheralType::keyboard,KEY_INPUT_P},
						  { PeripheralType::pad1,PAD_INPUT_R } };	// PADのSTARTボタン

	inputTable_["Jump"] = { { PeripheralType::keyboard,KEY_INPUT_Z},
						  { PeripheralType::pad1,PAD_INPUT_C } };	// PADのXボタン
	inputTable_["slow"] = { { PeripheralType::keyboard,KEY_INPUT_LSHIFT},
						  { PeripheralType::pad1,PAD_INPUT_A } };	// PADのAボタン
	inputTable_["HighJump"] = { { PeripheralType::keyboard,KEY_INPUT_X},
						  { PeripheralType::pad1,PAD_INPUT_X } };	// PADのYボタン

	inputTable_["up"] = { { PeripheralType::keyboard,KEY_INPUT_UP},
						{ PeripheralType::pad1,PAD_INPUT_UP } };
	inputTable_["down"] = { { PeripheralType::keyboard,KEY_INPUT_DOWN},
						  { PeripheralType::pad1,PAD_INPUT_DOWN } };
	inputTable_["left"] = { { PeripheralType::keyboard,KEY_INPUT_LEFT},
						  { PeripheralType::pad1,PAD_INPUT_LEFT } };
	inputTable_["right"] = { { PeripheralType::keyboard,KEY_INPUT_RIGHT},
						  { PeripheralType::pad1,PAD_INPUT_RIGHT } };
}

Input::Input() : inputData_{}, lastInputData_{}, inputTable_{}
{
	InitializeInputTable();
	Load();

	editableEventNames_ = { "ok","pause","Jump","slow","HighJump" };

	// あらかじめ枠を開けておく
	// ここで枠を開けておかないと、.at関数でクラッシュする可能性がある
	for (const auto& inputInfo : inputTable_)
	{
		inputData_[inputInfo.first] = false;
		lastInputData_[inputInfo.first] = false;
	}
}

void Input::Update()
{
	// まず現在の入力情報を取得
	char keyState[256];
	GetHitKeyStateAll(keyState);	// 生のキーボード情報
	int padState = GetJoypadInputState(DX_INPUT_PAD1);	// 生のPAD1情報
	lastInputData_ = inputData_;	// 直前のフレームを更新(更新される前のフレーム情報をコピー)
	// すべての入力イベントをチェックします
	// ここでinputData_が更新される
	// inputTable_を回して各イベントの入力をチェックする
	for (const auto& inputInfo : inputTable_)
	{
		auto& input = inputData_[inputInfo.first];	// inputInfo.firstには"ok"等が入ってる
		// inputを書き換えると、inputData_のそのイベントが押されてるかどうかを着かえることになる
		// InputStateのvectorを回す
		for (const auto& state : inputInfo.second)
		{
			// このループをInputState配列のループなので、まず入力種別をチェックします
			switch (state.type)
			{
			case PeripheralType::keyboard:	// キーボードだったら
				// GetHitKeyStateAllで取ってきた配列の中身を見て入力されているかどうかチェック
				input = keyState[state.id];
				break;
			case PeripheralType::pad1:	// PADだったら
				// GetJoypadStateで取ってきたビット情報を見て登録されているビット情報と&を取り、そのビットが立っているかどうかをチェック
				input = (padState & state.id);
				break;
			}
			if (input)	// 必須！
			{
				// ここでbreakしないと、最後のチェックで押されてないとfalseになる
				break;
			}
		}
	}
}

bool Input::IsPressed(const char* name) const
{
	// もし「ない」イベントを送られるとクラッシュします
	return inputData_.at(name);	// const関数内部なので[]ではなくatを使用してる
}

bool Input::IsTriggered(const char* name) const
{
	return inputData_.at(name) && !lastInputData_.at(name);
}

void Input::Save()
{
	// シグネチャ"kcnf"
	// バージョン番号
	// データ数
	// 先頭のデータサイズ固定部分を「ヘッダ」として書き込むために構造体を定義します
	FILE* fp = nullptr;
	auto err = fopen_s(&fp, "keyconf.kcnf", "wb");
	if (fp == nullptr)
	{
		return;
	}
	KeyConfHeader header = {};
	header.signature[0] = 'k';
	header.signature[1] = 'c';
	header.signature[2] = 'n';
	header.signature[3] = 'f';
	header.version = 1.0f;
	header.dataNum = static_cast<int>(inputTable_.size());

	fwrite(&header, sizeof(header), 1, fp);	// 12バイトまるまる書き込んでいる

	// 個別のデータ
	for (const auto& info : inputTable_)
	{
		const auto& name = info.first;
		unsigned char nameLen = static_cast<unsigned char>(name.size());	// イベント名(文字列数)
		// 文字列を書き込む際は先頭1バイトを「文字列数」にすることをおすすめします
		fwrite(&nameLen, sizeof(nameLen), 1, fp);	// 文字サイズの書き込み
		fwrite(name.data(), nameLen, 1, fp);	// 文字データの書き込み

		const auto& data = info.second;
		unsigned char dataNum = static_cast<unsigned char>(data.size());
		// データのサイズをあらかじめ書き込んでおく
		fwrite(&dataNum, sizeof(dataNum), 1, fp);
		// データをまとめて書き込む(サイズが分かっているから一気に書き込める)
		fwrite(data.data(), data.size() * sizeof(InputState), 1, fp);
	}

	fclose(fp);
}

void Input::Load()
{
	// FileRead_open は const char* を期待しているため、直接ファイル名を渡す
	int handle = FileRead_open(config_file_name);
	if (handle <= 0)
	{
		return;	// ファイルが見つからない場合は早期終了
	}

	KeyConfHeader header = {};
	FileRead_read(&header, sizeof(header), handle);
	for (int i = 0; i < header.dataNum; i++)
	{
		unsigned char nameSize = 0;	// 名前の文字列数が分からないため、1バイトロード
		FileRead_read(&nameSize, sizeof(nameSize), handle);

		std::string name;	// イベント名受け取り用
		name.resize(nameSize);	// 受け取るために名前文字列の領域を確保
		// 確保した名前領域にセーブされているイベント名をコピーします
		FileRead_read(name.data(), nameSize, handle);
		// その名前がinputTable_にあるかどうかをチェック
		if (inputTable_.contains(name))
		{
			// もしあったら、そのテーブルデータを取得します
			auto& info = inputTable_.at(name);
			unsigned char dataNum = 0;
			FileRead_read(&dataNum, sizeof(dataNum), handle);	// データ数を取得
			// 元のvectorサイズを超えないようにminで小さい方を選ぶようにする
			dataNum = min(dataNum, static_cast<unsigned char>(info.size()));	// inputTable_[name]内のvectorをオーバーしないように
			// 必要な分だけデータをファイルからリードしてコピーします
			FileRead_read(info.data(), dataNum * sizeof(InputState), handle);
		}
	}
	FileRead_close(handle);	// クローズを忘れないように
}