#ifndef SIGNON_DIALOG_H
#define SIGNON_DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class SignOn;
}
QT_END_NAMESPACE

// Ventana de inicio de sesión
class SignOn : public QDialog
{
    Q_OBJECT

private:
    Ui::SignOn *ui;

public:
    SignOn(QWidget *parent = nullptr);
    ~SignOn();

public slots:
    void onStart_clicked();

signals:
    void startXMPPClient(const QString &jid, const QString &password);

};
#endif // SIGNON_DIALOG_H
