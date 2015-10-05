#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QFile>
#include <QListWidgetItem>
#include <QColor>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gestBase();
    startAp();
}

MainWindow::~MainWindow()
{
    maBase.close();
    delete ui;
}

void MainWindow::gestBase()
{
    QFile fichierBase("./hosto.data");
    if(!fichierBase.exists())
    {
        createBase();
    }
    maBase = QSqlDatabase::addDatabase("QSQLITE");
    maBase.setDatabaseName("./hosto.data");
    maBase.open();

    /*QSqlQuery req;
    req.exec("select patNum, patNom, strftime('%d-%m-%Y',presDateFin), round(julianday(presDateFin)-julianday('now'),0) from Prescription natural join Patient order by presDateFin asc");
    QListWidgetItem *itm;
    while(req.next())
    {
        itm=new QListWidgetItem;
        itm->setText(req.value(1).toString()+" ["+req.value(2).toString()+"] "+req.value(3).toString()+" jours");
        QColor rouge, orange, vert, finalColor;
        rouge.setRgb(255, 0, 0);
        orange.setRgb(250, 100, 20);
        vert.setRgb(0, 255, 0);
        if(req.value(3).toInt() <= 0)
        {
            finalColor = rouge;
        }
        else if(req.value(3).toInt() <= 7 && req.valuudate Prescription setPresDateFin = 2015/10/01where presNum = 2e(3).toInt() > 0){
            finalColor = orange;
        }
        else if(req.value(3).toInt() > 7){
            finalColor = vert;
        }

        itm->setBackgroundColor(finalColor);
        ui->listWidgetPrescriptionRetard->addItem(itm);
    }
    sum(strftime( '%s', Heure_retour) - strftime( '%s', Heure_debut))/60*/
}

void MainWindow::createBase()
{
    QSqlDatabase creaBase= QSqlDatabase::addDatabase("QSQLITE");
    creaBase.setDatabaseName("./hosto.data");
    creaBase.open();
    QSqlQuery req;
    req.exec("create table Patient(patNum smallint primary key, patNom varchar(50), patNumSS varchar(50), patDoc varchar(50))");
    req.exec("create table Medecin(docNum smallint primary key, docNom varchar(50), docNumSS varchar(50), docAdresse1 varchar(100), docAdresse2 varchar(100), docCP varchar(20), docVille varchar(50), docTel varchar(15))");
    req.exec("create table Prescription(presNum smallint primary key, presDuree smallint, presDateFin date, presModeRenouv varchar(1), presType varchar(1), patNum smallint)");
    req.exec("create table Parametre(medCodePostal varchar(20), medVille varchar(50), dureeDSI smallint, dureeOrdonnance smallint)");

    req.exec("insert into Patient values(0, 'ROUX Thomas', '10000000900',0)");
    req.exec("insert into Patient values(1, 'LEGRAND julien', '20000000', 1)");
    req.exec("insert into Patient values(2, 'ZEFMAN Numa', '70505020759', 2)");

    req.exec("insert into Medecin values(0, 'BRIGITTE Lievin', '10000000900', 'rue des lilas', 'impasse de la terre', '04856', 'Boisier', '0685749312')");
    req.exec("insert into Medecin values(1, 'DELVOI Valery', '20000000', 'cour mirrabeau', 'allée des Chene', '14500', 'Rechefort', '0612479635')");
    req.exec("insert into Medecin values(2, 'PONS Charlotte', '70505020759', 'cour de la trinite', 'avenue du palace', '71593', 'Aubagne', '0647582196')");

    req.exec("insert into Prescription values(0, 10, '2015-10-18', '0', '1', 0)");
    req.exec("insert into Prescription values(1, 3, '2015-09-18', '0', '1', 1)");
    req.exec("insert into Prescription values(2, 6, '2015-09-30', '0', '1', 2)");

    req.exec("insert into Parametre values('05000', 'Gap', 15, 30)");
    creaBase.close();
}

void MainWindow::startAp()
{
    QSqlQueryModel * modelReq = new QSqlQueryModel;
    modelReq->setQuery("select patNum, patNom, strftime('%d-%m-%Y',presDateFin) as dateFin, round(julianday(presDateFin)-julianday('now'),0) as nbJourRestant from Prescription natural join Patient order by presDateFin");
    ui->tableViewRenouv->setModel(modelReq);
    ui->tableViewRenouv->hideColumn(0);
    ui->tableViewRenouv->show();
}

void MainWindow::on_tableViewRenouv_clicked(const QModelIndex &index)
{
    QAbstractItemModel * model = ui->tableViewRenouv->model();
    QString numeroPrescriptionClicked = model->data(model->index(index.row(),0)).toString();
    QSqlQuery req;
    req.exec("select presDateFin, presDuree, presType, presModeRenouv, patNum from Prescription where presNum = "+ numeroPrescriptionClicked);
    req.next();
    ui->labelPresValableValue->setText(req.value(0).toString());
    ui->labelPresDureeValue->setText(req.value(1).toString());
    ui->labelPresTypeValue->setText(req.value(2).toString());
    ui->labelPresModeRenouvValue->setText(req.value(3).toString());
    ui->dateEditRenouveller->setMinimumDate(req.value(0).toDate().addDays(1));
    ui->dateEditRenouveller->setDate(req.value(0).toDate().addDays(1));

    //cout<<req.value(0).toDate().toString("dd/MM/yyyy").toStdString()<<endl;

    QString numeroPatient = req.value(4).toString();
    req.exec("select patNom, patDoc from Patient where patNum = "+ numeroPatient);
    req.next();
    ui->labelPatientValue->setText(req.value(0).toString());

    QString numeroMedecin = req.value(1).toString();
    req.exec("select docNom from Medecin where docNum = "+ numeroMedecin);
    req.next();
    ui->labelMedecinValue->setText(req.value(0).toString());
    ui->tableViewRenouv->selectRow(index.row());
}

void MainWindow::on_pushButtonRenouveller_clicked()
{
    int row = ui->tableViewRenouv->selectionModel()->selectedRows().first().row();
    QAbstractItemModel * model = ui->tableViewRenouv->model();
    QString numeroPrescriptionClicked = model->data(model->index(row,0)).toString();
    QSqlQuery req;
    req.exec("update Prescription set PresDateFin = '"+ ui->dateEditRenouveller->date().toString("yyyy-MM-dd") +"' where presNum = "+ numeroPrescriptionClicked);
    req.next();
    startAp();
}
