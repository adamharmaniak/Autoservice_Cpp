#include "Autoservis.h"
#include "SignInDialog.h"

Autoservis::Autoservis(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    LoadServices();
    ReturnToHomePage();
    updateChangeRecordButtonState();

    ui.comboBox_Models->setEnabled(false);
    
    connect(ui.pushButton_EditCarRecord, &QPushButton::clicked, this, &Autoservis::pushButton_EditCarRecord_clicked);
    connect(ui.pushButton_BookService, &QPushButton::clicked, this, &Autoservis::pushButton_BookService_clicked);
    connect(ui.pushButton_ViewOrders, &QPushButton::clicked, this, &Autoservis::pushButton_ViewOrders_clicked);
    connect(ui.toolButton_car_2, &QToolButton::clicked, this, &Autoservis::ReturnToHomePage);
    connect(ui.toolButton_plier, &QToolButton::clicked, this, &Autoservis::ReturnToHomePage);
    connect(ui.toolButton_plier2, &QToolButton::clicked, this, &Autoservis::ReturnToHomePage);
    connect(ui.pushButton_Book, &QToolButton::clicked, this, &Autoservis::pushButton_Book_clicked);
    connect(ui.comboBox_Brands, &QComboBox::currentIndexChanged, this, &Autoservis::onBrandSelected);
    connect(ui.pushButton_ChangeRecord, &QPushButton::clicked, this, &Autoservis::pushButton_ChangeRecord_clicked);
    connect(ui.comboBox_Brands, &QComboBox::currentIndexChanged, this, &Autoservis::updateChangeRecordButtonState);
    connect(ui.comboBox_Models, &QComboBox::currentIndexChanged, this, &Autoservis::updateChangeRecordButtonState);
    connect(ui.comboBox_Years, &QComboBox::currentIndexChanged, this, &Autoservis::updateChangeRecordButtonState);
    connect(ui.pushButton_CancelOrder, &QPushButton::clicked, this, &Autoservis::pushButton_CancelOrder_clicked);
    connect(ui.pushButton_GenerateInvoice, &QPushButton::clicked, this, &Autoservis::pushButton_GenerateInvoice_clicked);
    connect(ui.pushButton_LogOut, &QPushButton::clicked, this, &Autoservis::pushButton_LogOut_clicked);
    connect(ui.pushButton_EditUsers, &QPushButton::clicked, this, &Autoservis::pushButton_EditUsers_clicked);
    connect(ui.pushButton_AddUser, &QPushButton::clicked, this, &Autoservis::pushButton_AddUser_clicked);
    connect(ui.pushButton_RemoveUser, &QPushButton::clicked, this, &Autoservis::pushButton_RemoveUser_clicked);
    connect(ui.pushButton_EditUsersOrders, &QPushButton::clicked, this, &Autoservis::pushButton_EditUsersOrders_clicked);
    connect(ui.comboBox_SelectUser, &QComboBox::currentIndexChanged, this, &Autoservis::comboBox_SelectUser_IndexChanged);
    connect(ui.pushButton_SaveChangesInOrders, &QPushButton::clicked, this, &Autoservis::pushButton_SaveChangesInOrders_clicked);

}

Autoservis::~Autoservis()
{}

//------------------------------------------------------------------
//                  <<< METHOD DEFINITIONS >>>
//------------------------------------------------------------------

void Autoservis::SetUpUser(User* user) {
    currentUser = user;

    if (Client* client = dynamic_cast<Client*>(user)) {
        SetUpButtons(user);
        SetUserLabel(user);
        LoadCarRecord(user);
        LoadUserOrders(user);
    }
    else if (Company* company = dynamic_cast<Company*>(user)) {
        SetUpButtons(user);
        SetUserLabel(user);
        LoadCarRecord(user);
        LoadUserOrders(user);
    }
    else if(Employee* employee = dynamic_cast<Employee*>(user)) {
        SetUpButtons(user);
        SetUserLabel(user);
        LoadUsersToComboBox();
        if (employee->getPosition() == Employee::Administrator) {
            LoadListOfUsers();
        }
    }
}

void Autoservis::SetUpButtons(User* user) {


    if (Employee* employee = dynamic_cast<Employee*>(user)) {
        ui.pushButton_EditCarRecord->setEnabled(false);
        ui.pushButton_BookService->setEnabled(false);
        ui.pushButton_ViewOrders->setEnabled(false);
        ui.pushButton_EditUsers->setEnabled(false);
        ui.pushButton_EditUsersOrders->setEnabled(true);

        if (employee->getPosition() == Employee::Administrator) {
            ui.pushButton_EditUsers->setEnabled(true);
        }
    }
    else if(Client* client = dynamic_cast<Client*>(user)) {
        ui.pushButton_EditCarRecord->setEnabled(true);
        ui.pushButton_BookService->setEnabled(true);
        ui.pushButton_ViewOrders->setEnabled(true);
        ui.pushButton_EditUsers->setEnabled(false);
        ui.pushButton_EditUsersOrders->setEnabled(false);
    }
}

void Autoservis::SetUserLabel(User* user) {
    QString labelContent;
    if (Company* company = dynamic_cast<Company*>(user)) {
        labelContent = QString("<html><head/><body><p>Klient:</p><p>%1</p><p>ICO: %2</p></body></html>")
            .arg(company->getUsername())
            .arg(company->getTaxId());
    }
    else if(Client* client = dynamic_cast<Client*>(user)) {
        labelContent = QString("<html><head/><body><p>Klient:</p><p>%1</p></body></html>")
            .arg(client->getUsername());
    }
    else if (Employee* employee = dynamic_cast<Employee*>(user)) {
        if (employee->getPosition() == Employee::Administrator) {
            labelContent = QString("<html><head/><body><p>Administrátor:</p><p>%1</p></body></html>")
                .arg(employee->getUsername());
        }
        else {
            labelContent = QString("<html><head/><body><p>Technik:</p><p>%1</p></body></html>")
                .arg(employee->getUsername());
        }
    }

    ui.label_UserInfo->setText(labelContent);
}

void Autoservis::LoadListOfUsers() {
    QString filePath = directoryPath + "zoznam_uzivatelov.txt";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Nemozno otvorit subor:" << file.errorString();
        return;
    }

    QTextStream in(&file);

    QStringList headers = { "Meno pouzivatela", "Typ pouzivatela", "ICO firmy", "Plati DPH", "Heslo" };
    QStandardItemModel* model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels(headers);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");

        QList<QStandardItem*> items;
        items.append(new QStandardItem(fields[0]));
        items.append(new QStandardItem(fields[1]));

        if (fields[1] == "firma") {
            items.append(new QStandardItem(fields[2]));
            items.append(new QStandardItem(fields[3] == "true" ? "Ano" : "Nie"));
            items.append(new QStandardItem(fields[4]));
        }
        else {
            items.append(new QStandardItem(""));
            items.append(new QStandardItem(""));
            items.append(new QStandardItem(fields[2]));
        }

        if (fields[1] == "technik" || fields[1] == "administrator") {
            for (auto& item : items) {
                item->setEditable(false);
            }
        }

        model->appendRow(items);
    }

    file.close();

    ui.tableView_Users->setModel(model);
    ui.tableView_Users->resizeColumnsToContents();
}

void Autoservis::LoadUsersToComboBox() {
    QString usersFileName = directoryPath + "zoznam_uzivatelov.txt";
    QFile file(usersFileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Nemozno otvorit subor s menami pouzivatelov:" << file.errorString();
        return;
    }

    QTextStream in(&file);
    ui.comboBox_SelectUser->clear();

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        if (!fields.isEmpty()) {
            QString userName = fields.first();
            ui.comboBox_SelectUser->addItem(userName);
        }
    }

    file.close();
}

void Autoservis::LoadServices() {
    QFile file(directoryPath + "sluzby.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return;
    }

    QStandardItemModel* model = new QStandardItemModel(0, 3, this);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nazov"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Cena za sluzbu s DPH (EUR)"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Cena za sluzbu bez DPH (EUR)"));

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");

        if (fields.size() >= 2) {
            QStandardItem* itemName = new QStandardItem(fields[0].trimmed());
            QStandardItem* itemPriceWithVAT = new QStandardItem(fields[1].trimmed());
            QStandardItem* itemPriceWithoutVAT = new QStandardItem(fields[2].trimmed());

            bool ok1;
            bool ok2;
            double priceVAT = itemPriceWithVAT->text().toDouble(&ok1);
            double priceNoVAT = itemPriceWithoutVAT->text().toDouble(&ok2);
            if (ok1 && ok2) {
                itemPriceWithVAT->setText(QString::number(priceVAT, 'f', 2));
                itemPriceWithoutVAT->setText(QString::number(priceNoVAT, 'f', 2));
            }

            model->appendRow({ itemName, itemPriceWithVAT, itemPriceWithoutVAT });
        }
    }

    file.close();

    for (int row = 0; row < model->rowCount(); row++) {
        QStandardItem* priceWithVATItem = model->item(row, 1);
        QStandardItem* priceWithoutVATItem = model->item(row, 2);
        if (priceWithVATItem && priceWithoutVATItem) {
            priceWithVATItem->setTextAlignment(Qt::AlignCenter);
            priceWithoutVATItem->setTextAlignment(Qt::AlignCenter);
        }
    }

    ui.tableView_Services->setModel(model);
    ui.tableView_Services->resizeColumnsToContents();
}

void Autoservis::LoadCarRecord(User* user) {
    QFile file(directoryPath + "vozidla_zaznam.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Nemozno otvorit subor:" << file.errorString();
        return;
    }

    QStandardItemModel* model = new QStandardItemModel(3, 1, this);
    ui.tableView_Services->verticalHeader()->setVisible(true);
    ui.tableView_CarRecord->horizontalHeader()->setVisible(false);

    QStringList verticalHeaders;
    verticalHeaders << "Znacka vozidla" << "Model vozidla" << "Rok vyroby";
    model->setVerticalHeaderLabels(verticalHeaders);

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");

        if (fields.size() >= 4 && fields[0] == user->getUsername()) {
            model->setItem(0, 0, new QStandardItem(fields[1]));
            model->setItem(1, 0, new QStandardItem(fields[2]));
            model->setItem(2, 0, new QStandardItem(fields[3]));
            break;
        }
    }

    file.close();
    ui.tableView_CarRecord->setModel(model);
    ui.tableView_CarRecord->resizeColumnsToContents();
}

void Autoservis::updateModelsForBrand(const QString& brand) {
    QFile file(directoryPath + "znacky_modely_vozidla.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Nemozno otvorit subor:" << file.errorString();
        return;
    }

    QTextStream in(&file);
    ui.comboBox_Models->clear();
    bool brandFound = false;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList carData = line.split(',');
        if (carData.first().compare(brand, Qt::CaseInsensitive) == 0) {
            carData.removeFirst();
            ui.comboBox_Models->addItems(carData);
            brandFound = true;
            break;
        }
    }

    file.close();

    if (!brandFound) {
        qDebug() << "Modely pre znacku" << brand << "neboli najdene.";
    }
}

void Autoservis::LoadUserOrders(User* user) {
    QString ordersFileName = directoryPath + "Objednavky\\" + user->getUsername() + "_objednavky.txt";
    QFile ordersFile(ordersFileName);

    if (!ordersFile.exists()) {
        qDebug() << "Subor s objednavkami pre pouzivatela neexistuje. Ide pravdepodobne o prve prihlasenie pouzivatela.";
        QStandardItemModel* ordersModel = new QStandardItemModel(0, 3, this);
        
        if (Company* company = dynamic_cast<Company*>(user)) {
            if (company->getVATPayerInformation()) {
                ordersModel->setHorizontalHeaderLabels(QStringList() << "Sluzba" << "Cena za sluzbu bez DPH(EUR)" << "Stav objednavky");
                ui.tableView_Orders->setModel(ordersModel);
                ui.tableView_CarRecord->resizeColumnsToContents();
                return;
            }
            else {
                ordersModel->setHorizontalHeaderLabels(QStringList() << "Sluzba" << "Cena za sluzbu s DPH(EUR)" << "Stav objednavky");
                ui.tableView_Orders->setModel(ordersModel);
                ui.tableView_CarRecord->resizeColumnsToContents();
                return;
            }
        }
        else {
            ordersModel->setHorizontalHeaderLabels(QStringList() << "Sluzba" << "Cena za sluzbu (EUR)" << "Stav objednavky");
            ui.tableView_Orders->setModel(ordersModel);
            ui.tableView_CarRecord->resizeColumnsToContents();
            return;
        }
    }

    if (!ordersFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Nemozno otvorit subor s objednavkami:" << ordersFile.errorString();
        return;
    }

    QTextStream in(&ordersFile);
    QStandardItemModel* ordersModel = new QStandardItemModel(this);
    if (Company* company = dynamic_cast<Company*>(user)) {
        if (company->getVATPayerInformation()) {
            ordersModel->setHorizontalHeaderLabels(QStringList() << "Sluzba" << "Cena za sluzbu bez DPH(EUR)" << "Stav objednavky");
            ui.tableView_Orders->setModel(ordersModel);
        }
        else {
            ordersModel->setHorizontalHeaderLabels(QStringList() << "Sluzba" << "Cena za sluzbu s DPH(EUR)" << "Stav objednavky");
            ui.tableView_Orders->setModel(ordersModel);
        }
    }
    else {
        ordersModel->setHorizontalHeaderLabels(QStringList() << "Sluzba" << "Cena za sluzbu (EUR)" << "Stav objednavky");
        ui.tableView_Orders->setModel(ordersModel);
    }

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        QList<QStandardItem*> items;

        for (const QString& field : fields) {
            items.append(new QStandardItem(field));
        }

        ordersModel->appendRow(items);
    }

    ordersFile.close();
    ui.tableView_Orders->resizeColumnsToContents();
}

//------------------------------------------------------------------
//                  SLOT DEFINITIONS
//------------------------------------------------------------------
void Autoservis::ReturnToHomePage() {
    int homePageIndex = 0;
    ui.stackedWidget->setCurrentIndex(homePageIndex);
}

void Autoservis::pushButton_EditCarRecord_clicked() {
    int editPageIndex = 1;
    ui.stackedWidget->setCurrentIndex(editPageIndex);
}

void Autoservis::pushButton_BookService_clicked() {
    int bookPageIndex = 2;
    ui.stackedWidget->setCurrentIndex(bookPageIndex);
}

void Autoservis::pushButton_ViewOrders_clicked() {
    int ordersViewPageIndex = 3;
    ui.stackedWidget->setCurrentIndex(ordersViewPageIndex);
}

void Autoservis::pushButton_EditUsers_clicked() {
    int editUsersPageIndex = 4;
    ui.stackedWidget->setCurrentIndex(editUsersPageIndex);
}

void Autoservis::pushButton_EditUsersOrders_clicked() {
    int editUsersOrdersPageIndex = 5;
    ui.stackedWidget->setCurrentIndex(editUsersOrdersPageIndex);
}

void Autoservis::pushButton_Book_clicked() {
    QModelIndexList selectedRows = ui.tableView_Services->selectionModel()->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, tr("Objednavka sluzby"), tr("Najprv vyberte sluzbu zo zoznamu!"));
        return;
    }

    QStandardItemModel* servicesModel = qobject_cast<QStandardItemModel*>(ui.tableView_Services->model());
    QStandardItemModel* ordersModel = qobject_cast<QStandardItemModel*>(ui.tableView_Orders->model());
    if (!ordersModel) {
        ordersModel = new QStandardItemModel(this);
        if (Company* company = dynamic_cast<Company*>(currentUser)) {
            if (company->getVATPayerInformation()) {
                ordersModel->setHorizontalHeaderLabels(QStringList() << tr("Sluzba") << tr("Cena za sluzbu bez DPH (EUR)") << tr("Stav objednavky"));
                ui.tableView_Orders->setModel(ordersModel);
                ui.tableView_Orders->resizeColumnsToContents();
            }
            else {
                ordersModel->setHorizontalHeaderLabels(QStringList() << tr("Sluzba") << tr("Cena za sluzbu s DPH (EUR)") << tr("Stav objednavky"));
                ui.tableView_Orders->setModel(ordersModel);
                ui.tableView_Orders->resizeColumnsToContents();
            }
        }
        else {
            ordersModel->setHorizontalHeaderLabels(QStringList() << tr("Sluzba") << tr("Cena za sluzbu (EUR)") << tr("Stav objednavky"));
            ui.tableView_Orders->setModel(ordersModel);
            ui.tableView_Orders->resizeColumnsToContents();
        }
    }

    for (const QModelIndex& index : selectedRows) {
        QString serviceName = servicesModel->item(index.row(), 0)->text();
        QString servicePriceWithVAT = servicesModel->item(index.row(), 1)->text();
        QString servicePriceWithoutVAT = servicesModel->item(index.row(), 2)->text();

        bool serviceExists = false;
        for (int row = 0; row < ordersModel->rowCount(); row++) {
            if (ordersModel->item(row, 0)->text() == serviceName) {
                serviceExists = true;
                break;
            }
        }

        if (serviceExists) {
            QMessageBox::warning(this, tr("Objednavka sluzby"), tr("Tato sluzba sa uz nachadza vo vasom zozname objednavok."));
        }
        else {
            QList<QStandardItem*> newRow;
            newRow << new QStandardItem(serviceName);
            if (Company* company = dynamic_cast<Company*>(currentUser)) {
                if (company->getVATPayerInformation()) {
                    newRow << new QStandardItem(servicePriceWithoutVAT);
                }
                else {
                    newRow << new QStandardItem(servicePriceWithVAT);
                }
            }
            else {
                newRow << new QStandardItem(servicePriceWithVAT);
            }
            newRow << new QStandardItem(tr("nova"));
            ordersModel->appendRow(newRow);

            QMessageBox::information(this, tr("Objednavka sluzby"), tr("Sluzba bola uspesne pridana do zoznamu objednavok."));
        }
    }

    ui.tableView_Orders->resizeColumnsToContents();
    for (int row = 0; row < ordersModel->rowCount(); row++) {
        QStandardItem* priceItem = ordersModel->item(row, 1);
        QStandardItem* statusItem = ordersModel->item(row, 2);
        if (priceItem) {
            priceItem->setTextAlignment(Qt::AlignCenter);
        }
        if (statusItem) {
            statusItem->setTextAlignment(Qt::AlignCenter);
        }
    }
}

void Autoservis::onBrandSelected(int index) {
    if (index < 0) {
        ui.comboBox_Models->clear();
        ui.comboBox_Models->setEnabled(false);
        return;
    }
    else {
        ui.comboBox_Models->setEnabled(true);
        QString selectedBrand = ui.comboBox_Brands->currentText();
        updateModelsForBrand(selectedBrand);
    }
}

void Autoservis::pushButton_ChangeRecord_clicked() {
    QString brand = ui.comboBox_Brands->currentText();
    QString model = ui.comboBox_Models->currentText();
    QString year = ui.comboBox_Years->currentText();

    QStandardItemModel* tableModel = qobject_cast<QStandardItemModel*>(ui.tableView_CarRecord->model());
    if (tableModel) {
        tableModel->setItem(0, 0, new QStandardItem(brand));
        tableModel->setItem(1, 0, new QStandardItem(model));
        tableModel->setItem(2, 0, new QStandardItem(year));

        ui.tableView_CarRecord->resizeColumnsToContents();
    }
}

void Autoservis::updateChangeRecordButtonState() {
    bool isBrandSelected = ui.comboBox_Brands->currentIndex() != -1;
    bool isModelSelected = ui.comboBox_Models->currentIndex() != -1;
    bool isYearSelected = ui.comboBox_Years->currentIndex() != -1;

    ui.pushButton_ChangeRecord->setEnabled(isBrandSelected && isModelSelected && isYearSelected);
}

void Autoservis::pushButton_CancelOrder_clicked() {
    QItemSelectionModel* selectionModel = ui.tableView_Orders->selectionModel();

    if (!selectionModel->hasSelection()) {
        QMessageBox::warning(this, tr("Zrusenie objednavky"), tr("Najprv vyberte objednavku, ktoru chcete zrusit!"));
        return;
    }

    QModelIndexList selectedRows = selectionModel->selectedRows();

    QStandardItemModel* tableModel = qobject_cast<QStandardItemModel*>(ui.tableView_Orders->model());

    for (const QModelIndex& index : selectedRows) {
        tableModel->removeRow(index.row());
    }

    QMessageBox::information(this, tr("Zrusenie objednavky"), tr("Vasa objednavka bola uspesne zrusena."));
}

void Autoservis::pushButton_GenerateInvoice_clicked() {
    QStandardItemModel* tableModel = qobject_cast<QStandardItemModel*>(ui.tableView_Orders->model());
    if (!tableModel) {
        return;
    }

    QString invoiceText = "Faktura\n";
    invoiceText += "-------------------------\n";
    double totalPrice = 0.0;

    for (int row = 0; row < tableModel->rowCount(); row++) {
        QString serviceName = tableModel->item(row, 0)->text();
        QString servicePrice = tableModel->item(row, 1)->text();
        invoiceText += serviceName + ": " + servicePrice + " EUR\n";
        totalPrice += servicePrice.toDouble();
    }

    invoiceText += "-------------------------\n";
    invoiceText += "Celkova cena: " + QString::number(totalPrice) + " EUR\n";

    QString defaultFileName = directoryPath + "Faktury\\" + currentUser->getUsername() + "_faktura.txt";

    QString filePath = QFileDialog::getSaveFileName(this, tr("Ulozit fakturu"), defaultFileName, tr("Textove subory (*.txt)"));
    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Chyba"), tr("Nemozno otvorit subor na zapis."));
        return;
    }

    QTextStream out(&file);
    out << invoiceText;
    file.close();

    QMessageBox::information(this, tr("Faktura"), tr("Faktura bola uspesne vygenerovana a ulozena."));
}

void Autoservis::pushButton_AddUser_clicked() {
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui.tableView_Users->model());

    if (!model) {
        model = new QStandardItemModel(this);
        QStringList headers = { "Meno pouzivatela", "Typ pouzivatela", "ICO firmy", "Plati DPH" };
        model->setHorizontalHeaderLabels(headers);
        ui.tableView_Users->setModel(model);
    }

    QStandardItem* nameItem = new QStandardItem();
    nameItem->setEditable(true);

    QStandardItem* typeItem = new QStandardItem();
    typeItem->setEditable(true);

    QStandardItem* icoItem = new QStandardItem();
    icoItem->setEditable(true);

    QStandardItem* vatItem = new QStandardItem();
    vatItem->setEditable(true);

    QStandardItem* passwordItem = new QStandardItem();
    passwordItem->setEditable(true);

    model->appendRow(QList<QStandardItem*>() << nameItem << typeItem << icoItem << vatItem << passwordItem);

    QModelIndex index = model->index(model->rowCount() - 1, 0);
    ui.tableView_Users->setCurrentIndex(index);
    ui.tableView_Users->edit(index);
}

void Autoservis::pushButton_RemoveUser_clicked() {
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui.tableView_Users->model());
    if (!model) {
        qDebug() << "Model nie je nastaveny.";
        return;
    }

    QItemSelectionModel* selectionModel = ui.tableView_Users->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::information(this, tr("Ziadny vyber"), tr("Nie je vybrany ziadny pouzivatel."));
        return;
    }

    int ret = QMessageBox::warning(this, tr("Potvrdenie vymazania"),
        tr("Naozaj chcete vymazat vybraneho pouzivatela?"),
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        for (int i = selectedRows.count() - 1; i >= 0; i--) {
            model->removeRow(selectedRows.at(i).row());
        }
    }
}

void Autoservis::comboBox_SelectUser_IndexChanged() {
    QString selectedUserName = ui.comboBox_SelectUser->currentText();
    QString ordersFileName = directoryPath + "Objednavky\\" + selectedUserName + "_objednavky.txt";
    QFile ordersFile(ordersFileName);

    if (!ordersFile.exists()) {
        qDebug() << "Subor s objednavkami pre pouzivatela ešte neexistuje.";
        return;
    }

    if (!ordersFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Nemozno otvorit subor s objednavkami:" << ordersFile.errorString();
        return;
    }

    QTextStream in(&ordersFile);
    QStandardItemModel* ordersModel = new QStandardItemModel(0, 3, this);
    QStringList headers = { "Sluzba", "Cena za sluzbu (EUR)", "Stav objednavky" };
    ordersModel->setHorizontalHeaderLabels(headers);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        QList<QStandardItem*> items;
        
        for (int i = 0; i < fields.size(); i++) {
            QStandardItem* item = new QStandardItem(fields[i]);
            if (i < 2) {
                item->setEditable(false);
            }
            items.append(item);
        }

        while (items.size() < 3) {
            QStandardItem* item = new QStandardItem();
            if (items.size() < 2) {
                item->setEditable(false);
            }
            items.append(item);
        }

        ordersModel->appendRow(items);
    }

    ui.tableView_UsersOrders->setModel(ordersModel);
    ordersFile.close();
    ui.tableView_UsersOrders->resizeColumnsToContents();
}

void Autoservis::pushButton_SaveChangesInOrders_clicked() {
    QString selectedUserName = ui.comboBox_SelectUser->currentText();
    QString ordersFileName = directoryPath + "Objednavky\\" + selectedUserName + "_objednavky.txt";
    QFile ordersFile(ordersFileName);

    if (!ordersFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Nemozno otvorit subor na zapis:" << ordersFile.errorString();
        QMessageBox::critical(this, tr("Chyba pri ukladani"), tr("Nemozno otvorit subor na zapis: ") + ordersFile.errorString());
        return;
    }

    QTextStream out(&ordersFile);
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui.tableView_UsersOrders->model());
    if (model) {
        for (int row = 0; row < model->rowCount(); row++) {
            QStringList rowData;
            for (int column = 0; column < model->columnCount(); column++) {
                QStandardItem* item = model->item(row, column);
                if (item) {
                    rowData << item->text();
                }
                else {
                    rowData << "";
                }
            }
            out << rowData.join(",") << "\n";
        }
    }

    ordersFile.close();
    QMessageBox::information(this, tr("Udaje ulozene"), tr("Udaje boli uspesne ulozene do suboru: ") + ordersFileName);
}

void  Autoservis::pushButton_LogOut_clicked() {

    if (Client* client = dynamic_cast<Client*>(currentUser)) {
        //  -----------<< Ukladanie záznamu o vozidle >>-----------

        QFile file(directoryPath + "vozidla_zaznam.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Nemozno otvorit subor na citanie:" << file.errorString();
            return;
        }

        QStringList fileLines;
        QTextStream in(&file);
        while (!in.atEnd()) {
            fileLines.append(in.readLine());
        }
        file.close();

        QStandardItemModel* model = static_cast<QStandardItemModel*>(ui.tableView_CarRecord->model());
        QString updatedLine = currentUser->getUsername();
        for (int row = 0; row < model->rowCount(); row++) {
            QStandardItem* item = model->item(row, 0);
            if (item) {
                updatedLine += "," + item->text();
            }
        }

        for (int i = 0; i < fileLines.size(); i++) {
            if (fileLines.at(i).startsWith(currentUser->getUsername() + ",")) {
                fileLines[i] = updatedLine;
                break;
            }
        }

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Nemozno otvorit subor na zapis:" << file.errorString();
            return;
        }

        QTextStream outRecord(&file);
        for (const QString& line : fileLines) {
            outRecord << line << "\n";
        }
        file.close();
        //----------------------------------------------------------

        //  -----------<< Ukladanie zoznamu objednávok >>-----------
        QString ordersFileName = directoryPath + "Objednavky\\" + currentUser->getUsername() + "_objednavky.txt";

        QFile ordersFile(ordersFileName);
        if (!ordersFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Nemozno otvorit subor na zapis:" << ordersFile.errorString();
            return;
        }

        QTextStream outOrders(&ordersFile);
        QStandardItemModel* ordersModel = qobject_cast<QStandardItemModel*>(ui.tableView_Orders->model());
        if (ordersModel) {
            for (int row = 0; row < ordersModel->rowCount(); row++) {
                QString line;
                for (int column = 0; column < ordersModel->columnCount(); column++) {
                    if (ordersModel->item(row, column)) {
                        line += ordersModel->item(row, column)->text();
                        if (column < ordersModel->columnCount() - 1) {
                            line += ",";
                        }
                    }
                }
                outOrders << line << "\n";
            }
        }
        ordersFile.close();
    }
    else if (Employee* employee = dynamic_cast<Employee*>(currentUser)) {
        if (employee->getPosition() == Employee::Position::Administrator) {
            
            //  -----------<< Ukladanie zoznamu užívate¾ov >>-----------      

            QString usersFileName = directoryPath + "zoznam_uzivatelov.txt";
            QFile usersFile(usersFileName);
            if (!usersFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                qDebug() << "Nemožno otvori súbor na zápis:" << usersFile.errorString();
                return;
            }

            QTextStream outUsers(&usersFile);
            QStandardItemModel* usersModel = qobject_cast<QStandardItemModel*>(ui.tableView_Users->model());
            if (usersModel) {
                for (int row = 0; row < usersModel->rowCount(); row++) {
                    QString line;
                    bool isFirma = usersModel->item(row, 1)->text() == "firma";
                    for (int column = 0; column < usersModel->columnCount(); column++) {
                        QStandardItem* item = usersModel->item(row, column);
                        if (item && !item->text().isEmpty()) {
                            QString text = item->text();
                            if (column == 3 && isFirma) {
                                text = (text == "Ano") ? "true" : (text == "Nie" ? "false" : "");
                            }
                            if (column == usersModel->columnCount() - 1 || (column == 2 && !isFirma)) {
                                line += text;
                            }
                            else {
                                line += text + ",";
                            }
                        }
                    }
                    outUsers << line.trimmed() << "\n";
                }
            }
            usersFile.close();
        }
 //---------------------------------------------------------------------------       
    }

    this->close();
    
    SignInDialog* signInDialog = new SignInDialog();
    signInDialog->setAttribute(Qt::WA_DeleteOnClose);
    if (signInDialog->exec() == QDialog::Accepted) {}
}
