#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class Table;
}

class Table : public QMainWindow
{
    Q_OBJECT

public:
    explicit Table(QWidget *parent = 0);
    ~Table();

private:
    Ui::Table *ui;

private slots:
void cellSelected(int nRow, int nCol);
};

#endif // MAINWINDOW_H
