#include "window.h"
#include "powerutils.h"

#ifndef QT_NO_SYSTEMTRAYICON

#include <QAction>
#include <QActionGroup>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QMenu>

Window::Window(QWidget *parent) {
    createTrayIcon();
}

void Window::createMenu() {
    // clear the menu so we can rebuild it again in case anything updated
    trayIconMenu->clear();
    powerPlanActionGroup->actions().clear();

    std::vector<GUID> guids = PowerUtils::getPowerPlanGUIDs(); // get all plans
    GUID activePowerPlan = PowerUtils::getActivePowerPlan();   // get current plan

    std::vector<QAction> actions;
    QAction *action;
    QString name;

    // create menu item for each GUID
    for (GUID guid : guids) {
        // get plan name, then create action
        name = QString::fromStdString(PowerUtils::getFriendlyNameFromGUID(guid));
        action = new QAction(name, this);
        action->setCheckable(true);

        // set guid in the action data, so we can set it when we click
        action->setData(QVariant::fromValue(guid));

        if (guid == activePowerPlan) {
            action->setChecked(true);
        }

        // on action click, set the current power plan to the guid of the clicked plan name
        connect(action, &QAction::triggered, this, [guid]() { PowerUtils::setPowerPlan(guid); });

        powerPlanActionGroup->addAction(action);
        trayIconMenu->addAction(action);
    }

    // add exit button
    trayIconMenu->addSeparator();
    trayIconMenu->addAction("Exit", qApp, &QCoreApplication::quit);
}

// TODO: eventually id want diff icons for diff power schemes...
QIcon Window::getIcon() {
    if (taskbarIsLight()) {
        // light taskbar needs dark icon
        return QIcon(":/icons/default");
    }

    // otherwise just return the light icon (for dark taskbars)
    return QIcon(":/icons/default_dark");
}

void Window::createTrayIcon() {
    trayIconMenu = new QMenu(this);
    powerPlanActionGroup = new QActionGroup(this);
    powerPlanActionGroup->setExclusive(true);

    // recreate the menu each time we show the menu (since it could change)
    connect(trayIconMenu, &QMenu::aboutToShow, this, &Window::createMenu);

    // check taskbar color first, then decide to invert the icon color
    QIcon icon = getIcon();

    trayIcon = new QSystemTrayIcon(icon, this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();
}

// detect if taskbar is light/dark from registry (true: light, false: dark)
bool Window::taskbarIsLight() {
    HKEY hkey;
    LONG lResult
        = RegOpenKeyEx(HKEY_CURRENT_USER,
                       L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                       0,
                       KEY_READ,
                       &hkey);

    if (lResult != ERROR_SUCCESS) {
        return true;
    } // default to true (light mode) just in case

    DWORD dwValue;
    DWORD dwSize = sizeof(dwValue);
    lResult = RegGetValue(hkey, NULL, L"SystemUsesLightTheme", RRF_RT_DWORD, NULL, &dwValue, &dwSize);

    RegCloseKey(hkey);

    if (lResult != ERROR_SUCCESS) {
        return true;
    }

    return (dwValue == 1);
}

Window::~Window() {}

#endif
