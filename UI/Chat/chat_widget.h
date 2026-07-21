#ifndef CHAT_WIDGET_H
#define CHAT_WIDGET_H

#include <QWidget>

class QListWidget;
class QLineEdit;
class QPushButton;
class QToolButton;

class ChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWidget(QWidget* parent = nullptr);

    void appendMessage(const QString& sender_name, const QString& text, bool is_own_message);
    void clearMessages();

signals:
    void messageSubmitted(const QString& text);

private slots:
    void sendClicked();
    void emojiButtonClicked();

private:
    QListWidget* message_list;
    QLineEdit* message_input;
    QPushButton* send_button;
    QToolButton* emoji_button;

    void setupUi();
    void insertEmoji(const QString& emoji);
};

#endif // CHAT_WIDGET_H