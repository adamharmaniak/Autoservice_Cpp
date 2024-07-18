#include "SignInDialog.h"
#include "Autoservis.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file("C:\\Users\\Asus ROG Strix G15\\STUBA\\Rocnik_2\\Zimny_semester\\C++\\Projekt\\Autoservis\\template.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        a.setStyleSheet(styleSheet);
    }

    SignInDialog signInDialog;
    if (signInDialog.exec() == QDialog::Accepted) {}
    else {
        return 0;
    }

    return a.exec();
}
