#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "xmppworker.h"
#include "addcontact.h"

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

    connect(ui->chat_list, &QListWidget::currentItemChanged, this, &MainWindow::onChatList_currentItemChanged);
    connect(ui->send, &QPushButton::clicked, this, &MainWindow::onSendButton_clicked);
    connect(ui->add_contact, &QPushButton::clicked, this, &MainWindow::onAddContact_clicked);

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
    if (!chat_register.contains(from)) {
        chat_register.insert(from, {});
        ui->chat_list->addItem(from);
    }

    QString msg = from + ":\n" + message;

    chat_register[from].append(msg);

    if (currentChat == from)
        ui->message_list->addItem(msg);
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

void MainWindow::onChatList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous) {
    currentChat = current->text().split(u'\n').at(0);
    ui->chat_label->setText(currentChat);

    // remove items
    while (ui->message_list->count() > 0) 
        delete ui->message_list->takeItem(0);

    ui->message_list->addItems(chat_register.value(currentChat));
    
}

void MainWindow::onAddContact_clicked() {
    AddContact add_contact = AddContact(worker, this);
    add_contact.exec();
}

void MainWindow::onSendButton_clicked() {

    QString message = ui->msg_input->text();
    if (message == "")
        return;

    auto my_message = new QListWidgetItem();
    QString msg = worker->getCurrentJID() + ":\n" + message;
    chat_register[currentChat].append(msg);
    my_message->setText(msg);

    ui->message_list->addItem(my_message);
    ui->msg_input->setText("");
    
    worker->handleSendMessage(message, currentChat);
}