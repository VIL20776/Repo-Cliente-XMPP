#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "xmppworker.h"

#include "QXmppLogger.h"

#include <QDebug>

QMap<QString, QStringList> chat_register;
QString currentChat;

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
    connect(worker, &XMPPWorker::rosterReceived, this, &MainWindow::onRosterReceived);
    connect(worker, &XMPPWorker::presenceChanged, this, &MainWindow::onPresenceChanged);

    connect(ui->contact_list, &QListWidget::currentItemChanged, this, &MainWindow::onContactList_currentItemChanged);

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

void MainWindow::onMessageReceived(const QString &from, const QString &message) {
    chat_register[from].append(message);

    if (currentChat == from)
        ui->message_list->addItem(message);
}

void MainWindow::onRosterReceived(const QStringList &barejids) {
    for (auto jid: barejids) {
        QListWidgetItem *contact = new QListWidgetItem;
        contact_map.insert(jid, contact);
        ui->contact_list->addItem(contact);
    }

}

void MainWindow::onPresenceChanged(const QString &barejid, const QString &presence) {
    if (contact_map.contains(barejid))
        contact_map[barejid]->setText(barejid + "\n" + presence);
}

void MainWindow::onContactList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous) {
    currentChat = current->text().split(u'\n').at(0);
    ui->chat_label->setText("JID: " + currentChat);

    // remove items
    for (int i = 0; i < ui->message_list->count(); i++) 
        delete ui->message_list->takeItem(i);

    ui->message_list->addItems(chat_register[currentChat]);
    
}