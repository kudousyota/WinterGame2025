#include "StringFunctions.h"
#include "DxLib.h"

std::wstring StringFunctions::WStringFromString(const std::string& str)
{
    std::wstring ret;
    int size = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), nullptr, 0);
    ret.resize(size);
    size = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), ret.data(), ret.size());
    return ret;
}
