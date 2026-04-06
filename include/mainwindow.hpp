#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QNetworkAccessManager>
#include <QFile>
#include <QObject>
#include <QFileInfo>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // QCryptographicHash hash(QCryptographicHash::Sha256);

private slots:
    // void on_pushButton_2_clicked();
    void slotConnected();
    void slotErrConnected();

    void on_pushButton_clicked();
    void on_lineEdit_returnPressed();
    void on_send_QFile_clicked();

    void sendFileToServer(const QString &filepath);

    // void on_pushButton_3_clicked();

    void on_pushButton_disconnect_clicked();

    void on_pushButton_connect_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QByteArray Data;
    void SendToServer(QString send_str);

    qint32 nextBlockSize;

public slots:
    void slotReadyRead();
    void slotServDisconnected();
};


// class FileUploader : public QObject
// {
//     Q_OBJECT

// public:
//     FileUploader(const QString &filepath, QObject *parent);
//     ~FileUploader();
//     // QFile *file = new QFile(filepath);

// private:
//     QFile file;
//     QString filepath;
//     QNetworkAccessManager* uplManager;

// private slots:
//     void uplProgress();
//     void uplFinished();
// };

#endif // MAINWINDOW_HPP
