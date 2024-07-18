#pragma once

#include <QDialog>
#include "ui_SignInDialog.h"
#include "User.h"

class SignInDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SignInDialog(QWidget *parent = nullptr);
	virtual ~SignInDialog();

	void LoadUserNames();


private:
	Ui::SignInDialogClass ui;
	QMap<QString, User*> userMap;
	User* loggedInUser;

public slots:
	void pushButton_Login_clicked();
	void pushButton_Cancel_clicked();
};
