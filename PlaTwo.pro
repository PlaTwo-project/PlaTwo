QT += widgets
QT += network

CONFIG += c++17

SOURCES += \
    Infrastructure/DataBase/history_storage_manager.cpp \
    Infrastructure/Database/storage_manager.cpp \
    Infrastructure/Network/guest.cpp \
    Infrastructure/Network/host.cpp \
    Infrastructure/Network/network.cpp \
    Logic/Game/DotsAndBoxes/DotsAndBoxesBoard/dots_and_boxes_board.cpp \
    Logic/Game/DotsAndBoxes/DotsAndBoxesLogic/dots_and_boxes.cpp \
    Logic/Game/DotsAndBoxes/DotsAndBoxesMove/dots_and_boxes_move.cpp \
    Logic/Game/Record/match_record.cpp \
    Logic/Game/room_state.cpp \
    Logic/Security/password_hasher.cpp \
    Logic/Security/validator.cpp \
    Logic/User/user.cpp \
    Management/App/app_manager.cpp \
    Management/App/session_manager.cpp \
    Management/Auth/authenticator.cpp \
    Management/Game/game_manager.cpp \
    UI/Auth/ForgotPassword/forgot_password.cpp \
    UI/Auth/ForgotPassword/forgot_password2.cpp \
    UI/Auth/Login/login.cpp \
    UI/Auth/SignUp/signup.cpp \
    UI/MainWindow/mainwindow.cpp \
    UI/Menu/game_menu.cpp \
    UI/Menu/history.cpp \
    UI/Menu/main_menu.cpp \
    UI/Menu/Proflie/edit_profile.cpp \
    UI/Network/guest_page.cpp \
    UI/Network/host_page.cpp \
    main.cpp \

HEADERS += \
    Infrastructure/DataBase/history_storage_manager.h \
    Infrastructure/Database/storage_manager.h \
    Infrastructure/Network/guest.h \
    Infrastructure/Network/host.h \
    Infrastructure/Network/network.h \
    Logic/Game/BaseLogicClasses/Board.h \
    Logic/Game/BaseLogicClasses/Game.h \
    Logic/Game/BaseLogicClasses/Move.h \
    Logic/Game/DotsAndBoxes/DotsAndBoxesBoard/dots_and_boxes_board.h \
    Logic/Game/DotsAndBoxes/DotsAndBoxesLogic/dots_and_boxes.h \
    Logic/Game/DotsAndBoxes/DotsAndBoxesMove/dots_and_boxes_move.h \
    Logic/Game/Record/match_record.h \
    Logic/Game/game_name.h \
    Logic/Game/room_state.h \
    Logic/Interface/history_interface.h \
    Logic/Interface/network_interface.h \
    Logic/Interface/user_interface.h \
    Logic/Security/password_hasher.h \
    Logic/Security/validator.h \
    Logic/User/user.h \
    Management/App/app_manager.h \
    Management/App/session_manager.h \
    Management/Auth/authenticator.h \
    Management/Game/game_manager.h \
    UI/Auth/ForgotPassword/forgot_password.h \
    UI/Auth/ForgotPassword/forgot_password2.h \
    UI/Auth/login/login.h \
    UI/Auth/signup/signup.h \
    UI/Games/BasePage/base_page.h \
    UI/MainWindow/mainwindow.h \
    UI/Menu/game_menu.h \
    UI/Menu/history.h \
    UI/Menu/main_menu.h \
    UI/Menu/Proflie/edit_profile.h \
    UI/Network/guest_page.h \
    UI/Network/host_page.h \

FORMS += \
    UI/Auth/ForgotPassword/forgot_password.ui \
    UI/Auth/ForgotPassword/forgot_password2.ui \
    UI/Auth/Login/login.ui \
    UI/Auth/SignUp/signup.ui \
    UI/MainWindow/mainwindow.ui \
    UI/Menu/game_menu.ui \
    UI/Menu/history.ui \
    UI/Menu/main_menu.ui \
    UI/Menu/Proflie/edit_profile.ui \
    UI/Network/guest_page.ui \
    UI/Network/host_page.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin