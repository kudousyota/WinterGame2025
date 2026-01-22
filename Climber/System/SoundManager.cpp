#include "SoundManager.h"
#include "DxLib.h"
std::unordered_map<std::string, int> SoundManager::seList;
std::unordered_map<std::string, int> SoundManager::bgmList;

int SoundManager::currentBgm = -1;

void SoundManager::Load()
{
	//Œø‰Ê‰¹‚Ì“Ç‚İ‚İ
	seList["Kill"]		= LoadSoundMem("Sound/•½è‘Å‚¿.mp3");
	seList["Jump"]		= LoadSoundMem("Sound/Jump.wav");
	seList["HighJump"]  = LoadSoundMem("Sound/highJump.wav");
	seList["run"]		= LoadSoundMem("Sound/run.mp3");
	seList["Admission"] = LoadSoundMem("Sound/Admission.wav");
	seList["TileBreak"] = LoadSoundMem("Sound/hitTile.mp3");


	//BGM‚Ì“Ç‚İ‚İ
	bgmList["Stage1"] = LoadSoundMem("Sound/stageBGM.ogg");
	bgmList["Title"] = LoadSoundMem("Sound/titleBGM.ogg");


}

void SoundManager::PlaySE(const std::string& key)
{
	auto it = seList.find(key);
	if (it != seList.end())
	{
		ChangeVolumeSoundMem(120, it->second);
		PlaySoundMem(it->second, DX_PLAYTYPE_BACK);
	}
}

void SoundManager::PlayBGM(const std::string& key, bool loop)
{
	//BGMÄ¶ˆ—
	auto it = bgmList.find(key);
	if (it == bgmList.end())
	{
		return;
	}
	if (currentBgm != -1)
	{
		StopSoundMem(currentBgm);
	}
	currentBgm = it->second;
	if (currentBgm >= 0)
	{
		//‰¹—Ê’²®
		ChangeVolumeSoundMem(50, currentBgm);
		PlaySoundMem(currentBgm, loop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK);
	}
}

void SoundManager::StopBGM()
{
	if (currentBgm !=  -1)
	{
		StopSoundMem(currentBgm);
		currentBgm = -1;
	}
}

void SoundManager::Update()
{
	//BGMXVˆ—

}