#include <iostream>
#include <string>
#include <windows.h>

#include "FuncHelper.h"

using namespace std;

int DisableTelemetry(void) {
	try {
		const string filePath = "scripts\\regeditTelemetry.bat"; // CHANGE ME
		const string expectedHash = "53d81128a289134658860c0944f08bec92303f4f173fa9588559c74050fe9269"; // CHANGE ME

		if (SHA256Helper::checkHASH(SHA256Helper::reader(filePath), expectedHash)) {
			wstring FilePath = SHA256Helper::changer(filePath);
			LPCWSTR lpcwstrFilePath = FilePath.c_str();

			HINSTANCE result = ShellExecute(nullptr, L"open", lpcwstrFilePath, nullptr, nullptr, SW_SHOWNORMAL);
			if ((intptr_t)result <= 32) {
				cerr << "Failed to open file. Error code: " << (intptr_t)result << endl;
			}
		}
		wcout << "[+] Telemetry disable on your Windows 10/11" << endl;
		return 0;
	}
	catch (const exception& e) {
		cerr << "Error: " << e.what() << '\n';
		return 1;
	}
}
int EnableTelemetry(void) {
	try {
		const string filePath = "scripts\\regeditTelemetryON.bat"; // CHANGE ME
		const string expectedHash = "f6f25bc016c17a38f48b1d239d0c49cde94dd1e382c9e3add3ad69783f50f934"; // CHANGE ME

		if (SHA256Helper::checkHASH(SHA256Helper::reader(filePath), expectedHash)) {
			wstring FilePath = SHA256Helper::changer(filePath);
			LPCWSTR lpcwstrFilePath = FilePath.c_str();

			HINSTANCE result = ShellExecute(nullptr, L"open", lpcwstrFilePath, nullptr, nullptr, SW_SHOWNORMAL);
			if ((intptr_t)result <= 32) {
				cerr << "Failed to open file. Error code: " << (intptr_t)result << endl;
			}
		}
		wcout << "[!] Telemetry enable on your Windows 10/11" << endl;
		return 0;
	}
	catch (const exception& e) {
		cerr << "Error: " << e.what() << '\n';
		return 1;
	}
}
void DisableDefender(void) {
	wstring regCommands[] = {
		L"reg add \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows Defender\" /v DisableAntiSpyware /t REG_DWORD /d 1 /f",
		L"reg add \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows Defender\\Real-Time Protection\" /v DisableRealtimeMonitoring /t REG_DWORD /d 1 /f",
		L"reg add \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows Defender\\Real-Time Protection\" /v DisableBehaviorMonitoring /t REG_DWORD /d 1 /f",
		L"reg add \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows Defender\\Real-Time Protection\" /v DisableScanOnRealtimeEnable /t REG_DWORD /d 1 /f",
		L"reg add \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows Defender\\Real-Time Protection\" /v DisableIOAVProtection /t REG_DWORD /d 1 /f",
		L"reg add \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows Defender\\Signature Updates\" /v DisableUpdateOnStartupWithoutEngine /t REG_DWORD /d 1 /f",
		L"reg add \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows Defender\\Spynet\" /v SubmitSamplesConsent /t REG_DWORD /d 2 /f",
		L"reg add \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows Defender\\Spynet\" /v DisableBlockAtFirstSeen /t REG_DWORD /d 1 /f",
		L"reg add \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows Defender\\MpEngine\" /v MpEnablePus /t REG_DWORD /d 0 /f"
	};

	wstring psCommands[] = {
		L"powershell -Command \"Stop-Service -Name WinDefend -Force\"",
		L"powershell -Command \"Set-Service -Name WinDefend -StartupType Disabled\"",
		L"powershell -Command \"Get-AppxPackage -AllUsers *Microsoft.SecHealthUI* | Remove-AppxPackage\"",
		L"powershell -Command \"Get-AppxPackage -AllUsers *Microsoft.Windows.SecHealthUI* | Remove-AppxPackage\"",
		L"powershell -Command \"Get-AppxPackage -AllUsers *Microsoft.WindowsDefender* | Remove-AppxPackage\""
	};

	for (const auto& command : regCommands) {
		ExecuteCommand(command);
	} for (const auto& command : psCommands) {
		ExecuteCommand(command);
	}

	wstring gpUpdateCommand = L"gpupdate /force";
	ExecuteCommand(gpUpdateCommand);
	std::wcout << L"[+] Microsoft Defender has been fully disabled.\n";
}
void EnableDefender(void) {
	std::wstring defenderSecHealthPath = L"C:\\Windows\\SystemApps\\Microsoft.Windows.SecHealthUI_cw5n1h2txyewy\\AppxManifest.xml";
	std::wstring defenderDefenderPath = L"C:\\Windows\\SystemApps\\Microsoft.WindowsDefender_cw5n1h2txyewy\\AppxManifest.xml";

	if (!CheckFileExists(defenderSecHealthPath) || !CheckFileExists(defenderDefenderPath)) {
		std::wcerr << L"[-] Error: One or both AppxManifest.xml files for Defender are missing from the expected locations.\n";
		std::wcerr << L"[-] Attempting to reinstall Microsoft Defender...\n";

		std::wstring psCommands[] = {
			L"powershell -Command \"Add-AppxPackage -Register -DisableDevelopmentMode C:\\Windows\\SystemApps\\Microsoft.Windows.SecHealthUI_cw5n1h2txyewy\\AppxManifest.xml\"",
			L"powershell -Command \"Add-AppxPackage -Register -DisableDevelopmentMode C:\\Windows\\SystemApps\\Microsoft.WindowsDefender_cw5n1h2txyewy\\AppxManifest.xml\""
		};

		for (const auto& command : psCommands) { ExecuteCommand(command); }
		if (!CheckFileExists(defenderSecHealthPath) || !CheckFileExists(defenderDefenderPath)) {
			std::wcerr << L"[-] Error: Defender files could not be reinstalled correctly.\n"; return;
		}
	}

	std::wstring psCommands[] = {
		L"powershell -Command \"Add-AppxPackage -Register -DisableDevelopmentMode C:\\Windows\\SystemApps\\Microsoft.Windows.SecHealthUI_cw5n1h2txyewy\\AppxManifest.xml\"",
		L"powershell -Command \"Add-AppxPackage -Register -DisableDevelopmentMode C:\\Windows\\SystemApps\\Microsoft.WindowsDefender_cw5n1h2txyewy\\AppxManifest.xml\"",
		L"powershell -Command \"Set-Service -Name WinDefend -StartupType Automatic\"",
		L"powershell -Command \"Start-Service -Name WinDefend\""
	};

	std::wstring regCommands[] = {
		L"reg delete \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows Defender\" /v DisableAntiSpyware /f",
		L"reg delete \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows Defender\\Real-Time Protection\" /v DisableRealtimeMonitoring /f",
		L"reg delete \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows Defender\\Real-Time Protection\" /v DisableBehaviorMonitoring /f",
		L"reg delete \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows Defender\\Real-Time Protection\" /v DisableScanOnRealtimeEnable /f",
		L"reg delete \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows Defender\\Real-Time Protection\" /v DisableIOAVProtection /f",
		L"reg delete \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows Defender\\Signature Updates\" /v DisableUpdateOnStartupWithoutEngine /f",
		L"reg delete \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows Defender\\Spynet\" /v SubmitSamplesConsent /f",
		L"reg delete \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows Defender\\Spynet\" /v DisableBlockAtFirstSeen /f",
		L"reg delete \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows Defender\\MpEngine\" /v MpEnablePus /f"
	};

	for (const auto& command : regCommands) {
		ExecuteCommand(command);
	} for (const auto& command : psCommands) {
		ExecuteCommand(command);
	}

	std::wstring gpUpdateCommand = L"gpupdate /force";
	ExecuteCommand(gpUpdateCommand);

	std::wcout << L"[!] Microsoft Defender has been enabled and resoted.\n";
}
void DisableCortana(void) {
	ExecuteCommand(L"reg add \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows\\Windows Search\" /v AllowCortana /t REG_DWORD /d 0 /f && Get-AppxPackage -allusers Microsoft.549981C3F5F10 | Remove-AppxPackage && gpupdate /force");
	std::wcout << L"[+] Microsoft Cortana has been fully disabled and uninstalled." << std::endl;
}
void EnableCortana(void) {
	const std::wstring filePathCortana = L"C:\\Program Files\\WindowsApps\\Microsoft.Windows.Cortana_1.0.0.0_x64__8wekyb3d8bbwe\\AppxManifest.xml";

	if (!CheckFileExists(filePathCortana)) {
		std::wcerr << L"[-] Error: One or both AppxManifest.xml files for Cortana are missing from the expected locations.\n";
		std::wcerr << L"[-] Please ensure the files have not been deleted or moved.\n";

		std::wcout << L"[...] Reinstalling Cortana...\n";
		ExecuteCommand(L"Get-AppxPackage -allusers Microsoft.549981C3F5F10 | Add-AppxPackage");
		if (!CheckFileExists(filePathCortana)) {
			std::wcerr << L"[-] Error: Cortana could not be reinstalled correctly. The file is still missing.\n";
			return;
		}
	}

	ExecuteCommand(L"reg add \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows\\Windows Search\" /v AllowCortana /t REG_DWORD /d 1 /f && Get-AppxPackage -allusers Microsoft.549981C3F5F10 | Add-AppxPackage && gpupdate /force");
	std::wcout << L"[!] Microsoft Cortana has been enabled.\n";
}
