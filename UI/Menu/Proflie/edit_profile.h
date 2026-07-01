#ifndef EDIT_PROFILE_H
#define EDIT_PROFILE_H

#include <QWidget>

namespace Ui {
class EditProfile;
}

class EditProfile : public QWidget
{
    Q_OBJECT

public:
    explicit EditProfile(QWidget *parent = nullptr);
    void setInitialValues(const QString& name, const QString& username, const QString& email, const QString& phone);
    ~EditProfile();

signals:
    void navigateToMainMenu();
    void editProfileRequested(QString name, QString username, QString email, QString phone, QString oldPassword, QString newPassword);

private slots:
    void on_pushButton_save_changes_clicked();
    void on_pushButton_back_to_menu_clicked();
private:
    Ui::EditProfile *ui;
};

#endif // EDIT_PROFILE_H