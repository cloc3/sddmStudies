/***************************************************************************
* Copyright (c) 2013 Abdurrahman AVCI <abdurrahmanavci@gmail.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the
* Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
***************************************************************************/

#include "UserModel.h"

#include "Constants.h"
#include "Configuration.h"

#include <QFile>
#include <QList>
#include <QTextStream>
#include <QStringList>

#include <memory>
#include <pwd.h>

namespace SDDM {
    class User {
    public:
        QString name;
        QString realName;
        QString homeDir;
        QString icon;
        bool needsPassword { false };
        int uid { 0 };
        int gid { 0 };
    };

    typedef std::shared_ptr<User> UserPtr;

    class UserModelPrivate {
    public:
        int lastIndex { 0 };
        QList<UserPtr> users;
    };

    UserModel::UserModel(QObject *parent) : QAbstractListModel(parent), d(new UserModelPrivate()) {
        const QString facesDir = mainConfig.Theme.FacesDir.get();
        const QString defaultFace = QStringLiteral("%1/.face.icon").arg(facesDir);

        struct passwd *current_pw;
        while ((current_pw = getpwent()) != nullptr) {

            // skip entries with uids smaller than minimum uid
            if (int(current_pw->pw_uid) < mainConfig.Users.MinimumUid.get())
                continue;

            // skip entries with uids greater than maximum uid
            if (int(current_pw->pw_uid) > mainConfig.Users.MaximumUid.get())
                continue;
            // skip entries with user names in the hide users list
            if (mainConfig.Users.HideUsers.get().contains(QString::fromLocal8Bit(current_pw->pw_name)))
                continue;

            // skip entries with shells in the hide shells list
            if (mainConfig.Users.HideShells.get().contains(QString::fromLocal8Bit(current_pw->pw_shell)))
                continue;

            // create user
            UserPtr user { new User() };
            user->name = QString::fromLocal8Bit(current_pw->pw_name);
            user->realName = QString::fromLocal8Bit(current_pw->pw_gecos).split(QLatin1Char(',')).first();
            user->homeDir = QString::fromLocal8Bit(current_pw->pw_dir);
            user->uid = int(current_pw->pw_uid);
            user->gid = int(current_pw->pw_gid);
            // if shadow is used pw_passwd will be 'x' nevertheless, so this
            // will always be true
            user->needsPassword = strcmp(current_pw->pw_passwd, "") != 0;

            // search for face icon
            user->icon = defaultFace;

            // add user
            d->users << user;
        }

        endpwent();

        // sort users by username
        std::sort(d->users.begin(), d->users.end(), [&](const UserPtr &u1, const UserPtr &u2) { return u1->name < u2->name; });

        bool avatarsEnabled = mainConfig.Theme.EnableAvatars.get();
        if (avatarsEnabled && mainConfig.Theme.EnableAvatars.isDefault()) {
            if (d->users.count() > mainConfig.Theme.DisableAvatarsThreshold.get()) avatarsEnabled=false;
        }

        // find out index of the last user
        for (int i = 0; i < d->users.size(); ++i) {
            UserPtr user { d->users.at(i) };
            if (user->name == stateConfig.Last.User.get())
                d->lastIndex = i;

            if (avatarsEnabled) {
                const QString userFace = QStringLiteral("%1/.face.icon").arg(user->homeDir);
                const QString systemFace = QStringLiteral("%1/%2.face.icon").arg(facesDir).arg(user->name);

                if (QFile::exists(userFace))
                    user->icon = userFace;
                else if (QFile::exists(systemFace))
                    user->icon = systemFace;
            }
        }
    }

    UserModel::~UserModel() {
        delete d;
    }

    QHash<int, QByteArray> UserModel::roleNames() const {
        // set role names
        QHash<int, QByteArray> roleNames;
        roleNames[NameRole] = QByteArrayLiteral("name");
        roleNames[RealNameRole] = QByteArrayLiteral("realName");
        roleNames[HomeDirRole] = QByteArrayLiteral("homeDir");
        roleNames[IconRole] = QByteArrayLiteral("icon");
        roleNames[NeedsPasswordRole] = QByteArrayLiteral("needsPassword");

        return roleNames;
    }

    const int UserModel::lastIndex() const {
        return d->lastIndex;
    }

    QString UserModel::lastUser() const {
        return stateConfig.Last.User.get();
    }

    int UserModel::rowCount(const QModelIndex &parent) const {
        return d->users.length();
    }

    QVariant UserModel::data(const QModelIndex &index, int role) const {
        if (index.row() < 0 || index.row() > d->users.count())
            return QVariant();

        // get user
        UserPtr user = d->users[index.row()];

        // return correct value
        if (role == NameRole)
            return user->name;
        else if (role == RealNameRole)
            return user->realName;
        else if (role == HomeDirRole)
            return user->homeDir;
        else if (role == IconRole)
            return user->icon;
        else if (role == NeedsPasswordRole)
            return user->needsPassword;

        // return empty value
        return QVariant();
    }

    int UserModel::disableAvatarsThreshold() const {
        return mainConfig.Theme.DisableAvatarsThreshold.get();
    }
}
