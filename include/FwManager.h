#pragma once
#pragma once

#include <windows.h>
#include <netfw.h>
#include <string>
#include <comutil.h>
#include <iostream>
#include <fstream>

class FirewallManager {
public:
    FirewallManager();
    ~FirewallManager();

    bool addOutboundRule(const std::string& filePath, const std::string& expectedHash);
    bool deleteOutboundRule();

private:
    HRESULT initializeCOM();
    HRESULT initializeFirewallPolicy(INetFwPolicy2** ppNetFwPolicy2);
    BSTR loadEndpoints(const wchar_t* filePath);

    INetFwPolicy2* m_policy;

    static constexpr const wchar_t* RULE_NAME = L"Diagnostic block rule.";
    static constexpr const wchar_t* RULE_DESC = L"Recopilation about telemetry, data, and diagnostic endpoints.";
};
