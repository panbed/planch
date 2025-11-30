#ifndef WINDOW_H
#define WINDOW_H

#include <QSystemTrayIcon>

#ifndef QT_NO_SYSTEMTRAYICON

#include <QDialog>
#include <windows.h>

// Q_DECLARE_METATYPE(GUID);

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QCheckBox;
class QMenu;
QT_END_NAMESPACE

class Window : public QDialog {
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);

    ~Window();

private:
    void createTrayIcon();
    void createMenu();
    bool taskbarIsLight();
    QIcon getIcon();

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QActionGroup *powerPlanActionGroup;
};

#endif // QT_NO_SYSTEMTRAYICON

#endif // WINDOW_H
