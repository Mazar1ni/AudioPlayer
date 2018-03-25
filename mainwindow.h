#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QMediaPlayer>
#include <QMediaPlaylist>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString convertFileSize(qint64);

private slots:
    void on_btn_add_clicked();//���� ��� ��������� ���������� ������ ����� ���������� ����
    void on_durationChanged(qint64);
    void on_positionChanged(qint64);
    void on_progress(int);
    void on_btn_savePlaylist_clicked();
    void on_btn_openPlaylist_clicked();
    QString convertMM(qint64);

private:
    Ui::MainWindow *ui;
    QStandardItemModel *m_playlistModel;
    QMediaPlayer *m_player;
    QMediaPlaylist *m_playlist;
    qint64 counter = 0;//������ ������
    int lastVolume = 50;//��������� ���������
    QString durationSong;
};

#endif // MAINWINDOW_H
