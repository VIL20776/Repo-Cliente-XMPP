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

    ui->pressence_status->addItems({
        "Online", 
        "Ausente", 
        "No disponible", 
        "Ocupado"
    });
    ui->pressence_status->setCurrentIndex(0);

    worker = new XMPPWorker();
    client_thread = new QThread(this);
    worker->moveToThread(client_thread);

    connect(this, &MainWindow::startXMPPClient, worker, &XMPPWorker::connectToServer);

    connect(worker, &XMPPWorker::messageReceived, this, &MainWindow::onMessageReceived);
    connect(worker, &XMPPWorker::rosterReceived, this, &MainWindow::onRosterReceived);
    connect(worker, &XMPPWorker::rosterItemAdded, this, &MainWindow::onRosterItemAdded);
    connect(worker, &XMPPWorker::presenceChanged, this, &MainWindow::onPresenceChanged);

    connect(ui->chat_list, &QListWidget::currentItemChanged, this, &MainWindow::onChatList_currentItemChanged);
    connect(ui->send, &QPushButton::clicked, this, &MainWindow::onSendButton_clicked);
    connect(ui->add_contact, &QPushButton::clicked, this, &MainWindow::onAddContact_clicked);
    connect(ui->add_chat, &QPushButton::clicked, this, &MainWindow::onAddChat_clicked);
    connect(ui->update_presence, &QPushButton::clicked, this, &MainWindow::onUpdatePresece_clicked);


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

void MainWindow::onRosterItemAdded(const QString &barejid) {
    ui->contact_list->addItem(barejid);
}

void MainWindow::onNewChat(const QString &barejid, const QString &message) {
    if (chat_register.contains(barejid))
        return;

    if (message.length() == 0)
        chat_register.insert(barejid, {});
    else
        chat_register.insert(barejid, {worker->getCurrentJID() + ":\n" + message});

    ui->chat_list->addItem(barejid);

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
    add_contact = new AddContact(worker, this);
    add_contact->exec();
    delete add_contact;
    add_chat = nullptr;
}

void MainWindow::onAddChat_clicked() {
    add_chat = new AddChat(worker, this);
    connect(add_chat, &AddChat::newChat, this, &MainWindow::onNewChat);
    add_chat->exec();
    disconnect(add_chat, &AddChat::newChat, this, &MainWindow::onNewChat);
    delete add_chat;
    add_chat = nullptr;
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

void MainWindow::onUpdatePresece_clicked() {
    worker->setPresence(
        ui->pressence_status->currentIndex(),
        ui->presence_message->text()
    );
}