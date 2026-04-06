#include "mainwindow.hpp"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::slotServDisconnected);

    connect(socket, &QTcpSocket::connected, this, &MainWindow::slotConnected);
    connect(socket, &QTcpSocket::errorOccurred, this, &MainWindow::slotErrConnected);

    nextBlockSize = 0;
}

MainWindow::~MainWindow()
{
    socket -> close();
    delete ui;
}

void MainWindow::on_pushButton_connect_clicked()
{
    // todo: окно с вводом айпи сервера
    if (socket->state() == QAbstractSocket::ConnectedState) {
        ui->textBrowser->append(" INFO >> Вы уже подключены.");
        return;
    }

    socket -> connectToHost("127.0.0.1", 6953);
}

void MainWindow::SendToServer(QString send_str)
{
    if(socket->state() != QAbstractSocket::ConnectedState) {
        ui ->textBrowser ->append(" INFO >> Клиент не подключен к серверу.");
        return;
    }

    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);

    out << quint32(0);
    out << qint8(0) << send_str;
    out.device()->seek(0);
    out << qint32(Data.size() - sizeof(qint32));

    socket -> write(Data);
    ui->lineEdit->clear();
}

void MainWindow::slotConnected()
{
    ui ->textBrowser ->append(" INFO >> Вы подключены к серверу.");
}

void MainWindow::slotErrConnected()
{
    ui ->textBrowser ->append(" INFO >> Не удалось подключиться к серверу. Повторите попытку позже.");
    socket->close();
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);

    if(in.status() == QDataStream::Ok) {
        for(;;) {
            // Узнаем размер блока
            if(nextBlockSize == 0) {
                if(socket->bytesAvailable() < sizeof(quint32)) {
                    break;
                }
                in >> nextBlockSize;
            }
            if(socket->bytesAvailable() < nextBlockSize) {
                break;
            }

            QString str;
            in >> str;
            nextBlockSize = 0;
            ui->textBrowser->append(str);
        }
    }
    else {
        ui ->textBrowser ->append("read error!");
    }
}

void MainWindow::slotServDisconnected()
{
    ui->textBrowser->append(" INFO >> Подключение к серверу прервано.");
    socket->close();
}

void MainWindow::on_pushButton_clicked()
{
    SendToServer(ui->lineEdit->text());
}

void MainWindow::on_lineEdit_returnPressed()
{
    SendToServer(ui->lineEdit->text());
}

void MainWindow::sendFileToServer(const QString &filepath)
{
    if(socket->state() != QAbstractSocket::ConnectedState) {
        ui ->textBrowser ->append(" INFO >> Клиент не подключен к серверу.");
        return;
    }

    QFile file(filepath);

    if (!file.open(QIODevice::ReadOnly)){
        ui ->textBrowser ->append("File read error!");
        return;
    }

    //todo: QCryptographicHash

    qint64 FileSize = file.size();

    QFileInfo fileInfo(filepath);
    QString FileName = fileInfo.fileName();


    QByteArray headerData;
    QDataStream out(&headerData, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);

    // Reserve memory for size and type
    out << quint32(0) << qint8(1)
        << FileName
        << FileSize;

    out.device()->seek(0);
    out << qint32(headerData.size() - sizeof(qint32));

    socket -> write(headerData);

    ui ->textBrowser ->append("Отправка файла: " + FileName);

    // QByteArray
    while(!file.atEnd()) {
        QByteArray chunk = file.read(64*1024);
        socket -> write(chunk);
    }

    file.close();
}

void MainWindow::on_send_QFile_clicked()
{
    if(socket->state() != QAbstractSocket::ConnectedState) {
        ui ->textBrowser ->append(" INFO >> Клиент не подключен к серверу.");
        return;
    }

    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Выберите файл для отправки",
                                                    QString(),
                                                    "Все файлы (*.*)");
    if (!filePath.isEmpty()) {
        sendFileToServer(filePath);
    }
}


void MainWindow::on_pushButton_disconnect_clicked()
{
    socket -> close();
}


