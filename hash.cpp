#include "hash.h"

QString myhash(const QString& utf8){
    return QString(QCryptographicHash::hash(
                       utf8.toUtf8()
                       ,QCryptographicHash::Sha3_512
                   ).toHex()
                  );
}
