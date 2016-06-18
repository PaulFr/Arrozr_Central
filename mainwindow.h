#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QMessageBox>
#include <QCheckBox>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    unsigned char ToByte(bool b[8]);
    void afficheBytes(unsigned char bytes[42]);

private slots:
    void selectedItems();
};

#endif // MAINWINDOW_H
