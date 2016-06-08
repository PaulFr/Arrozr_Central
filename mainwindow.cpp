#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QMessageBox>
#include <QCheckBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidget->setRowCount(24);
    ui->tableWidget->setColumnCount(7);

    for (char i = 0; i < 24; ++i) {
        ui->tableWidget->setVerticalHeaderItem(i, new QTableWidgetItem( QString("%1").arg(i, 2, 10, QChar('0')) + "h" ));
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
            ui->tableWidget->setItem(row,col,new QTableWidgetItem());
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


    connect( ui->tableWidget, SIGNAL( cellClicked(int,int) ), this, SLOT( cellSelected( int, int ) ) );

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::cellSelected(int nRow, int nCol)
{

    QMessageBox::information(this, "", "Cell at row "+QString::number(nRow)+" column "+QString::number(nCol)+ " was double clicked.");

}
