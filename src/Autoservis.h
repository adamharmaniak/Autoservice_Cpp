#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Autoservis.h"
#include "User.h"

class Autoservis : public QMainWindow
{
    Q_OBJECT

public:
    Autoservis(QWidget *parent = nullptr);
    ~Autoservis();

    //---------<< Method declarations >>----------

    //-------< Universal methods >-------
    void SetUpButtons(User* user);
    void LoadServices();
    void SetUpUser(User* user);
    void SetUserLabel(User* user);

    //-------< Client/Company methods >-------
    void LoadCarRecord(User* user);
    void updateModelsForBrand(const QString& brand);

    //-------< Employee methods >-------
    void LoadUserOrders(User* user);
    void LoadListOfUsers();
    void LoadUsersToComboBox();
//------------------------------------------------

private slots:
    void ReturnToHomePage();
    //--------< Menu Buttons >---------
    void pushButton_EditCarRecord_clicked();
    void pushButton_BookService_clicked();
    void pushButton_ViewOrders_clicked();
    void pushButton_EditUsersOrders_clicked();
    void pushButton_LogOut_clicked();
    void pushButton_EditUsers_clicked();
    //---------------------------------
    void pushButton_Book_clicked();
    void onBrandSelected(int index);
    void pushButton_ChangeRecord_clicked();
    void updateChangeRecordButtonState();
    void pushButton_CancelOrder_clicked();
    void pushButton_GenerateInvoice_clicked();
    void pushButton_AddUser_clicked();
    void pushButton_RemoveUser_clicked();
    void comboBox_SelectUser_IndexChanged();
    void pushButton_SaveChangesInOrders_clicked();

private:
    Ui::AutoservisClass ui;
    QMap<QString, QStringList> brandModelsMap;
    User* currentUser;
    QString directoryPath = "C:\\Pocitacova_grafika_projects\\Autoservis\\Databaza\\";
};
