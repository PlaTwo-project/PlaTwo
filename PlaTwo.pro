QT += widgets

CONFIG += c++17

SOURCES += \
    Infrastructure/DataBase/history_storage_manager.cpp \
    Infrastructure/Database/storage_manager.cpp \
    Logic/Security/password_hasher.cpp \
    Logic/Security/validator.cpp \
    Logic/match_record.cpp \
    Logic/user.cpp \
    Management/App/app_manager.cpp \
    Management/App/session_manager.cpp \
    Management/Auth/authenticator.cpp \
    UI/Auth/ForgotPassword/forgot_password.cpp \
    UI/Auth/ForgotPassword/forgot_password2.cpp \
    UI/Auth/Login/login.cpp \
    UI/Auth/SignUp/signup.cpp \
    UI/Menu/game_menu.cpp \
    UI/Menu/main_menu.cpp \
    UI/Menu/Proflie/edit_profile.cpp \
    UI/host_page.cpp \
    main.cpp \
    UI/mainwindow.cpp

HEADERS += \
    Infrastructure/DataBase/history_storage_manager.h \
    Infrastructure/Database/storage_manager.h \
    Logic/Security/password_hasher.h \
    Logic/Security/validator.h \
    Logic/match_record.h \
    Logic/user.h \
    Management/App/app_manager.h \
    Management/App/session_manager.h \
    Management/Auth/authenticator.h \
    UI/Auth/ForgotPassword/forgot_password.h \
    UI/Auth/ForgotPassword/forgot_password2.h \
    UI/Auth/login/login.h \
    UI/Auth/signup/signup.h \
    UI/Menu/game_menu.h \
    UI/Menu/main_menu.h \
    UI/Menu/Proflie/edit_profile.h \
    UI/host_page.h \
    UI/mainwindow.h

FORMS += \
    UI/Auth/ForgotPassword/forgot_password.ui \
    UI/Auth/ForgotPassword/forgot_password2.ui \
    UI/Auth/Login/login.ui \
    UI/Auth/SignUp/signup.ui \
    UI/Menu/game_menu.ui \
    UI/Menu/main_menu.ui \
    UI/Menu/Proflie/edit_profile.ui \
    UI/host_page.ui \
    UI/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target