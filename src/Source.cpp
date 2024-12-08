#include <iostream>
#include <string>
#include <map>
#include <functional>

#include "Options.h"
#include "FwManager.h"

using namespace std;

void helpMenu(const std::string& programNameTEMP) {
	std::string programName = programNameTEMP;
	size_t pos = programName.find_last_of("\\/");
	if (pos != std::string::npos) {
		programName = programName.substr(pos + 1);
	}

	std::cout << R"(
Usage: .\)" << programName << R"( [options]

Options:
  --help                Display this help menu.
  --telemetry           Disable system telemetry.
  --enable-telemetry    Enable system telemetry.
  --firewall            Add an outbound firewall rule to block diagnostic data from Microsoft.
  --disable-firewall    Remove the previously added firewall rule.
  --cortana             Disable Cortana.
  --enable-cortana      Enable Cortana.
  --defender            Disable Microsoft Defender.
  --enable-defender     Enable Microsoft Defender.

Examples:
  .\)" << programName << R"( --help
  .\)" << programName << R"( --firewall --defender
  .\)" << programName << R"( --disable-firewall
)" << std::endl;
}

int main(int argc, char* argv[]) {
	if (argc == 1) {
		std::string programName = argv[0];
		helpMenu(programName);
	} else {
		map<string, function<void()>> options = {
		{"--help", [&argv]() { std::string programName = argv[0]; helpMenu(programName); }},
		{"--telemetry", []() { DisableTelemetry(); }},
		{"--enable-telemetry", []() { EnableTelemetry(); }},
		{"--firewall", []() {
			const std::string filePath = "scripts\\ips.lst";
			const std::string expectedHash = "ea71dd34fd9500c8ce4cdc4299600c84afaa0d2c42cfe29e6bece63400e6670f";
			FirewallManager firewallManager;

			if (firewallManager.addOutboundRule(filePath, expectedHash)) {
				std::cout << "[+] Rule added successfully." << std::endl;
			}
			else {
				std::cerr << "[-] Error adding the rule." << std::endl;
			}
		}},
		{"--disable-firewall", []() {
			FirewallManager firewallManager;
			if (firewallManager.deleteOutboundRule()) {
				std::cout << "[!] Rule deleted successfully." << std::endl;
			} else {
				std::cerr << "[-] An error ocurred while deleting the Firewall Rule." << std::endl;
			}
		}},
		{"--cortana", []() { DisableCortana(); }},
		{"--enable-cortana", []() { EnableCortana(); }},
		{"--defender", []() { DisableDefender(); }},
		{"--enable-defender", []() { EnableDefender(); }}
		};


		for (int i = 1; i < argc; ++i) {
			string argument = argv[i];
			auto it = options.find(argument);

			if (it != options.end()) {
				it->second();
			}
			else {
				cerr << "[-] Error: The argument named " << string(argv[1]) << " was not found." << endl;
			}
		}
	}
	return 0;
}