#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H
#include "settings.h"
#include <QDialog>

namespace Ui {
  class SettingWidget;
}

class SettingWidget : public QDialog
{
  Q_OBJECT
  
public:
  explicit SettingWidget(Settings &tempSet,QWidget *parent = 0 );
  ~SettingWidget();
  
private:
  Ui::SettingWidget *ui;
  Settings *WotSet;


private slots:
  void SetReplayDir();
  void SetFavoriteDir();
  void SetPathToWot();
  void RecursiveCatalogChanged();
  void SaveSettings();




};

#endif // SETTINGWIDGET_H
