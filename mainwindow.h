#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <tilesexporter.h>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void onExportBtnClicked();
    void tileExported();
    void showStatus(QString status);
    void showDialog();
    void palettePathChanged(QString);
    void dataPathChanged(QString);
    void exportPathChanged(QString);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TilesExporter  * exporter;
};

#endif // MAINWINDOW_H
