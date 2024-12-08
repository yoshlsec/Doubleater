#include <netfw.h>
#include <stdexcept>

void FirewallManager::showFirewallSettings() {
    if (!m_policy) {
        throw std::runtime_error("Firewall policy not initialized.");
    }

    showProfileSettings(NET_FW_PROFILE2_DOMAIN, m_policy);
    showProfileSettings(NET_FW_PROFILE2_PRIVATE, m_policy);
    showProfileSettings(NET_FW_PROFILE2_PUBLIC, m_policy);
}
void FirewallManager::showProfileSettings(NET_FW_PROFILE_TYPE2 profileType, INetFwPolicy2* policy) {
    if (!policy) return;

    VARIANT_BOOL isEnabled = FALSE;
    NET_FW_ACTION action;

    std::wcout << L"******************************************" << std::endl;

    if (SUCCEEDED(policy->get_FirewallEnabled(profileType, &isEnabled))) {
        std::wcout << L"Firewall is " << (isEnabled ? L"enabled" : L"disabled") << std::endl;
    }

    if (SUCCEEDED(policy->get_BlockAllInboundTraffic(profileType, &isEnabled))) {
        std::wcout << L"Block all inbound traffic is " << (isEnabled ? L"enabled" : L"disabled") << std::endl;
    }

    if (SUCCEEDED(policy->get_DefaultInboundAction(profileType, &action))) {
        std::wcout << L"Default inbound action is " << (action != NET_FW_ACTION_BLOCK ? L"Allow" : L"Block") << std::endl;
    }

    if (SUCCEEDED(policy->get_DefaultOutboundAction(profileType, &action))) {
        std::wcout << L"Default outbound action is " << (action != NET_FW_ACTION_BLOCK ? L"Allow" : L"Block") << std::endl;
    }
}
