#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QCoreApplication::setOrganizationName("Frinchaboy");
    QCoreApplication::setOrganizationDomain("frinchaboy.fr");
    QCoreApplication::setApplicationName("Arrozr");

    setListeners();
    createTimetable();
    setTimetableStyle();

    disableUi();

    watcher.start();
}

void MainWindow::selectDevice(QModelIndex index){
    selectedDevice = *devices.find(ui->listWidget->currentItem()->data(Qt::UserRole).toInt());
    ui->label->setText("<html><head/><body><p><span style=\"font-size:18pt\">"+selectedDevice->getName()+"</span></p></body></html>");
    ui->horizontalSlider->setValue(selectedDevice->getSeuil());
    enableUi();
}

void MainWindow::addDevice(Device *device){
    int id = device->getId();
    devices.insert(id, device);
    qDebug() << "new auth :" << id;

    QListWidgetItem *newItem = new QListWidgetItem;
    newItem->setText(device->getName());
    newItem->setData(Qt::UserRole, id);
    devicesItems.insert(id, newItem);

    ui->listWidget->insertItem(device->getId(), newItem);
}

void MainWindow::removeDevice(Device *device){
    int id = device->getId();

    qDebug() << "rem device :" << id;
    devices.remove(id);
    QListWidgetItem *item = *devicesItems.find(id);
    if(ui->listWidget->currentItem() == item || selectedDevice == device){
        disableUi();
        selectedDevice = nullptr;
    }
    ui->listWidget->removeItemWidget(item);
    devicesItems.remove(id);
    delete item;
}

void MainWindow::selectedItems()
{
    unsigned char bytes[42];
    bool bools[8];
    int nbBits = 0;
    int nbByte = 0;
    for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
        for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
            if (nbBits == 8){
                nbBits = 0;
                bytes[nbByte] = ToByte(bools);
                ++nbByte;
            }

            bools[nbBits] = ui->tableWidget->item(row,col)->isSelected();
            ++nbBits;

        }
    }

    afficheBytes(bytes);
}

void MainWindow::afficheBytes(unsigned char bytes[42]){
    QString retour;
    for (int i = 0; i < 42; ++i) {
        for (int j = 0; j < 8; ++j) {
            retour.append(QString::number((bytes[i] >> j) & 1));
        }
    }
    QMessageBox::information(this, "",retour);

}

unsigned char MainWindow::ToByte(bool b[8])
{
    unsigned char c = 0;
    for (int i=0; i < 8; ++i)
        if (b[i])
            c |= 1 << i;
    return c;
}

void MainWindow::setListeners(){
    connect( ui->Envoyer, SIGNAL( pressed() ), this, SLOT( selectedItems() ) );
    connect(ui->listWidget, SIGNAL(clicked(QModelIndex)), this, SLOT( selectDevice(QModelIndex) ));
    connect(&watcher, SIGNAL(connected(Device*)), this, SLOT(addDevice(Device*)));
    connect(&watcher, SIGNAL(disconnected(Device*)), this, SLOT(removeDevice(Device*)));
}

void MainWindow::createTimetable(){
    ui->tableWidget->setRowCount(48);
    ui->tableWidget->setColumnCount(7);

    for (char i = 0; i < 48; ++i) {
        QString half = (i % 2 == 1) ? "30" : "";
        ui->tableWidget->setVerticalHeaderItem(i, new QTableWidgetItem( QString("%1").arg(i/2, 2, 10, QChar('0')) + "h" + half));
    }

    ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("Dimanche"));
    ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Lundi"));
    ui->tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("Mardi"));
    ui->tableWidget->setHorizontalHeaderItem(3,new QTableWidgetItem("Mercredi"));
    ui->tableWidget->setHorizontalHeaderItem(4,new QTableWidgetItem("Jeudi"));
    ui->tableWidget->setHorizontalHeaderItem(5,new QTableWidgetItem("Vendredi"));
    ui->tableWidget->setHorizontalHeaderItem(6,new QTableWidgetItem("Samedi"));


    for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
        for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
            ui->tableWidget->setItem(row,col,new QTableWidgetItem(" "));
        }
    }
}

void MainWindow::setTimetableStyle(){
    QString styleHeader = "::section {"
                          "background-color: #262626;"
                          "color: #f2f2f2;"
                          "border-bottom:#d2d2d2;"
                          "padding: 10px;"
                          "text-align: center;"
                          "font-family: arial;"
                          "font-size: 16px;"
                          "font-weight: bold;}";

    ui->tableWidget->horizontalHeader()->setStyleSheet(styleHeader);
    ui->tableWidget->verticalHeader()->setStyleSheet("::section {background-color: #262626;color:#f2f2f2}");
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::disableUi(){
    ui->label->setText("<html><head/><body><p><span style=\"font-size:18pt\">Selectionnez un module</span></p></body></html>");
    ui->horizontalSlider->setEnabled(false);
    ui->tableWidget->setEnabled(false);
}

void MainWindow::enableUi(){
    ui->horizontalSlider->setEnabled(true);
    ui->tableWidget->setEnabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
