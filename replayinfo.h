#ifndef REPLAYINFO_H
#define REPLAYINFO_H

#include "infowotreplay.h"
#include <QDialog>
#include <QProcess>

namespace Ui {
  class ReplayInfo;
}

class ReplayInfo : public QDialog
{
  Q_OBJECT
  
public:
  explicit ReplayInfo(InfoWotReplay *tempReplay, QString path, QWidget *parent = 0);
  ~ReplayInfo();

private slots:
  void ViewReplay();

private:
  Ui::ReplayInfo *ui;

  void SetFirstTable(QMap <unsigned int, vehicle >  &tempMap);


  QProcess *wot;
  QString replayPath;

};

#endif // REPLAYINFO_H
