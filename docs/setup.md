# Small guide for developers

In this same folder, you will find: `DevFuncs.cpp`, inside this file are two functions related to the Windows Firewall API, using the library `<netfw.h>`: [More info here](https://learn.microsoft.com/en-us/windows/win32/api/netfw/)

Anyways, if you are going to implement these functions consider naming them in the header file: *FwManager.h*

```cpp
class FirewallManager {
public:
    FirewallManager();
    ~FirewallManager();
    showFirewallSettings()
    bool addOutboundRule(const std::string& filePath, const std::string& expectedHash);
    bool deleteOutboundRule();

private:
    HRESULT initializeCOM();
    HRESULT initializeFirewallPolicy(INetFwPolicy2** ppNetFwPolicy2);
    BSTR loadEndpoints(const wchar_t* filePath);

    showProfileSettings(NET_FW_PROFILE_TYPE2 profileType, INetFwPolicy2* policy);
    INetFwPolicy2* m_policy;

    static constexpr const wchar_t* RULE_NAME = L"Diagnostic block rule.";
    static constexpr const wchar_t* RULE_DESC = L"Recopilation about telemetry, data, and diagnostic endpoints.";
};
```

## Avaliable functions

### showFirewallSettings && showProfileSettings
With this function, you will be able to check if the Firewall is enabled on Windows through the function: *showProfileSettings*.

You can add an extra parameter in *Source.cpp* requesting information about the firewall rules, another option is implementing [get_FirewallEnabled](https://learn.microsoft.com/en-us/windows/win32/api/netfw/nf-netfw-inetfwprofile-get_firewallenabled) function.

To add an extra argument to the executable, you will need to create another map item. If you need to introduce a global variable into the item, replicate the --help item but use the memory address of the variable instead, here the example:

```cpp
{"--ARGUMENT", [&VARIABLE]() { FUNCTION(VARIABLE); }}
```

## Externa aclaration
We are using *SHA256* to check the hash of the file and prevent some file hijacking, but it is recommended to use blake3 hash, as it is faster and safe to use.

*Blake3 Github*
- https://github.com/BLAKE3-team/BLAKE3

*Blake3 Implementation Guide*
- https://www.voidtools.com/forum/viewtopic.php?t=15177