#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <windows.h>
#include <algorithm>
#include <vector>

#include "SHA256.h"
#include "FuncHelper.h"

using namespace std;

void ExecuteCommand(const wstring& command) {
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi = { 0 };

	if (CreateProcessW(nullptr, const_cast<LPWSTR>(command.c_str()), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else {
		std::wcerr << L"Error al ejecutar comando: " << command << L" (Error: " << GetLastError() << L")\n";
	}
}
bool CheckFileExists(const wstring& filePath) {
	std::wifstream file(filePath);
	return file.good();
}

bool SHA256Helper::checkHASH(const string& first, const string& second) {
	if (first.size() != second.size()) {
		return false;
	}

	volatile unsigned char r = 0;
	for (size_t i = 0; i < first.size(); ++i) {
		r |= first[i] ^ second[i];
	}
	return r == 0;
}
string SHA256Helper::sha256(const string& data) {
	SHA256 sha;
	sha.update(data);
	array<uint8_t, 32> digest = sha.digest();
	return SHA256::toString(digest);
}
string SHA256Helper::reader(const string& filepath) {
	ifstream file(filepath, ios::binary);
	if (!file.is_open()) {
		throw runtime_error("File " + filepath + " not found");
	}

	ostringstream buffer;
	buffer << file.rdbuf();
	return SHA256Helper::sha256(buffer.str());
}
wstring SHA256Helper::changer(const string& path) { string modPath = path; replace(modPath.begin(), modPath.end(), '\\', '/'); return wstring(modPath.begin(), modPath.end()); }