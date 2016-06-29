#include "table.h"
#include "ui_table.h"
#include <QString>
#include <QMessageBox>
#include <QCheckBox>

Table::Table(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Table)
{
    ui->setupUi(this);

    ui->tableWidget->setRowCount(48);
    ui->tableWidget->setColumnCount(7);

    for (char i = 0; i < 48; ++i) {
        QString half = (i % 2 == 1) ? "30" : "";
        ui->tableWidget->setVerticalHeaderItem(i, new QTableWidgetItem( QString("%1").arg(i/2, 2, 10, QChar('0')) + "h" + half));
    }

    ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("Lundi"));
    ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Mardi"));
    ui->tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("Mercredi"));
    ui->tableWidget->setHorizontalHeaderItem(3,new QTableWidgetItem("Jeudi"));
    ui->tableWidget->setHorizontalHeaderItem(4,new QTableWidgetItem("Vendredi"));
    ui->tableWidget->setHorizontalHeaderItem(5,new QTableWidgetItem("Samedi"));
    ui->tableWidget->setHorizontalHeaderItem(6,new QTableWidgetItem("Dimanche"));

   for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
        for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
            ui->tableWidget->setItem(row,col,new QTableWidgetItem(" "));
        }
    }

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

    connect( ui->Envoyer, SIGNAL( pressed() ), this, SLOT( selectedItems() ) );
}

Table::~Table()
{
    delete ui;
}

void Table::selectedItems()
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

void Table::afficheBytes(unsigned char bytes[42]){
    QString retour;
    for (int i = 0; i < 42; ++i) {
        for (int j = 0; j < 8; ++j) {
            retour.append(QString::number((bytes[i] >> j) & 1));
        }
    }
    QMessageBox::information(this, "",retour);

}

void Table::rempliTable(unsigned int bytes[336]){

    int col=0;
    int row=0;
    for (int i = 0; i < sizeof(bytes); ++i) {
        ++row;
        if(row==48){
            row=0;
            ++col;
        }
        if (bytes[i] == 1){
            ui->tableWidget->item(row,col)->setSelected(true);
        }
    }
    QMessageBox::information(this, "","il y a : "+QString::number(sizeof(bytes)));

}

unsigned char Table::ToByte(bool b[8])
{
    unsigned char c = 0;
    for (int i=0; i < 8; ++i)
        if (b[i])
            c |= 1 << i;
    return c;
}
