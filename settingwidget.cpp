#include "settingwidget.h"
#include "ui_settingwidget.h"
#include <QFileDialog>
#include <QDebug>

SettingWidget::SettingWidget(Settings &tempSet, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SettingWidget)
{
  ui->setupUi(this);
  WotSet=&tempSet;

  connect(ui->PathToReplaysButton,SIGNAL(clicked()),SLOT(SetReplayDir()) );
  connect(ui->PathToFavoriteButton,SIGNAL(clicked()),SLOT(SetFavoriteDir()) );
  connect(ui->PathToWotButton,SIGNAL(clicked()),SLOT(SetPathToWot()) );
  connect(ui->RecursiveCatalog,SIGNAL(clicked()),SLOT(RecursiveCatalogChanged()));

  connect(ui->buttonBox,SIGNAL(accepted()), SLOT(SaveSettings()) );


  //connect(SIGNAL(destroyed()),SLOT(close()));

  ui->PathToReplays->setText(WotSet->ReplayDir.absolutePath().toAscii());
  ui->PathToFavorite->setText(WotSet->FavoriteDir.absolutePath().toAscii());
  ui->PathToWot->setText(WotSet->PathToWOT);

  if(!WotSet->PathToWOT.isEmpty())
    ui->RecursiveCatalog->setChecked(WotSet->RecursivDir );
  else
    WotSet->RecursivDir=ui->RecursiveCatalog->isChecked();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

SettingWidget::~SettingWidget()
{
  delete ui;
  //qDebug()<< "~SettingWidget" ;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void SettingWidget::SaveSettings()
{
  WotSet->Save();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void SettingWidget::SetReplayDir()
{
  QString temp=QFileDialog::getExistingDirectory(this,
						 QString::fromUtf8
						 ("Откройте каталог с реплеями"),
						 QString::fromUtf8("~/"));
  if (!temp.isEmpty())
  {
    WotSet->ReplayDir.setPath(temp);
    ui->PathToReplays->setText(temp);
  }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void SettingWidget::SetFavoriteDir()
{
  QString temp= QFileDialog::getExistingDirectory(this,
						  QString::fromUtf8
						  ("Откройте каталог с избранным"),
						  QString::fromUtf8("~/"));
  if (!temp.isEmpty())
  {
    WotSet->FavoriteDir.setPath(temp);
    ui->PathToFavorite->setText(temp);
  }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void SettingWidget::SetPathToWot()
{
  QString temp= QFileDialog::getOpenFileName(this,
					     QString::fromUtf8
					     ("Укажите путь к WorldOfTanks.exe"),
					     QString::fromUtf8("WorldOfTanks.exe"));

  if (!temp.isEmpty())
  {
    WotSet->PathToWOT =temp;
    ui->PathToWot->setText(temp);
  }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void SettingWidget::RecursiveCatalogChanged()
{
  WotSet->RecursivDir=ui->RecursiveCatalog->isChecked();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
