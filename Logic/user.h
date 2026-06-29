#ifndef USER_H
#define USER_H

#include <QString>

class User
{
private:
    QString name;
    QString username;
    QString phone_number;
    QString email;
    QString hashed_password;

public:
    User();
    User(const QString& name, const QString& username, const QString& phone_number, const QString& email, const QString& hashed_password);

    QString getName() const;
    QString getUsername() const;
    QString getPhoneNumber() const;
    QString getEmail() const;
    QString getHashedPassword() const;

    void setName(const QString& name);
    void setUsername(const QString& username);
    void setPhoneNumber(const QString& phone_number);
    void setEmail(const QString& email);
    void setHashedPassword(const QString& hashed_password);
};

#endif
