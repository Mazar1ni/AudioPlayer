#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QPushButton>
#include <QFileInfo>
#include <QSlider>
#include <QIcon>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //��������� ������� ���������
    m_playlistModel = new QStandardItemModel(this);
    ui->playlistView->setModel(m_playlistModel);

    //������������� ��������� �������
    m_playlistModel->setHorizontalHeaderLabels(QStringList() << tr("Unknown")
                                                             << tr("File Path"));
    ui->playlistView->hideColumn(1);//�������� ������� � ������� ��������� ���� � �����
    ui->playlistView->verticalHeader()->setVisible(false);//�������� ��������� �����
    ui->playlistView->setSelectionBehavior(QAbstractItemView::SelectRows);//�������� ��������� �����
    ui->playlistView->setSelectionMode(QAbstractItemView::SingleSelection);//��������� �������� ������ ���� ������
    ui->playlistView->setEditTriggers(QAbstractItemView::NoEditTriggers);//��������� ��������������
    //�������� ������� ��������� ������� ������� � ������ TableView
    ui->playlistView->horizontalHeader()->setStretchLastSection(true);

    m_player = new QMediaPlayer;//�������������� �����
    m_playlist = new QMediaPlaylist;//�������������� ��������
    m_player->setPlaylist(m_playlist);//������������� �������� � �����
    m_player->setVolume(50);//��������� ���������������
    ui->volume->setValue(50);
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);//����������� ����� ������������

    //���������� ������ ���������� � ������ ����������
    //��������� �� ��������� �������������� ����� ��������
    //� ������/�����/��������� ����� �����
    connect(ui->btn_previous, &QPushButton::clicked, m_playlist, QMediaPlaylist::previous);
    connect(ui->btn_next, &QPushButton::clicked, m_playlist, QMediaPlaylist::next);
    connect(ui->btn_play, &QPushButton::clicked, m_player, QMediaPlayer::play);
    connect(ui->btn_pause, &QPushButton::clicked, m_player, QMediaPlayer::pause);
    connect(ui->btn_stop, &QPushButton::clicked, m_player, QMediaPlayer::stop);

    //
    connect(m_player, &QMediaPlayer::positionChanged, this, MainWindow::on_positionChanged);
    //
    connect(m_player, &QMediaPlayer::durationChanged, this, MainWindow::on_durationChanged);

    //���������/���������� ������ ��� �����
    connect(ui->btn_volume, &QPushButton::clicked, m_player, [this](){
       if(ui->volume->value() != 0)
       {
           lastVolume = ui->volume->value();
           ui->volume->setValue(0);
       }
       else
       {
           ui->volume->setValue(lastVolume);
       }
    });

    //��������� ��������
    connect(ui->volume, &QSlider::valueChanged, m_player, [this](){
        //�������� ���������
        m_player->setVolume(ui->volume->value());
        //�������� ������
        if(ui->volume->value() < 30 && ui->volume->value() > 0)
        {
            ui->btn_volume->setIcon(QIcon(":/buttons/LowVolume.png"));
        }
        else if(ui->volume->value() > 70)
        {
            ui->btn_volume->setIcon(QIcon(":/buttons/HighVolume.png"));
        }
        else if(ui->volume->value() < 1)
        {
            ui->btn_volume->setIcon(QIcon(":/buttons/Mute.png"));
        }
        else
        {
            ui->btn_volume->setIcon(QIcon(":/buttons/MediumVolume.png"));
        }
    });

    //��� ��������� �� ����� � ������� ������������ ���� � ���������
    connect(ui->playlistView, &QTableView::doubleClicked, [this](const QModelIndex &index){
       m_playlist->setCurrentIndex(index.row());
    });

    //��� ��������� ������� �������� ����� � ���������, ������������� �������� ����� � ����������� ����
    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, [this](int index){
        ui->currentTrack->setText(m_playlistModel->data(m_playlistModel->index(index, 0)).toString());
        ui->playlistView->selectRow(index);
        ui->playlistView->setFocus();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_player;
    delete m_playlist;
    delete m_playlistModel;
}

void MainWindow::on_btn_add_clicked()
{
    //� ������� ������� ������ ������ ������ ������������� ����� mp3 ������
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                     tr("Open Files"),
                                                     QString(),
                                                     tr("Audio Files (*.mp3 *.mp4)"));

    //������������� ������ �� ������ � ���� � ������
    //� �������� � ������� ������������ ��������
    foreach(QString filePath, files)
    {
        QList<QStandardItem *> items;
        QFileInfo fileSize(QUrl(filePath).toString());
        items.append(new QStandardItem(QDir(filePath).dirName()
                                       + "     " +
                                       convertFileSize(fileSize.size())));
        items.append(new QStandardItem(filePath));
        m_playlistModel->appendRow(items);
        m_playlist->addMedia(QUrl(filePath));

        counter += fileSize.size();
    }

    ui->filesSize->setText(convertFileSize(counter));
}

void MainWindow::on_positionChanged(qint64 position)
{
    ui->progress->setValue(position);
    ui->duration->setText(convertMM(position) + " / " + durationSong);
}

void MainWindow::on_durationChanged(qint64 position)
{
    ui->progress->setMaximum(position);
    durationSong = convertMM(position);
}

void MainWindow::on_progress(int position)
{
    m_player->setPosition(position);
}

QString MainWindow::convertMM(qint64 mm)
{
    int s = mm/1000;
    if(s >= 60)
    {
        int m = s/60;
        s = s - m * 60;
        if(s < 10)
        {
            return QString().setNum(m) +":0"+ QString().setNum(s);
        }
        else
        {
            return QString().setNum(m) +":"+ QString().setNum(s);
        }
    }
    else
    {
        if(s < 10)
        {
            return "0:0" + QString().setNum(s);
        }
        else
        {
            return "0:" + QString().setNum(s);
        }
    }

}

QString MainWindow::convertFileSize(qint64 nSize)
{
    qint64 i = 0;
    for (; nSize > 1023; nSize /= 1024, ++i) { }
    return QString().setNum(nSize) + "BKMGT"[i];
}

void MainWindow::on_btn_openPlaylist_clicked()
{
    QFileInfo fileSize;
    QString tempstr;
    QString fileName = QFileDialog::getOpenFileName(this,
                                                 "Open File",
                                                 "",
                                                 "Playlist (*.pl)");

    QFile fileProtocol(fileName);
    fileProtocol.open(QIODevice::ReadWrite);
    m_playlistModel->clear();
    m_playlist->clear();
    counter = 0;

    QTextStream readStream(&fileProtocol);
    while(!readStream.atEnd())
    {
        QList<QStandardItem *> items;
        tempstr = readStream.readLine();
        fileSize = (QUrl(tempstr).toString());
        items.append(new QStandardItem(QDir(tempstr).dirName()
                                       + "     " +
                                       convertFileSize(fileSize.size())));
        items.append(new QStandardItem(tempstr));

        m_playlistModel->appendRow(items);
        m_playlist->addMedia(QUrl(tempstr));

        counter += fileSize.size();
    }

    ui->filesSize->setText(convertFileSize(counter));

    m_playlistModel->setHorizontalHeaderLabels(QStringList() <<QString(QDir(fileName).dirName())
                                                             <<"File Path");
    ui->playlistView->hideColumn(1);

    fileProtocol.close();
}

void MainWindow::on_btn_savePlaylist_clicked()
{
    QString fn=QFileDialog::getSaveFileName(this,
                                            "Save file",
                                            "",
                                            "Playlist (*.pl)");
    m_playlist->save(QUrl::fromLocalFile(fn),"m3u");
}
