#include "signon.h"
#include "ui_signon_dialog.h"

SignOn::SignOn(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SignOn)
{
    ui->setupUi(this);
    connect(ui->start, &QPushButton::clicked, this, &SignOn::onStart_clicked);
}

SignOn::~SignOn()
{
    delete ui;
}

void SignOn::onStart_clicked()
{
    emit startXMPPClient(ui->user_jid->text(), ui->user_password->text());
    QDialog::accept();
}