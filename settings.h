#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QDir>

class Settings : public QObject
{
  Q_OBJECT
public:
  explicit Settings(QObject *parent = 0);
  
signals:

public:
  QDir ReplayDir, FavoriteDir;
  QString PathToWOT;
  bool RecursivDir;

#ifdef Q_OS_LINUX
  int CreateSH();
#endif

signals:
  void NoSetting();


public slots:
  int Save();
  void Init();

private:
  QString FileName;




  
};

#endif // SETTINGS_H
