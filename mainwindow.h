#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "readwotreplay.h"

#include "settingwidget.h"
#include <QTableWidget>
#include "qcheckboxpath.h"
#include "replayinfo.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();


public slots:



private slots:
  int Refresh();
  int CheckReplays(QDir &tempDir,
		   QMap <unsigned int, QString> &tempPath);
  void ParseReplays(QMap <unsigned int, QString> &tempPath,
		    QMap <unsigned int, InfoWotReplay*> &tempMap,
		    QTableWidget *tempTable);

  void ShowSettings();
  void ShowReplayInfo(InfoWotReplay *tempReplay, QString &replayPath);
  void FindReplayFrom1( int row, int column );
  void FindReplayFrom2( int row, int column );

  void FindReplay(int row,
		  QTableWidget *tempTable);

private:
    /// GUI
  Ui::MainWindow *ui;
  SettingWidget *SetWidget;
  ReplayInfo *ReplayInfoWidget;
  Settings  WotSettings;


  QList <ReadWotReplay*> PreadRep;
  QMap <unsigned int, InfoWotReplay*> WotReplayMap, FavoriteMap;
  QMap <unsigned int, QString> WotReplayPath, FavoritePath;
  //QMap <unsigned int, QCheckBoxPath*> WotReplayBox, FavoriteBox;

  ReadWotReplay WotReplay;


};

#endif // MAINWINDOW_H
