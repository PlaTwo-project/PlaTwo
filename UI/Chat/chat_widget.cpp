#include "chat_widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QMenu>
#include <QGridLayout>
#include <QWidgetAction>
#include <QColor>

ChatWidget::ChatWidget(QWidget* parent) : QWidget(parent) {
    setupUi();
}

void ChatWidget::setupUi() {
    setMinimumWidth(230);
    setMaximumWidth(320);

    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(6, 6, 6, 6);
    main_layout->setSpacing(6);

    QLabel* title_label = new QLabel("Chat", this);
    title_label->setStyleSheet("font-weight: bold; font-size: 14px; color: #3B2414;");
    main_layout->addWidget(title_label);

    message_list = new QListWidget(this);
    message_list->setWordWrap(true);
    message_list->setSelectionMode(QAbstractItemView::NoSelection);
    message_list->setFocusPolicy(Qt::NoFocus);
    message_list->setStyleSheet(
        "QListWidget {"
        "border: 1px solid #B77A3D;"
        "border-radius: 6px;"
        "background-color: rgba(255, 248, 235, 230);"
        "color: #3B2414;"
        "}"
        "QListWidget::item {"
        "padding: 4px;"
        "}"
        );
    main_layout->addWidget(message_list, 1);

    QHBoxLayout* input_layout = new QHBoxLayout();
    input_layout->setSpacing(4);

    emoji_button = new QToolButton(this);
    emoji_button->setText(QString::fromUtf8("\xF0\x9F\x98\x80")); // 😀
    emoji_button->setToolTip("Insert emoji");
    emoji_button->setStyleSheet(
        "QToolButton {"
        "background-color: #B77A3D;"
        "color: #FFF8EA;"
        "border: 1px solid #8A5528;"
        "border-radius: 6px;"
        "padding: 4px;"
        "}"
        "QToolButton:hover {"
        "background-color: #C98A49;"
        "}"
        "QToolButton:pressed {"
        "background-color: #8F5729;"
        "}"
        );
    input_layout->addWidget(emoji_button);

    message_input = new QLineEdit(this);
    message_input->setPlaceholderText("Type a message...");
    message_input->setStyleSheet(
        "QLineEdit {"
        "background-color: rgba(255, 248, 235, 230);"
        "color: #2F1B0C;"
        "border: 1px solid #B77A3D;"
        "border-radius: 6px;"
        "padding: 4px 8px;"
        "}"
        );
    input_layout->addWidget(message_input, 1);

    send_button = new QPushButton("Send", this);
    send_button->setStyleSheet(
        "QPushButton {"
        "background-color: #B77A3D;"
        "color: #FFF8EA;"
        "border: 1px solid #8A5528;"
        "border-radius: 6px;"
        "padding: 4px 10px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background-color: #C98A49;"
        "}"
        "QPushButton:pressed {"
        "background-color: #8F5729;"
        "}"
        );
    input_layout->addWidget(send_button);

    main_layout->addLayout(input_layout);

    connect(send_button, &QPushButton::clicked, this, &ChatWidget::sendClicked);
    connect(message_input, &QLineEdit::returnPressed, this, &ChatWidget::sendClicked);
    connect(emoji_button, &QToolButton::clicked, this, &ChatWidget::emojiButtonClicked);
}

void ChatWidget::sendClicked() {
    QString text = message_input->text().trimmed();
    if (text.isEmpty())
        return;

    emit messageSubmitted(text);
    message_input->clear();
}

void ChatWidget::emojiButtonClicked() {
    static const QStringList emojis = {
        QString::fromUtf8("\xF0\x9F\x98\x80"), // 😀
        QString::fromUtf8("\xF0\x9F\x98\x82"), // 😂
        QString::fromUtf8("\xF0\x9F\x98\x8D"), // 😍
        QString::fromUtf8("\xF0\x9F\x98\x8E"), // 😎
        QString::fromUtf8("\xF0\x9F\x98\xA2"), // 😢
        QString::fromUtf8("\xF0\x9F\x98\xA1"), // 😡
        QString::fromUtf8("\xF0\x9F\x91\x8D"), // 👍
        QString::fromUtf8("\xF0\x9F\x91\x8E"), // 👎
        QString::fromUtf8("\xF0\x9F\x8E\x89"), // 🎉
        QString::fromUtf8("\xE2\x9D\xA4\xEF\xB8\x8F"), // ❤️
        QString::fromUtf8("\xF0\x9F\x94\xA5"), // 🔥
        QString::fromUtf8("\xF0\x9F\x91\x8F"), // 👏
    };

    QMenu menu(this);
    menu.setStyleSheet(
        "QMenu {"
        "background-color: #F7E7C6;"
        "border: 1px solid #B77A3D;"
        "}"
        "QPushButton {"
        "color: #3B2414;"
        "background: transparent;"
        "border: none;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(183, 122, 61, 60);"
        "border-radius: 4px;"
        "}"
        );

    QWidget* grid_container = new QWidget(&menu);
    QGridLayout* grid = new QGridLayout(grid_container);
    grid->setContentsMargins(4, 4, 4, 4);
    grid->setSpacing(2);

    const int columns = 4;
    for (int i = 0; i < emojis.size(); ++i) {
        QPushButton* emoji_btn = new QPushButton(emojis[i], grid_container);
        emoji_btn->setFlat(true);
        emoji_btn->setFixedSize(32, 32);
        connect(emoji_btn, &QPushButton::clicked, this, [this, &menu, emoji = emojis[i]]() {
            insertEmoji(emoji);
            menu.close();
        });
        grid->addWidget(emoji_btn, i / columns, i % columns);
    }

    QWidgetAction* action = new QWidgetAction(&menu);
    action->setDefaultWidget(grid_container);
    menu.addAction(action);
    menu.exec(emoji_button->mapToGlobal(QPoint(0, emoji_button->height())));
}

void ChatWidget::insertEmoji(const QString& emoji) {
    message_input->insert(emoji);
    message_input->setFocus();
}

void ChatWidget::appendMessage(const QString& sender_name, const QString& text, bool is_own_message) {
    QString display_name;
    if (is_own_message)
        display_name = "You";
    else
        display_name = sender_name;

    QListWidgetItem* item = new QListWidgetItem(QString("%1: %2").arg(display_name, text));
    if (is_own_message) {
        item->setTextAlignment(Qt::AlignRight);
        item->setForeground(QColor("#5B3A1E"));
    }
    else {
        item->setTextAlignment(Qt::AlignLeft);
        item->setForeground(QColor("#8A5A33"));
    }

    message_list->addItem(item);
    message_list->scrollToBottom();
}

void ChatWidget::clearMessages() {
    message_list->clear();
}
