#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
//#include <QCheckBox>
#include <QThread>



MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  connect(&WotSettings, SIGNAL(NoSetting()),SLOT(ShowSettings()));
  connect(ui->settings,SIGNAL(triggered()),SLOT(ShowSettings()));
  connect(ui->tableWidget,
	  SIGNAL(cellDoubleClicked(int,int)),
	  SLOT(FindReplayFrom1(int,int)) );
  connect(ui->tableWidget_2,
	  SIGNAL(cellDoubleClicked(int,int)),
	  SLOT(FindReplayFrom2(int,int)) );

  connect(ui->RefreshButton,SIGNAL(clicked()),SLOT(Refresh()) );

  SetWidget=0;
  ReplayInfoWidget=0;
  WotSettings.Init();

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


void MainWindow::FindReplayFrom1( int row, int column )
{
  column=0;
  FindReplay(row, ui->tableWidget);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void MainWindow::FindReplayFrom2( int row, int column )
{
  column=0;
  FindReplay(row, ui->tableWidget_2);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void MainWindow::FindReplay(int row,
			    QTableWidget *tempTable)
{
  unsigned int k;
  QWidget *widg = tempTable->cellWidget(row,0);
  if (widg)
  {
    QCheckBoxPath *tempCheckBox = (QCheckBoxPath *)(widg);
    if (tempCheckBox)
    {
      k= tempCheckBox->getIndex();
    }
  }
  if (tempTable == ui->tableWidget)
  {
      qDebug() <<WotReplayPath[k] ;
      ShowReplayInfo(WotReplayMap[k], WotReplayPath[k]);
  }
  else if (tempTable == ui->tableWidget_2)
  {
      qDebug() <<FavoritePath[k] ;
      ShowReplayInfo(FavoriteMap[k], FavoritePath[k]);
  }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void MainWindow::ShowReplayInfo(InfoWotReplay *tempReplay,QString  &replayPath )
{
  if(ReplayInfoWidget)
    delete ReplayInfoWidget;
  ReplayInfoWidget = new ReplayInfo(tempReplay, replayPath, this);
  ReplayInfoWidget->show();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void MainWindow::ShowSettings()
{
  if(SetWidget)
    delete SetWidget;
  SetWidget= new SettingWidget(WotSettings,this );
  SetWidget->show();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int MainWindow::Refresh()
{
  if(WotSettings.FavoriteDir.absolutePath().toAscii()=="" ||
     WotSettings.ReplayDir.absolutePath().toAscii()=="")
  {
    ShowSettings();
    return 0;
  }

  int ReplaysCount=0;

  ReplaysCount+= CheckReplays(WotSettings.ReplayDir, WotReplayPath);
  if( WotSettings.ReplayDir != WotSettings.FavoriteDir )
    ReplaysCount+= CheckReplays(WotSettings.FavoriteDir, FavoritePath);

  if(ReplaysCount)
    ui->progressBar->setMaximum(ReplaysCount);

  if(!WotReplayPath.isEmpty())
    ParseReplays(WotReplayPath,
		 WotReplayMap,
		 //WotReplayBox,
		 ui->tableWidget);

  if(!FavoritePath.isEmpty())
    ParseReplays(FavoritePath,
		 FavoriteMap,
		 //FavoriteBox,
		 ui->tableWidget_2);
  //ui->tableWidget->selectedItems()
  return 0;

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



int MainWindow::CheckReplays(QDir &tempDir,
			     QMap <unsigned int, QString> &tempPath)
{

  QStringList filters;
      filters << "*.wotreplay";
  QStringList listFiles =
	      tempDir.entryList(filters,QDir::Files);

  QString tempString;

  unsigned int i=0;
  foreach (QString file, listFiles)
  {
    tempString=tempDir.absoluteFilePath(file);
    tempPath[i++]=tempString;
  }
  return tempPath.count();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


void MainWindow::ParseReplays(QMap <unsigned int, QString> &tempPath,
			      QMap<unsigned int, InfoWotReplay *> &tempMap,
			      QTableWidget *tempTable )
{

  InfoWotReplay *tempReplay;

  int i=0;
  tempTable->setRowCount(tempPath.count());

  //QThread  thread1, thread2;
//  ReadWotReplay *pReadWot;
  for(i=0; i < tempPath.count(); i++)
  {
    //qDebug() <<dir+file ;
    //    qDebug() <<"reded" ;
    tempReplay = new InfoWotReplay;

//    pReadWot = new ReadWotReplay;
//    PreadRep.append( pReadWot);

    WotReplay.Init(tempPath[i], *tempReplay);
    if(WotReplay.ReadReplay()==2)
    {
      int k= ui->progressBar->value()+1;
      ui->progressBar->setValue( k);
      continue;
    }

//    pReadWot->Init(tempPath[i], *tempReplay);
//    pReadWot->start();
    tempMap[i]=tempReplay;
    //tempReplay->Parse();

//  }
//  for(i=0; i < tempMap.count(); i++)
//  {

    tempTable->setCellWidget(i, 0, new QCheckBoxPath  );
    QWidget *widg = tempTable->cellWidget(i,0);
    if (widg)
    {
      QCheckBoxPath *tempCheckBox = (QCheckBoxPath *)(widg);
      if (tempCheckBox)
      {
	tempCheckBox->setIndex(i);
	//	qDebug()<< tempCheckBox->getIndex();
      }
    }

    QModelIndex playerName =   tempTable->model()->index(i, 1, QModelIndex());
    tempTable->model()->setData(playerName, QVariant(tempMap[i]->playerName));


    QModelIndex map =   tempTable->model()->index(i, 2, QModelIndex());
    tempTable->model()->setData(map, QVariant(tempMap[i]->mapName));

    QModelIndex nation =   tempTable->model()->index(i, 3, QModelIndex());
    tempTable->model()->setData(nation, QVariant(tempMap[i]->Nation));

    QModelIndex playerVehicle =   tempTable->model()->index(i, 4, QModelIndex());
    tempTable->model()->setData(playerVehicle, QVariant(tempMap[i]->Tank));

    QModelIndex isWinner = tempTable->model()->index(i, 5, QModelIndex());

    if(tempMap[i]->isSecondTable)
    {
      if(tempMap[i]->EndInfo.isWinner==1)
	tempTable->model()->setData(isWinner, QVariant(QString().fromUtf8("Победа") ));
      else if (tempMap[i]->EndInfo.isWinner==0)
	tempTable->model()->setData(isWinner, QVariant(QString().fromUtf8("Поражение")));
      else
	tempTable->model()->setData(isWinner,
				    QVariant(QString().fromUtf8("Поражение. Захват флага")));

      QModelIndex killed =   tempTable->model()->index(i, 6, QModelIndex());
      tempTable->model()->setData(killed, QVariant(tempMap[i]->EndInfo.killed.count()));

      QModelIndex xp =   tempTable->model()->index(i, 7, QModelIndex());
      tempTable->model()->setData(xp, QVariant(tempMap[i]->EndInfo.xp));

      QModelIndex credits =   tempTable->model()->index(i, 8, QModelIndex());
      tempTable->model()->setData(credits, QVariant(tempMap[i]->EndInfo.credits));


      QModelIndex Mclass =   tempTable->model()->index(i, 9, QModelIndex());
      switch (tempMap[i]->EndInfo.markOfMastery)
      {
	case(0):
	  tempTable->model()->setData(Mclass, QVariant(QString().fromUtf8("Нету")));
	  break;
	case(1):
	  tempTable->model()->setData(Mclass, QVariant(QString().fromUtf8("3-ая степень")));
	  break;
	case(2):
	  tempTable->model()->setData(Mclass, QVariant(QString().fromUtf8("2-ая степень")));
	  break;
	case(3):
	  tempTable->model()->setData(Mclass, QVariant(QString().fromUtf8("1-ая степень")));
	  break;
	case(4):
	  tempTable->model()->setData(Mclass, QVariant(QString().fromUtf8("\"Мастер\"")));
	  break;
      }
      //if(tempMap[i]->EndInfo.credits>100000 || tempMap[i]->EndInfo.xp >5000 || tempMap[i]->EndInfo.xp ==0  )

    }
    else
    {
      tempTable->model()->setData(isWinner, QVariant(QString().fromUtf8("Выход из боя")));
    }

    int k= ui->progressBar->value()+1;
    ui->progressBar->setValue( k);
  }//end for

  tempTable->resizeColumnsToContents();
  tempTable->resizeRowsToContents();

}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


MainWindow::~MainWindow()
{
  delete ui;
  if(SetWidget)
    delete SetWidget;
  if(ReplayInfoWidget)
    delete ReplayInfoWidget;
}
