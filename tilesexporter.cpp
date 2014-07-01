#include "tilesexporter.h"

TilesExporter::TilesExporter(QObject *parent) :
    QObject(parent),
    m_palettePath("/home/ringo/.local/share/openxcom/data/GEODATA/PALETTES.DAT"),
    m_exportPath("/tmp/tiles/"),
    m_dataPath("/home/ringo/.local/share/openxcom/data/GEODATA/SCANG.DAT")
{
    connect(this, SIGNAL(tileExported()),
            parent, SLOT(tileExported()));
    connect(this, SIGNAL(statusChanged(QString)),
            parent, SLOT(showStatus(QString)));
    connect(this, SIGNAL(palettePathChanged(QString)),
            parent, SLOT(palettePathChanged(QString)));
    connect(this, SIGNAL(exportPathChanged(QString)),
            parent, SLOT(exportPathChanged(QString)));
    //
    m_palette = new QFile(m_palettePath, this);
    if (! m_palette->exists())
    {
        qDebug() << m_palettePath << " does not exists.";
        emit statusChanged(m_palettePath + " does not exists.");
    }
    m_palette->open(QFile::ReadOnly);
    //
    m_dataFile = new QFile(m_dataPath, this);
    if (! m_dataFile->exists())
    {
        qDebug() << m_dataPath << " does not exists.";
        emit statusChanged(m_dataPath + " does not exists.");
    }
    m_dataFile->open(QFile::ReadOnly);
    //
    m_export = new QDir(m_exportPath);
    if (! m_export->exists())
    {
        if (! m_export->mkpath(m_exportPath))
        {
            emit statusChanged(m_dataPath + " does not exists and cannot be created.");
        }
    }
    fillColors();
    m_pixmap = new QImage(4, 4, QImage::Format_ARGB32);
}

void TilesExporter::fillColors()
{
    QByteArray bytes;
    bytes.resize(256*3);
    bytes.fill(0xff);
    m_palette->seek(3096);
    qint64 bytesRead = m_palette->read(bytes.data(), 256*3);
    if (bytesRead < 0)
    {
        emit statusChanged("Error: " + m_palette->errorString());
    }
    QColor tempColor;
    QByteArray * rgb = new QByteArray(3, 0);
    QByteArray::iterator it = bytes.begin();
    while (it != bytes.end())
    {
        for (int i = 0; i < 3 && it != bytes.end(); ++i)
        {
            (*rgb)[i] = *it;
            ++it;
        }
        tempColor.setRed(rgb->at(0) * k);
        tempColor.setGreen(rgb->at(1)* k);
        tempColor.setBlue(rgb->at(2) * k);
        m_colors.append(tempColor.rgb());
    }
    delete rgb;
}

void TilesExporter::readTiles()
{
    qint64 bytesRead = -1;
    uchar data[16];
    int c = 0;
    char str[2];

    bytesRead = m_dataFile->read((char *)data, 16);
    while (bytesRead > 0)
    {
        sprintf(str, "%d", c);
        savePixmap(data, QString(str));
        ++c;
        if (c >= 460)
            break;
        bytesRead = m_dataFile->read((char *)data, 16);
    }
    m_dataFile->seek(0);
}

bool TilesExporter::savePixmap(const uchar *data, QString name)
{
    m_pixmap->fill(0x00000000);
    for (int i = 0; i < 16; i++)
    {
        if (data[i] == 0)
            continue;
        m_pixmap->setPixel(i%4, i/4, m_colors[data[i]]);
    }
    bool result = m_pixmap->scaled(10, 10).save(m_exportPath + name + ".png", "PNG");
    if (result)
    {
        emit tileExported();
        emit statusChanged("Export is ok at " + name);
    } else {
        emit statusChanged("Export error at " + name);
    }
    return result;
}

TilesExporter::~TilesExporter()
{
    delete m_export;
    delete m_pixmap;
}
