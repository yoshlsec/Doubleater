#include "FwManager.h"
#include <stdexcept>
#include "FuncHelper.h"

FirewallManager::FirewallManager() : m_policy(nullptr) {
    HRESULT hr = initializeCOM();
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to initialize COM.");
    }

    hr = initializeFirewallPolicy(&m_policy);
    if (FAILED(hr)) {
        CoUninitialize(); // Asegurar limpieza en caso de error
        throw std::runtime_error("Failed to initialize Firewall Policy.");
    }
}
FirewallManager::~FirewallManager() {
    if (m_policy) {
        m_policy->Release();
    }
    CoUninitialize();
}
HRESULT FirewallManager::initializeCOM() {
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (hr != RPC_E_CHANGED_MODE && FAILED(hr)) {
        return hr;
    }
    return S_OK;
}
HRESULT FirewallManager::initializeFirewallPolicy(INetFwPolicy2** ppNetFwPolicy2) {
    if (!ppNetFwPolicy2) {
        return E_POINTER;
    }

    return CoCreateInstance(
        __uuidof(NetFwPolicy2),
        nullptr,
        CLSCTX_INPROC_SERVER,
        __uuidof(INetFwPolicy2),
        reinterpret_cast<void**>(ppNetFwPolicy2)
    );
}
bool FirewallManager::addOutboundRule(const std::string& filePath, const std::string& expectedHash) {
    if (!m_policy) {
        throw std::runtime_error("[-] Firewall policy not initialized.\n");
    }

    try {
        if (!SHA256Helper::checkHASH(SHA256Helper::reader(filePath), expectedHash)) {
            throw std::runtime_error("Hash mismatch.\n");
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return false;
    }

    std::wstring wFilePath = SHA256Helper::changer(filePath);
    BSTR bstrRemoteAddresses = loadEndpoints(wFilePath.c_str());
    if (!bstrRemoteAddresses) {
        return false;
    }

    INetFwRules* rules = nullptr;
    HRESULT hr = m_policy->get_Rules(&rules);
    if (FAILED(hr)) {
        SysFreeString(bstrRemoteAddresses);
        return false;
    }

    INetFwRule* rule = nullptr;
    hr = CoCreateInstance(
        __uuidof(NetFwRule),
        nullptr,
        CLSCTX_INPROC_SERVER,
        __uuidof(INetFwRule),
        reinterpret_cast<void**>(&rule)
    );

    if (SUCCEEDED(hr)) {
        rule->put_Name(SysAllocString(RULE_NAME));
        rule->put_Description(SysAllocString(RULE_DESC));
        rule->put_Direction(NET_FW_RULE_DIR_OUT);
        rule->put_Action(NET_FW_ACTION_BLOCK);
        rule->put_Profiles(NET_FW_PROFILE2_ALL);
        rule->put_RemoteAddresses(bstrRemoteAddresses);
        rule->put_Enabled(VARIANT_TRUE);

        rules->Add(rule);
    }

    if (rule) {
        rule->Release();
    }
    if (rules) {
        rules->Release();
    }

    SysFreeString(bstrRemoteAddresses);
    return SUCCEEDED(hr);
}
bool FirewallManager::deleteOutboundRule() {
    if (!m_policy) {
        throw std::runtime_error("Firewall policy not initialized.");
    }

    INetFwRules* rules = nullptr;
    HRESULT hr = m_policy->get_Rules(&rules);
    if (FAILED(hr)) {
        return false;
    }

    hr = rules->Remove(SysAllocString(RULE_NAME));
    if (rules) {
        rules->Release();
    }
    return SUCCEEDED(hr);
}
BSTR FirewallManager::loadEndpoints(const wchar_t* filePath) {
    std::wifstream file(filePath);
    if (!file.is_open()) {
        std::wcerr << L"Error opening file" << std::endl;
        return nullptr;
    }

    std::wstring endpoints, line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            line.erase(0, line.find_first_not_of(L" \t"));
            line.erase(line.find_last_not_of(L" \t") + 1);
            endpoints += line + L",";
        }
    }

    if (!endpoints.empty()) {
        endpoints.pop_back();
    }
    return SysAllocString(endpoints.c_str());
}
