#ifndef QTINTERFACE_FILESERVICE_H
#define QTINTERFACE_FILESERVICE_H

#include <QMessageBox>
#include <QFileDialog>

#include "../GenericInterface.h"
#include "WindowService.h"

namespace genericinterface
{
  class FileService : public QObject, public Service
  {
  Q_OBJECT
  public:
    void connect (GenericInterface* gi);

    void display (GenericInterface* gi);

  private slots:
    void chooseFile();

  signals:
    void fileChosen(const QString& path);

  private:
    QAction* _open;
    GenericInterface* _gi;
  };
}

#endif