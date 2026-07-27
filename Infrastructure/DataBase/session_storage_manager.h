#ifndef SESSION_STORAGE_MANAGER_H
#define SESSION_STORAGE_MANAGER_H

#include <QString>
#include <QMap>
#include <QLockFile>

class SessionStorageManager {
public:
    SessionStorageManager();
    ~SessionStorageManager();

    bool addSession(int user_id);
    bool removeSession(int user_id);
    bool isUserOnline(int user_id);
    void clearAllSessions();

private:
    QString getLockFilePath(int user_id) const;

    static QMap<int, QLockFile*>& getActiveLocks();
};

#endif // SESSION_STORAGE_MANAGER_H
