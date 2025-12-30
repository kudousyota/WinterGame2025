#pragma once

/// <summary>
/// 2Dベクトルクラス
/// </summary>
struct Vector2
{
	float x, y;
	float Length() const;	// ベクトルの大きさを返す
	void Normalize();	// 大きさを1にする
	Vector2 Normalized() const;	// 大きさを1にしたベクトルを返す
	Vector2 operator-() const;	// ベクトルを反転したベクトルを返す
	void operator+=(const Vector2& val);	// 加算
	void operator-=(const Vector2& val);	// 減算
	void operator*=(const float scale);	// ベクトルをn倍する
	Vector2 operator+(const Vector2& val) const;	// 加算
	Vector2 operator-(const Vector2& val) const;	// 減算
	Vector2 operator*(const float scale) const;	// ベクトルの乗算
};

using Position2 = Vector2;	// 座標もベクトルとして扱う

/// <summary>
/// 2Dのサイズを定義する構造体
/// </summary>
struct Size
{
	int w;	// 幅
	int h;	// 高さ
};

/// <summary>
/// 円を表す構造体
/// </summary>
struct Circle
{
	Position2 pos;	// 中心点
	float r;	// 半径
	void Draw();
};