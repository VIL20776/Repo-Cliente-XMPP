#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "xmppworker.h"

#include "QXmppLogger.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); 

    worker = new XMPPWorker();
    client_thread = new QThread(this);
    worker->moveToThread(client_thread);

    connect(this, &MainWindow::startXMPPClient, worker, &XMPPWorker::connectToServer);
    connect(worker, &XMPPWorker::messageReceived, this, &MainWindow::onMessageReceived);

    connect(client_thread, &QThread::finished, worker, &XMPPWorker::deleteLater);
    connect(client_thread, &QThread::finished, client_thread, &QThread::deleteLater);

    client_thread->start();
    emit startXMPPClient();
}

MainWindow::~MainWindow()
{
    delete ui;
    if (client_thread->isRunning()) {
        client_thread->quit();
        client_thread->wait();
    }
}

void MainWindow::onMessageReceived(const QString &message) {
    ui->message_list->addItem(message);
}
