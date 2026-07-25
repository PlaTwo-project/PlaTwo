#ifndef BASE_EXCEPTION_H
#define BASE_EXCEPTION_H

#include <exception>
#include <QString>
#include <QWidget>

class BaseException : public std::exception {

protected:
    QString message;
    QString title;

public:
    BaseException(const QString& msg, const QString& title = "Error");
    virtual ~BaseException() = default;

    const char* what() const noexcept override;
    virtual void showDialog(QWidget *parent = nullptr) const;
};

#endif // BASE_EXCEPTION_H