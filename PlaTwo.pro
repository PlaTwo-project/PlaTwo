QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Logic/Security/password_hasher.cpp \
    UI/forgot_password.cpp \
    UI/forgot_password2.cpp \
    UI/signup.cpp \
    main.cpp \
    UI/mainwindow.cpp

HEADERS += \
    Logic/Security/password_hasher.h \
    UI/forgot_password.h \
    UI/forgot_password2.h \
    UI/signup.h \
    UI/mainwindow.h

FORMS += \
    UI/forgot_password.ui \
    UI/forgot_password2.ui \
    UI/signup.ui \
    UI/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
