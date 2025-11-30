#include <QApplication>

#ifndef QT_NO_SYSTEMTRAYICON

#include <QMessageBox>
#include "window.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Window window;
    return app.exec();
}

#else

#include <QDebug>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QString text("QSystemTrayIcon is not supported on this platform!");
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.setIcon(QMessageBox::Critical);

    qDebug() << text;

    return msgBox.exec();
}

#endif
