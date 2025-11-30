#include "powerutils.h"

#include <codecvt>
#include <locale>
#include <powersetting.h>
#include <powrprof.h>

// get friendly name from GUID as string
std::string PowerUtils::getFriendlyNameFromGUID(GUID guid) {
    UCHAR buffer[2048];
    DWORD bufferSize = sizeof(buffer);
    DWORD result;

    // PowerReadFriendlyName should return ERROR_SUCCESS if it got the name
    result = PowerReadFriendlyName(NULL, &guid, &NO_SUBGROUP_GUID, NULL, buffer, &bufferSize);

    if (result == ERROR_SUCCESS) {
        // convert the wstring into a regular string (TODO: might use wstring later?)
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.to_bytes((wchar_t *) buffer);
    }

    return ""; // couldn't get name
}

// return array of GUIDs
std::vector<GUID> PowerUtils::getPowerPlanGUIDs() {
    std::vector<GUID> guids;

    GUID guid;
    DWORD bufferSize = sizeof(guid);
    DWORD index;

    for (index = 0;; index++) {
        // zero out the guid var (idk i just saw somewhere that i might need to)
        ZeroMemory(&guid, sizeof(guid));

        // append GUIDs to vector by iterating through PowerEnumerate
        if (PowerEnumerate(NULL, NULL, NULL, ACCESS_SCHEME, index, (UCHAR *) &guid, &bufferSize)
            == ERROR_SUCCESS) {
            guids.push_back(guid);
        }

        else {
            break;
        }
    }

    return guids;
}

// set power plan to GUID (true -> success, false -> failed)
bool PowerUtils::setPowerPlan(GUID guid) {
    DWORD result;
    result = PowerSetActiveScheme(NULL, &guid);

    if (result == ERROR_SUCCESS)
        return true;

    return false; // error setting power plan
}

GUID PowerUtils::getActivePowerPlan() {
    GUID *guidPtr;
    DWORD result;

    result = PowerGetActiveScheme(NULL, &guidPtr);

    if (result == ERROR_SUCCESS)
        return *guidPtr;

    return GUID_NULL; // couldn't get active power plan, ret null
}
