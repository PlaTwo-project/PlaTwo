#ifndef SESSION_STORAGE_MANAGER_H
#define SESSION_STORAGE_MANAGER_H

#include <QString>
#include <QList>

class SessionStorageManager
{
public:
    SessionStorageManager();

    bool addSession(int user_id);
    bool removeSession(int user_id);
    bool isUserOnline(int user_id);
    void clearAllSessions();

private:
    QString file_path;
    QList<int> online_users_list;

    void loadSessions();
    void saveSessions() const;
};

#endif // SESSION_STORAGE_MANAGER_H