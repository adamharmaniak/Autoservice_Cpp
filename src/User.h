#pragma once

#include <QStandardItemModel>
#include <QFile>
#include <QString>
#include <QMenu>
#include <QAction>
#include <QStringList>
#include <QTextStream>
#include <QMessageBox>
#include <QMap>
#include <QFileDialog>

class User {
protected:
    QString username;
    QString password;

public:
    User(const QString& username, const QString& password) : username(username), password(password) {}
    virtual ~User() {}

    QString getUsername() const { return username; }
    void setUsername(const QString& username) { this->username = username; }

    QString getPassword() const { return password; }
    void setPassword(const QString& password) { this->password = password; }
};

class Client : public User {
protected:
    QString carBrand;
    QString carModel;
    int yearOfManufacture;

public:
    Client(const QString& username, const QString& password, const QString& fullName,
        const QString& brand, const QString& model, int year)
        : User(username, password), carBrand(brand), carModel(model), yearOfManufacture(year) {}
    virtual ~Client() {}
};

class Company : public Client {
private:
    QString taxId;
    bool isVATPayer;

public:
    Company(const QString& username, const QString& password, const QString& companyName,
        const QString& brand, const QString& model, int year,
        const QString& taxId, bool vat)
        : Client(username, password, companyName, brand, model, year), taxId(taxId), isVATPayer(vat) {}
    ~Company() {}

    QString getTaxId() { return taxId; }
    bool getVATPayerInformation() { return isVATPayer; }
};

class Employee : public User {
public:
    enum Position { Technician, Administrator };

private:
    Position jobPosition;

public:
    Employee(const QString& username, const QString& password, Position position)
        : User(username, password), jobPosition(position) {}
    ~Employee() {}

    Position getPosition() const { return jobPosition; }
};