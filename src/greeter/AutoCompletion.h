/***************************************************************************
* Copyright (c) 2016 Marco Clocchiatti <ziapannocchia@gmail.com>
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

#ifndef AUTOCOMPLETION_H
#define AUTOCOMPLETION_H

#include <QAbstractListModel>

#define BUF_SIZE 200
#define DEL '\b'

namespace SDDM {
    class AutoCompletionPrivate;

    class AutoCompletion : public QAbstractListModel {
        Q_OBJECT
        Q_DISABLE_COPY(AutoCompletion)

        Q_PROPERTY(int initAutoCompletion READ initAutoCompletion CONSTANT)
        Q_PROPERTY(QString tail READ tail WRITE setTail)
        Q_PROPERTY(QString head READ head WRITE setHead)

    public:
        enum UserRoles {
            NameRole = Qt::UserRole + 1
        };
        AutoCompletion(QObject *parent = 0);
        ~AutoCompletion();

        QHash<int, QByteArray> roleNames() const override;

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;

        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

        typedef struct letter {
             char character;
             int sign;
             letter* right;
             letter* down;
         } letter;

        QString head();
        void setHead(QString);
        QString tail();
        void setTail(QString);

        int initAutoCompletion();
        void makeTrie(letter**);
        void addStringToTrie(letter**,const char*,int);
        letter* findCharacter(letter*,char);
        void completion(letter*);
        letter* readWord(letter *root,const char *word);
        letter*	addLetterToTrie(char, int);

    private:
        AutoCompletionPrivate *d { nullptr };

        char privateHead[BUF_SIZE];
        char* privateHeadEnd=privateHead;
        char privateTail[BUF_SIZE];
        char* privateTailEnd=privateTail;
        char c='\0';
        int lockFlag=0;
        letter *root;
        letter* stack[BUF_SIZE];
        letter** stackPointer;
    };
}

#endif // AUTOCOMPLETION_H
