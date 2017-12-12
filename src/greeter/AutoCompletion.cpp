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

#include "AutoCompletion.h"

#include "Constants.h"
#include "Configuration.h"

#include <QFile>
#include <QList>
#include <QStringBuilder>
#include <QTextStream>
#include <QStringList>

#include <memory>
#include <pwd.h>

namespace SDDM {
    class User {
    public:
        QString name;
    };

    typedef std::shared_ptr<User> UserPtr;

    class AutoCompletionPrivate {
    public:
        int lastIndex { 0 };
        QList<UserPtr> users;
    };

    AutoCompletion::AutoCompletion(QObject *parent) : QAbstractListModel(parent), d(new AutoCompletionPrivate()) {
        //UserPtr user { new User() };
    }

    AutoCompletion::~AutoCompletion() {
        delete d;
    }

    QHash<int, QByteArray> AutoCompletion::roleNames() const {
        // set role names
        QHash<int, QByteArray> roleNames;
        roleNames[NameRole] = QByteArrayLiteral("name");
        return roleNames;
    }

    int AutoCompletion::rowCount(const QModelIndex &parent) const {
        return d->users.length();
    }

    QVariant AutoCompletion::data(const QModelIndex &index, int role) const {
        if (index.row() < 0 || index.row() > d->users.count())
            return QVariant();

        // get user
        UserPtr user = d->users[index.row()];

        // return correct value
            return user->name;

        // return empty value
        return QVariant();
    }

    int AutoCompletion::initAutoCompletion() {
        makeTrie(&root);

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
           addStringToTrie(&root,current_pw->pw_name,0);
       }
       stackPointer=stack;
       *(stackPointer++)=root;
       return 0;
    }

    QString AutoCompletion::tail() {return QString::fromStdString(privateTail);}
    QString AutoCompletion::head() {return QString::fromStdString(privateHead);}
    void AutoCompletion::setTail(QString tail) {return;}

    void AutoCompletion::setHead(QString inputString) {
       QChar inputChar=inputString.at(0);

       c=inputChar.unicode();
       if (c==DEL) {
           if (privateHead<privateHeadEnd) *(--privateHeadEnd)='\0';
            if (lockFlag) {
                if (!(--lockFlag))
                    if (privateHead<privateHeadEnd) completion(*(stackPointer-1));
            }
            else {
                if (stack<stackPointer-1) stackPointer--;
                if (privateHead<privateHeadEnd) completion((*(stackPointer-1)));
                else *privateTail='\0';
                privateTailEnd=privateTail;
            }

       } else {
           *privateHeadEnd++=c;
           *privateHeadEnd='\0';
           if (lockFlag) lockFlag++;
           else {
               *stackPointer=findCharacter((*(stackPointer-1))->down,c);
               if (*stackPointer==NULL) {
                   *privateTail='\0';
                   privateTailEnd=privateTail;
                   lockFlag=1;
               } else {
                   completion(*(stackPointer++));}
           }
       }
    return;

    }

    AutoCompletion::letter* AutoCompletion::findCharacter(letter *currentLeaf, char character) {
        letter* pt;
        while(1) {
            for (pt=currentLeaf;pt != NULL;pt=pt->right) {
                if (pt->character==character) {
                    return pt;
                }
            }
            return NULL;
        }
    }

    AutoCompletion::letter* AutoCompletion::addLetterToTrie(char character, int sign) {
        letter* nodo = new letter;

        nodo->character=character;
        nodo->right=NULL;
        nodo->down=NULL;
        nodo->sign=sign;

        return nodo;
    }

    AutoCompletion::letter * AutoCompletion::readWord(letter *currentLeaf,const char *word) {
        while(1) {
           letter *nextLetter=NULL;
           letter *pt;

           for (pt=currentLeaf;pt != NULL;pt=pt->right) {
               if(pt->character==*word) {
                   nextLetter=pt;
                   break;
             }
           }

           if (nextLetter==NULL) return NULL;
           if (*word=='\0') return pt;

           currentLeaf=nextLetter->down;
           word++;
        }
    }

    void AutoCompletion::addStringToTrie(letter **root,const char *word, int sign) {
        if (NULL == *root) {qWarning("empty Trie");return;}
        letter* stringCursor=(*root)->down;
        if (stringCursor==NULL) {
            for(stringCursor=*root;*word;stringCursor=stringCursor->down) {
                stringCursor->down=addLetterToTrie(*word,sign);
                word++;
            }
            stringCursor->down=addLetterToTrie('\0',sign);
            return;
        }

        if(readWord(stringCursor, word)) qWarning("duplicate word; %s",word);

        while(*word != '\0') {
            if (*word == stringCursor->character) {
                word++;
                stringCursor=stringCursor->down;
            } else {
              break;
            }
        }

        while(stringCursor->right) {
            if(*word == stringCursor->right->character) {
                word++;
                addStringToTrie(&(stringCursor->right),word,sign);
                return;
            }
            stringCursor=stringCursor->right;
        }

        stringCursor->right=addLetterToTrie(*word,sign);
        if(!(*word)) return;
        word++;

        for(stringCursor=stringCursor->right; *word; stringCursor=stringCursor->down) {
            stringCursor->down=addLetterToTrie(*word,sign);
            word++;
        }

        stringCursor->down=addLetterToTrie('\0',sign);
        return;
    }

    void AutoCompletion::makeTrie(letter** root) {
        *root = addLetterToTrie('\0',0);
    }

    void AutoCompletion::completion(letter* pt) {
        *privateTail='\0';
        privateTailEnd=privateTail;
        while (pt->down->character!='\0') {
            *privateTailEnd++=pt->down->character;
            *privateTailEnd='\0';
            pt=pt->down;
        }
    }
}

