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
    void rempliTable(unsigned int bytes[336]);
    ~Table();

private:
    Ui::Table *ui;
    unsigned char ToByte(bool b[8]);
    void afficheBytes(unsigned char bytes[42]);
private slots:
void selectedItems();
};

#endif // MAINWINDOW_H
