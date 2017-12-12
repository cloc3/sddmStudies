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

    /*AutoCompletion::AutoCompletion(QObject *parent) : QAbstractListModel(parent) , d(new SDDM::UserModel::UserModelPrivate()){

    }*/

    /*AutoCompletion::~AutoCompletion() {
        delete d;
    }*/

    AutoCompletion::letter* AutoCompletion::findCharacter(letter *currentLeaf, char character) {
        letter* pt;
        while(1) {
            for (pt=currentLeaf;pt != NULL;pt=pt->dx) {
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
        nodo->dx=NULL;
        nodo->down=NULL;
        nodo->sign=sign;

        return nodo;
    }

    AutoCompletion::letter * AutoCompletion::readWord(letter *currentLeaf,const char *word) {
        while(1) {
           letter *nextLetter=NULL;
           letter *pt;

           for (pt=currentLeaf;pt != NULL;pt=pt->dx) {
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

        while(stringCursor->dx) {
            if(*word == stringCursor->dx->character) {
                word++;
                addStringToTrie(&(stringCursor->dx),word,sign);
                return;
            }
            stringCursor=stringCursor->dx;
        }

        stringCursor->dx=addLetterToTrie(*word,sign);
        if(!(*word)) return;
        word++;

        for(stringCursor=stringCursor->dx; *word; stringCursor=stringCursor->down) {
            stringCursor->down=addLetterToTrie(*word,sign);
            word++;
        }

        stringCursor->down=addLetterToTrie('\0',sign);
        return;
    }

    void AutoCompletion::makeTrie(letter** root) {
        *root = addLetterToTrie('\0',0);
    }

    int AutoCompletion::initAutoCompletion() {
        d=new SDDM::AutoCompletion

        makeTrie(&root);

       for (int i=0;i<d->users.size();i++) {
           const char* username=d->users.at(i)->name.toStdString().c_str();
           addStringToTrie(&root,username,0);
       }
       stackPointer=stack;
       *(stackPointer++)=root;
       bufferEnd=buffer;
       return 0;
    }

    void AutoCompletion::completion(letter* pt) {
        privateTail.clear();
        while (pt->down->character!='\0') {
            privateTail+=QStringLiteral("%1").arg(pt->down->character);
            pt=pt->down;
        }
    }

    QString AutoCompletion::tail() {return privateTail;}
    QString AutoCompletion::head() {return privateHead;}
    void AutoCompletion::setTail(QString tail) {return;}
    void AutoCompletion::setHead(QString inputString) {
      QChar inputChar=inputString.at(0);

       c=inputChar.unicode();
       if (c==DEL) {
           if (buffer<bufferEnd) *(--bufferEnd)='\0';
           privateHead=QString::fromStdString(buffer);
            if (lockFlag) {
                if (!(--lockFlag))
                    if (buffer<bufferEnd) completion(*(stackPointer-1));
            }
            else {
                if (stack<stackPointer-1) stackPointer--;
                if (buffer<bufferEnd) completion((*(stackPointer-1)));
                else privateTail.clear();
            }

       } else {
           *bufferEnd++=c;
           *bufferEnd='\0';
           privateHead=QString::fromStdString(buffer);
           if (lockFlag) lockFlag++;
           else {
               *stackPointer=findCharacter((*(stackPointer-1))->down,c);
               if (*stackPointer==NULL) {
                   privateTail.clear();
                   lockFlag=1;
               } else {
                   completion(*(stackPointer++));}
           }
       }
    return;

    }
}
