#pragma once

#ifndef SHA256_H
#define SHA256_H

#include <string>
#include <array>
#include <cstdint>
#endif

class SHA256Helper {
public:
	static bool checkHASH(const std::string& first, const std::string& second);
	static std::string sha256(const std::string& data);
	static std::string reader(const std::string& filepath);
	static std::wstring changer(const std::string& path);
};

void ExecuteCommand(const std::wstring& command);
bool CheckFileExists(const std::wstring& filePath);
