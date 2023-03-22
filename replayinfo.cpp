#include "replayinfo.h"
#include "ui_replayinfo.h"
#include <QDebug>

ReplayInfo::ReplayInfo(InfoWotReplay *tempReplay, QString path, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ReplayInfo)
{
  ui->setupUi(this);
  wot=0;
  replayPath=path;
  connect(ui->WatchReplayButton,SIGNAL(clicked()),SLOT(ViewReplay()));
  connect(ui->CloseButton, SIGNAL(clicked()), SLOT(close()));
  //connect(ui->)
  //QMap <unsigned int, vehicle >  * Temp;

  if(tempReplay->isSecondTable==1)
  {
    QString isWinner;
    if(tempReplay->EndInfo.isWinner==1)
      isWinner = QString().fromUtf8("Победа");
    else if (tempReplay->EndInfo.isWinner==0)
      isWinner = QString().fromUtf8("Поражение");
    else
      isWinner = QString().fromUtf8("Поражение. Захват флага");


    ui->IsWinLabel->setText(isWinner);
    ui->IsWinLabel_2->setText(isWinner);
    ui->IsWinLabel_3->setText(isWinner);


    tempReplay->ParseVehicles(tempReplay->EndVihiles);
    SetFirstTable(tempReplay->EndVihiles);
    ui->Credits->setText(QString::number(tempReplay->EndInfo.credits));
    ui->Repair->setText(QString::number(tempReplay->EndInfo.repair));
    ui->Exp->setText(QString::number(tempReplay->EndInfo.xp));

    if (tempReplay->EndInfo.killerID)
    {
  //    if (tempReplay->EndVihiles[tempReplay->EndInfo.killerID].clanAbbrev.isEmpty())
	ui->KilledByLabel->setText(tempReplay->EndVihiles[tempReplay->EndInfo.killerID].Nick
				   +" ("+ tempReplay->EndVihiles[tempReplay->EndInfo.killerID].Tank + ")");
    }

    //second table
    ui->ShotsGet->setText(QString::number(tempReplay->EndInfo.shotsReceived));
    ui->ShotsToEnemy->setText(QString::number(tempReplay->EndInfo.hits));
    ui->ShotsLabel->setText(QString::number(tempReplay->EndInfo.shots));
    ui->GetCPoint->setText(QString::number(tempReplay->EndInfo.capturePoints));
    ui->DropCP->setText(QString::number(tempReplay->EndInfo.droppedCapturePoints));

    int i,j;
    if(!tempReplay->EndInfo.killed.isEmpty())
    {
      for( i=0; i < tempReplay->EndInfo.killed.count(); i++)
      {
	ui->KilledList->addItem(tempReplay->EndVihiles[tempReplay->EndInfo.killed[i] ].Nick
	    +" ("+ tempReplay->EndVihiles[tempReplay->EndInfo.killed[i] ].Tank + ")");
	for( j=0; j < tempReplay->EndInfo.damaged.count(); j++)
	  if (tempReplay->EndInfo.damaged[j] == tempReplay->EndInfo.killed[i])
	    tempReplay->EndInfo.damaged.removeAt(j);
      }
      if(!tempReplay->EndInfo.damaged.isEmpty())
	for( i=0; i < tempReplay->EndInfo.damaged.count(); i++)
	  ui->DamagedList->addItem(tempReplay->EndVihiles[tempReplay->EndInfo.damaged[i] ].Nick
				   +" ("+ tempReplay->EndVihiles[tempReplay->EndInfo.damaged[i] ].Tank + ")");
    }
    if(!tempReplay->EndInfo.spotted.isEmpty())
      for( i=0; i < tempReplay->EndInfo.spotted.count(); i++)
	ui->DiscovedList->addItem(tempReplay->EndVihiles[tempReplay->EndInfo.spotted[i] ].Nick
	    +" ("+ tempReplay->EndVihiles[tempReplay->EndInfo.spotted[i] ].Tank + ")");
  }
  else
  {
    tempReplay->ParseVehicles(tempReplay->StartVehicles);
    SetFirstTable(tempReplay->StartVehicles);

  }

  //down
  ui->NickLabel->setText(tempReplay->Nick);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ReplayInfo::SetFirstTable(QMap <unsigned int, vehicle >  &tempMap)
{
  QMap<unsigned int, vehicle>::iterator i;

  int myTeam,enemyTeam;
  myTeam=enemyTeam=0;


  for (i = tempMap.begin(); i != tempMap.end(); ++i)
  {
    if( tempMap[i.key()].team==1 )
    {
      ui->TeamMyTable->setRowCount(myTeam+1);

      QModelIndex playerName =   ui->TeamMyTable->model()->index(myTeam, 0, QModelIndex());
      ui->TeamMyTable->model()->setData(playerName, QVariant(tempMap[i.key()].Nick ));

      QModelIndex tank =   ui->TeamMyTable->model()->index(myTeam, 1, QModelIndex());
      ui->TeamMyTable->model()->setData(tank, QVariant(tempMap[i.key()].Tank ));

      QModelIndex isAlive =   ui->TeamMyTable->model()->index(myTeam, 2, QModelIndex());
      if(tempMap[i.key()].isAlive)
	ui->TeamMyTable->model()->setData(isAlive, QVariant(QString().fromUtf8("Выжил")));
      else
	ui->TeamMyTable->model()->setData(isAlive, QVariant(QString().fromUtf8("Уничтожен")));

      QModelIndex frags =   ui->TeamMyTable->model()->index(myTeam, 3, QModelIndex());
      ui->TeamMyTable->model()->setData(frags, QVariant(tempMap[i.key()].frags));

      myTeam++;
    }
    else
    {
      ui->TeamEnemesTable->setRowCount(enemyTeam+1);

      QModelIndex playerName =   ui->TeamEnemesTable->model()->index(enemyTeam, 0, QModelIndex());
      ui->TeamEnemesTable->model()->setData(playerName, QVariant(tempMap[i.key()].Nick ));

      QModelIndex tank =   ui->TeamEnemesTable->model()->index(enemyTeam, 1, QModelIndex());
      ui->TeamEnemesTable->model()->setData(tank, QVariant(tempMap[i.key()].Tank ));

      QModelIndex isAlive =   ui->TeamEnemesTable->model()->index(enemyTeam, 2, QModelIndex());
      if(tempMap[i.key()].isAlive)
	ui->TeamEnemesTable->model()->setData(isAlive, QVariant(QString().fromUtf8("Выжил")));
      else
	ui->TeamEnemesTable->model()->setData(isAlive, QVariant(QString().fromUtf8("Уничтожен")));

      QModelIndex frags =   ui->TeamEnemesTable->model()->index(enemyTeam, 3, QModelIndex());
      ui->TeamEnemesTable->model()->setData(frags, QVariant(tempMap[i.key()].frags));

      enemyTeam++;
    }
  }
  ui->TeamEnemesTable->resizeColumnsToContents();
  ui->TeamEnemesTable->resizeRowsToContents();
  ui->TeamMyTable->resizeColumnsToContents();
  ui->TeamMyTable->resizeRowsToContents();

  ui->TeamMyTable->sortItems(2);
  ui->TeamEnemesTable->sortItems(2);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ReplayInfo::ViewReplay()
{
#ifdef Q_OS_LINUX
  system(QApplication::applicationDirPath().toAscii() + "/wotreplay.sh "
	 + replayPath.toAscii());
#endif

#ifdef Q_OS_WIN
    wot= new QProcess;
    wot->start("explorer " + replayPath.toAscii());
#endif
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ReplayInfo::~ReplayInfo()
{
  delete ui;
#ifdef Q_OS_WIN
  if(wot)
    delete wot;
#endif
}
