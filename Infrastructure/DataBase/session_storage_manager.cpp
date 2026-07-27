#include "session_storage_manager.h"
#include <QDir>
#include <QStandardPaths>

SessionStorageManager::SessionStorageManager() {
}

SessionStorageManager::~SessionStorageManager() {
}

QMap<int, QLockFile*>& SessionStorageManager::getActiveLocks() {
    static QMap<int, QLockFile*> active_locks;
    return active_locks;
}

QString SessionStorageManager::getLockFilePath(int user_id) const {
    return QDir::tempPath() + QString("/platwo_user_%1.lock").arg(user_id);
}

bool SessionStorageManager::isUserOnline(int user_id) {
    if (getActiveLocks().contains(user_id))
        return true;


    QLockFile testLock(getLockFilePath(user_id));
    testLock.setStaleLockTime(0);

    if (testLock.tryLock()) {
        testLock.unlock();
        return false;
    }

    return true;
}

bool SessionStorageManager::addSession(int user_id) {
    if (isUserOnline(user_id))
        return false;

    auto *lock = new QLockFile(getLockFilePath(user_id));
    lock->setStaleLockTime(0);

    if (lock->tryLock()) {
        getActiveLocks()[user_id] = lock;
        return true;
    } else {
        delete lock;
        return false;
    }
}

bool SessionStorageManager::removeSession(int user_id) {
    if (getActiveLocks().contains(user_id)) {
        QLockFile *lock = getActiveLocks().take(user_id);
        lock->unlock();
        delete lock;
        return true;
    }
    return false;
}

void SessionStorageManager::clearAllSessions() {
    for (int user_id : getActiveLocks().keys()) {
        removeSession(user_id);
    }
}