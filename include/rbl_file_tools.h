#ifndef RBL_FILE_TOOLS_H
#define RBL_FILE_TOOLS_H

#include <QByteArray>
#include <QFileInfoList>
#include <QString>

namespace RFileTools
{

    //! Read binary file.
    bool readBinaryFile(const QString &fileName, QByteArray &byteArray);

    //! Write binary file.
    bool writeBinaryFile(const QString &fileName, const QByteArray &byteArray);

    //! Write binary file atomically (write to a temporary file and rename).
    //! On failure the previous file content is left untouched.
    bool writeBinaryFileAtomic(const QString &fileName, const QByteArray &byteArray);

    //! Read ascii file.
    bool readAsciiFile(const QString &fileName, QByteArray &byteArray);

    //! Read ascii file.
    bool writeAsciiFile(const QString &fileName, const QByteArray &byteArray);

    //! List files in directory.
    QFileInfoList listFiles(const QString &directoryPath);

};

#endif // RBL_FILE_TOOLS_H
