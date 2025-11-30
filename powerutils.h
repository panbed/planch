#ifndef POWERUTILS_H
#define POWERUTILS_H

#include <string>
#include <vector>
#include <windows.h>

namespace PowerUtils {
char *guid_to_str(const GUID *id, char *out);
std::string getFriendlyNameFromGUID(GUID guid);
std::vector<GUID> getPowerPlanGUIDs();
bool setPowerPlan(GUID guid);
GUID getActivePowerPlan();
} // namespace PowerUtils

#endif // POWERUTILS_H
