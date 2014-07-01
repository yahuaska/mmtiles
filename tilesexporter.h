#ifndef TILESEXPORTER_H
#define TILESEXPORTER_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QVector>
#include <QColor>
#include <QDebug>
#include <QPixmap>
#include <cstdio>

class TilesExporter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString palettePath READ palettePath WRITE setPalettePath NOTIFY palettePathChanged)
    Q_PROPERTY(QString exportPath READ exportPath WRITE setExportPath NOTIFY exportPathChanged)
    Q_PROPERTY(QString dataPath READ dataPath WRITE setDataPath NOTIFY dataPathChanged)
private:
    QString m_palettePath;
    QFile * m_palette;
    QVector<QRgb> m_colors;
    QString m_exportPath;
    QDir * m_export;
    QString m_dataPath;
    QFile * m_dataFile;
    QImage * m_pixmap;
    static const double k = 4.0476190476190474;  //Коэффициент, необходимый в связи с тем,
                                          //что в палитре максимальное значение интенсивности - 0x3F

    void fillColors();

    bool savePixmap(const uchar *data, QString name);

public:
    explicit TilesExporter(QObject *parent = 0);
    ~TilesExporter();
    void setPalettePath(QString s) { m_palettePath = s; emit palettePathChanged(s); }
    void setExportPath(QString s) { m_exportPath = s; emit exportPathChanged(s); }
    void setDataPath(QString s) { m_dataPath = s; emit dataPathChanged(s); }
    QString palettePath() { return  m_palettePath; }
    QString exportPath() { return m_exportPath; }
    QString dataPath() { return m_dataPath; }
    void readTiles();

signals:
    void palettePathChanged(QString);
    void exportPathChanged(QString);
    void dataPathChanged(QString);
    void tileExported();
    void statusChanged(QString);
public slots:

};

#endif // TILESEXPORTER_H
