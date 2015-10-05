#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void gestBase();
    void createBase();
    void startAp();

private slots:

    void on_tableViewRenouv_clicked(const QModelIndex &index);

    void on_pushButtonRenouveller_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase maBase ;

};

#endif // MAINWINDOW_H
