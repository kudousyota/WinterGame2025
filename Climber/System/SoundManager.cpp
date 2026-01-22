#include "SoundManager.h"
#include "Dxlib.h"
std::unordered_map<std::string, int> SoundManager::seList;
std::unordered_map<std::string, int> SoundManager::bgmList;

int SoundManager::currentBgm = -1;

void SoundManager::Load()
{
	seList["Kill"] = LoadSoundMem("Sound/•½è‘Å‚¿.mp3");
}