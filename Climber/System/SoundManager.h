#pragma once
#include <unordered_map>
#include <memory>
#include <string> 
class SoundManager
{
public:
	static void Load();

	static void PlaySE(const std::string& key);
	static void PlayBGM(const std::string& key, bool loop = true);
	static void StopBGM();
	static void Update();


private:

	static std::unordered_map<std::string, int> seList;
	static std::unordered_map<std::string, int> bgmList;

	static int currentBgm;

};

