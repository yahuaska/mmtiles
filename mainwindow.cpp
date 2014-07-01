#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    exporter = new TilesExporter(this);
    connect(this->ui->exportBtn, SIGNAL(clicked()), this, SLOT(onExportBtnClicked()));
    connect(this->ui->palettesPath, SIGNAL(clicked()), this, SLOT(showDialog()));
    connect(this->ui->dataPath, SIGNAL(clicked()), this, SLOT(showDialog()));
    connect(this->ui->exportPath, SIGNAL(clicked()), this, SLOT(showDialog()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete exporter;
}

void MainWindow::onExportBtnClicked()
{
    this->ui->progressBar->setValue(0);
    exporter->readTiles();
}

void MainWindow::tileExported()
{
    int value = this->ui->progressBar->value();
    this->ui->progressBar->setValue(value+1);
}

void MainWindow::showStatus(QString status)
{
    this->ui->status->setText(status);
}

void MainWindow::showDialog()
{
    QPushButton * button = (QPushButton *) sender();
    QString senderName =  button->objectName();
    qDebug() << senderName;
    QString fileName;
    if (senderName == QString("palettesPath"))
    {
        fileName = QFileDialog::getOpenFileName(this,
            tr("Open Palette"), "/home/ringo/.local/share/openxcom/data/GEODATA/", tr("All files (*)"));
        if (QFile(fileName).exists())
        {
            exporter->setProperty("palettePath", fileName);
        } else {
            qDebug() << fileName << " does not exists.";
        }
    } else if (senderName == QString("dataPath")) {
        fileName = QFileDialog::getOpenFileName(this,
            tr("Open datafile"), "/home/ringo/.local/share/openxcom/data/GEODATA/", tr("All files (*)"));
        if (QFile(fileName).exists())
        {
            exporter->setProperty("dataPath", fileName);
        } else {
            qDebug() << fileName << " does not exists.";
        }
    } else if (senderName == QString ("exportPath")) {
        fileName = QFileDialog::getExistingDirectory(this, "Select export path","/tmp/");
        exporter->setProperty("exportPath", fileName);
    }
}

void MainWindow::palettePathChanged(QString path)
{
    this->ui->palettesPath->setText(path);
}

void MainWindow::dataPathChanged(QString path)
{
    this->ui->dataPath->setText(path);
}

void MainWindow::exportPathChanged(QString path)
{
    this->ui->exportPath->setText(path);
}
