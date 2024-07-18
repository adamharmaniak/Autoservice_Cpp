#include "SignInDialog.h"
#include "Autoservis.h"

SignInDialog::SignInDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
    LoadUserNames();

    connect(ui.pushButton_login, &QPushButton::clicked, this, &SignInDialog::pushButton_Login_clicked);
    connect(ui.pushButton_cancel, &QPushButton::clicked, this, &SignInDialog::pushButton_Cancel_clicked);
}

SignInDialog::~SignInDialog() {
    qDeleteAll(userMap);
    userMap.clear();
}

//-----------------------------------------------------------
//                      FUNCTION DEFINITIONS
//-----------------------------------------------------------
void SignInDialog::LoadUserNames() {
    QFile file("C:\\Pocitacova_grafika_projects\\Autoservis\\Databaza\\zoznam_uzivatelov.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Chyba"), tr("Nemozno otvorit subor so zoznamom pouzivatelov."));
        QApplication::quit();
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        if (!fields.isEmpty()) {
            QString userName = fields[0];
            QString userType = fields[1];
            QString password = fields.last();

            ui.comboBox_user->addItem(userName);

            if (userType == "osoba") {
                userMap[userName] = new Client(userName, password, "", "", "", 0);
            }
            else if (userType == "firma") {
                QString taxId = fields[2];
                bool isVATPayer = (fields[3].toLower() == "true");
                userMap[userName] = new Company(userName, password, userName, "", "", 0, taxId, isVATPayer);
            }
            else if (userType == "administrator") {
                userMap[userName] = new Employee(userName, password, Employee::Administrator);
            }
            else if (userType == "technik") {
                userMap[userName] = new Employee(userName, password, Employee::Technician);
            }
        }
    }

    file.close();
}

//-----------------------------------------------------------
//                      SLOT DEFINITIONS
//-----------------------------------------------------------

void SignInDialog::pushButton_Login_clicked() {
    QString username = ui.comboBox_user->currentText();
    QString password = ui.lineEdit_password->text();

    if (userMap.contains(username) && userMap.value(username)->getPassword() == password) {
        User* user = userMap.value(username);
        accept();

        Autoservis* w = new Autoservis();
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->SetUpUser(user);
        w->show();
    }
    else {
        QMessageBox::warning(this, tr("Nespravne heslo"), tr("Zadali ste nespravne heslo, skuste to znova."));
    }
}

void SignInDialog::pushButton_Cancel_clicked() {
    reject();
}
