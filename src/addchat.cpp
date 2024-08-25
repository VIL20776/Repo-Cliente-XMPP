#include "addchat.h"
#include "ui_addchat_dialog.h"

AddChat::AddChat(XMPPWorker *worker, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddChat),
    worker(worker)
{
    ui->setupUi(this);
    ui->message->setText("Hola, soy" + worker->getCurrentJID());
    
    connect(ui->start_chat, &QPushButton::clicked, this, &AddChat::onStartChat_clicked);
}

AddChat::~AddChat()
{

}

void AddChat::onStartChat_clicked() {
    
}