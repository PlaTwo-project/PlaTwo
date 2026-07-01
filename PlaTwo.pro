QT += widgets

CONFIG += c++17

SOURCES += \
    Infrastructure/Database/storage_manager.cpp \
    Logic/Security/password_hasher.cpp \
    Logic/Security/validator.cpp \
    Logic/user.cpp \
    Management/app_manager.cpp \
    Management/authenticator.cpp \
    UI/Auth/ForgotPassword/forgot_password.cpp \
    UI/Auth/ForgotPassword/forgot_password2.cpp \
    UI/Auth/Login/login.cpp \
    UI/Auth/SignUp/signup.cpp \
    UI/Menu/main_menu.cpp \
    UI/Menu/Proflie/edit_profile.cpp \
    main.cpp \
    UI/mainwindow.cpp

HEADERS += \
    Infrastructure/Database/storage_manager.h \
    Logic/Security/password_hasher.h \
    Logic/Security/validator.h \
    Logic/user.h \
    Management/app_manager.h \
    Management/authenticator.h \
    UI/Auth/ForgotPassword/forgot_password.h \
    UI/Auth/ForgotPassword/forgot_password2.h \
    UI/Auth/login/login.h \
    UI/Auth/signup/signup.h \
    UI/Menu/main_menu.h \
    UI/Menu/Proflie/edit_profile.h \
    UI/mainwindow.h

FORMS += \
    UI/Auth/ForgotPassword/forgot_password.ui \
    UI/Auth/ForgotPassword/forgot_password2.ui \
    UI/Auth/Login/login.ui \
    UI/Auth/SignUp/signup.ui \
    UI/Menu/main_menu.ui \
    UI/Menu/Proflie/edit_profile.ui \
    UI/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target