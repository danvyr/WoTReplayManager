#include "readwotreplay.h"
#include <QDebug>
#include <QFile>
#include <iostream>
#include <fstream>


using namespace std;

const unsigned int MAX = 15000;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ReadWotReplay::ReadWotReplay(QObject *parent) :
  QThread(parent)
{

}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ReadWotReplay::Init(QString tFileName, InfoWotReplay &tempRep)
{
  FileName=tFileName;
  WotReplay= &tempRep;
  WotReplay->Clear();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ReadWotReplay::run()
{
  ReadReplay();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int ReadWotReplay::ReadReplay()
{
  Debug=0;

//  if(FileName=="/home/daniel/World_of_Tanks_07/replays/20120408_1447_france-AMX_13_75_monastery.wotreplay")
//  {
//    Debug=1;
//  }

  char    mas[MAX];
  fstream  ioFile(FileName.toAscii(), ios::in | ios::out);

  ioFile.read((char*) mas,sizeof(mas) ); //читаем массив из файла
  ioFile.close();

  //12 32 34 11
  if( !(mas[0]==0x12 &&mas[1]==0x32 && mas[2]==0x34 && mas[3]==0x11))
    return 2 ;

  unsigned int i;
  isTypeReaded=0; isFirstTable=0; isReplayEnded=0;
  isSecondTable=0;
  json1 tempjson;
  fs=0; s=0; ks=0;
  QList <json1> replay;

  unsigned int maxP=MAX;

//  if(Debug)
//    qDebug() <<"start" ;
  char c;

  for(i = 12; i < maxP; i++)
  {
    c=mas[i];
    switch(mas[i])
    {
      case('{'):
	fs=fs+1;
	break;
      case(','):
//	if(Debug)
//	  qDebug() <<"case(','):" ;
	replay.append(tempjson);
	tempjson.type.clear();
	tempjson.value.clear();
	isTypeReaded=0;
	break;
      case('['):
//	if(Debug)
//	  qDebug() <<"case('['):" ;
	isReplayEnded=1;
	ks=1;
	if(mas[i+1]=='{' || mas[i+2]=='"')
	{//запуск парсера второй части
	  fs=0;
	  isTypeReaded=0;
	  ReadSecondPart(mas, i+1);
	}
	i=maxP;
	break;
      case(']'):
//	if(Debug)
//	  qDebug() <<"case(']'):" ;
	ks=ks-1;
	break;
      case('"'):
//	if(Debug)
//	  qDebug() <<"case(skobka):" ;
	if (s>0)
	  s--;
	else
	  s++;

	if (fs>1)
	{
	  tempjson.type.clear();
	  tempjson.value.clear();
	  isTypeReaded=0;
	  i=ReadVehicles(mas,i, WotReplay->StartVehicles);
	  continue;
	}
	break;
      case('}'):
//	if(Debug)
//	  qDebug() <<"case(}):" ;
	fs=fs-1;
	if (fs==0)
	{
	  isFirstTable=1;
	  for(unsigned int qq = i+1; qq < i+5; qq++)
	    mas[qq]=32;
	  maxP=i+6;
	  replay.append(tempjson);
	  //запуск парсера реплея
	  ParseFirst(replay);
	  tempjson.type.clear();
	  tempjson.value.clear();
	  isTypeReaded=0;
	}
	break;
      case(':'):
//	if(Debug)
//	  qDebug() <<"case(:):" ;
	isTypeReaded=1;
	break;

      case(' '):
	continue;
      break;
      default:
	if(isTypeReaded==0)
	  tempjson.type.append(mas[i]);
	else
	  tempjson.value.append(mas[i]);
	break;
    }
  }

//  if(Debug)
//    qDebug() <<"end" ;


  if(isSecondTable==0)
    WotReplay->isSecondTable=0;
  else
    (WotReplay->isSecondTable=1);
  WotReplay->Parse();

  return 0;
}



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


void ReadWotReplay::ParseFirst(QList <json1> &tempjson)
{

  for(int j=0; j<tempjson.count(); j++)
  {
    if(tempjson[j].type.toLower()=="mapname")
	  WotReplay->mapName=tempjson[j].value;
    else if (tempjson[j].type.toLower()=="playerid")
      WotReplay->playerID=tempjson[j].value.toUInt();
    else if (tempjson[j].type.toLower()=="playervehicle")
      WotReplay->playerVehicle=tempjson[j].value;
    else if (tempjson[j].type.toLower()=="gameplaytype")
      WotReplay->gameplayType=tempjson[j].value;
    else if (tempjson[j].type.toLower()=="playername")
      WotReplay->playerName=tempjson[j].value;
    else if (tempjson[j].type.toLower()=="mapdisplayname")
      WotReplay->mapDisplayName= tempjson[j].value;
    else if (tempjson[j].type.toLower()=="datetime")
      WotReplay->dateTime=tempjson[j].value;

  }

}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



unsigned int ReadWotReplay::ReadVehicles(char str[],
					 unsigned int pos,
					 QMap <unsigned int, vehicle> &tanks)
{
//  if(Debug)
//    qDebug() <<"ReadVehicles" ;
  unsigned int i;
  isTypeReaded=0;
  bool isIdReaded=0;

  json1 tempjson;
  QString tempID;
  QList <json1> vihicles;

  for(i = pos+1; i < MAX; i++)
  {
    switch(str[i])
    {
      case('{'):
	fs=fs+1;
	break;
      case('"'):
	if (s>0)
	  s--;
	else
	  s++;
	if (fs>1)
	break;
      case('['):
	isReplayEnded=1;
	ks=ks+1;
	break;
      case(']'):
	ks=ks-1;
	break;
      case('}'):
	fs=fs-1;
	if( fs==1 && isFirstTable==0)
	{
	  isTypeReaded=0;
	  vihicles.append(tempjson);
	  ParseVihecles(tempID, vihicles,tanks);
	  vihicles.clear();
	  tempID.clear();
	  tempjson.type.clear();
	  tempjson.value.clear();
	  isTypeReaded=0;
	  isIdReaded=0;
//	  if(Debug)
//	    qDebug() <<"end ReadVehicles fs=1" ;
	  return (i+1);
	}
	else if (fs==0 && isFirstTable==1)
	{
	  isTypeReaded=0;
	  vihicles.append(tempjson);
	  ParseVihecles(tempID, vihicles,tanks);
	  vihicles.clear();
	  tempID.clear();
	  tempjson.type.clear();
	  tempjson.value.clear();
	  isTypeReaded=0;
	  isIdReaded=0;

//	  if(Debug)
//	    qDebug() <<"end ReadVehicles fs==0" ;
	  return (i+1);

	}
	break;
      case(':'):
	if(isIdReaded==1)
	  isTypeReaded=1;
	isIdReaded=1;
	break;
      case(','):
	if ( (fs==2&& isFirstTable==0) || (fs==1 && isFirstTable==1))
	{//конец описания танка
	  vihicles.append(tempjson);
	  ParseVihecles(tempID, vihicles,tanks);
	  vihicles.clear();
	  tempID.clear();
	  tempjson.type.clear();
	  tempjson.value.clear();
	  isTypeReaded=0;
	  isIdReaded=0;
	}
	if(tempjson.type.count()>0)
	{ // конец параметра json
	  vihicles.append(tempjson);
	  tempjson.type.clear();
	  tempjson.value.clear();
	  if(isTypeReaded==1)
	    isTypeReaded=0;
	}
	break;
      case(' '):
	continue;
      break;
      default:
	if((fs>2 && isFirstTable==0) || (fs>1 && isFirstTable==1))
	{
	  if(isTypeReaded==0)
	    tempjson.type.append(str[i]);
	  else
	    tempjson.value.append(str[i]);
	}
	else
	  tempID.append(str[i]);
	break;
    }
  }
//  if(Debug)
//    qDebug() <<"end ReadVehicles" ;
  return i;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


unsigned int ReadWotReplay::ReadFrags(char str[],
				      unsigned int pos,
				      QMap <unsigned int, vehicle> &tanks)
{
//  if(Debug)
//    qDebug() <<"ReadFrags" ;
  unsigned int i;
  //unsigned int k=0,end = tanks.count();
  isTypeReaded=0;
  bool isIdReaded=0;

  json1 tempjson;
  QString tempID;
  QList <json1> vihicles;

  for(i = pos+1; i < MAX; i++)
  {
    switch(str[i])
    {
      case('{'):
	fs=fs+1;
	break;
      case('"'):
	if (s>0)
	  s--;
	else
	  s++;
	if (fs>1)
	break;
      case('['):
	isReplayEnded=1;
	ks=ks+1;
	break;
      case(']'):
	ks=ks-1;
	break;
      case('}'):
	fs=fs-1;
	if (fs==0)
	{
	  isTypeReaded=0;
	  vihicles.append(tempjson);
	  ParseVihecles(tempID, vihicles,tanks);
	  vihicles.clear();
	  tempID.clear();
	  tempjson.type.clear();
	  tempjson.value.clear();
	  isTypeReaded=0;
	  isIdReaded=0;
	  return i;
	}
	break;
      case(':'):
	if(isIdReaded==1)
	  isTypeReaded=1;
	isIdReaded=1;
	break;
      case(','):

	//конец описания танка
	vihicles.append(tempjson);
	ParseVihecles(tempID, vihicles,tanks);
	vihicles.clear();
	tempID.clear();
	tempjson.type.clear();
	tempjson.value.clear();
	isTypeReaded=0;
	isIdReaded=0;

	break;
      case(' '):
	continue;
      break;
      default:
	if (fs>1 && isFirstTable==1)
	{
	  if(isTypeReaded==0)
	    tempjson.type.append(str[i]);
	  else
	    tempjson.value.append(str[i]);
	}
	else
	  tempID.append(str[i]);
	break;
    }
  }
//  if(Debug)
//    qDebug() <<"end ReadFrags" ;

  return i;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


void ReadWotReplay::ParseVihecles(QString& tempID,
				  QList <json1> &vihicles,
				  QMap <unsigned int, vehicle> &tanks)
{

  ID = tempID.toUInt();
  for(int j=0; j<vihicles.count(); j++)
  {

    if(vihicles[j].type.toLower()=="frags")
	  tanks[ID].frags=vihicles[j].value.toUInt();

    else if(vihicles[j].type.toLower()=="vehicletype")
      tanks[ID].vehicleType=vihicles[j].value;

    else if(vihicles[j].type.toLower()=="isalive")
    {
      if (vihicles[j].value.toLower()=="true")
	tanks[ID].isAlive=1;
      else
	tanks[ID].isAlive=0;
    }

    else if(vihicles[j].type.toLower()=="name")
      tanks[ID].name=vihicles[j].value;

    else if(vihicles[j].type.toLower()=="clanabbrev")
      tanks[ID].clanAbbrev=vihicles[j].value;

    else if(vihicles[j].type.toLower()=="team")
      tanks[ID].team=vihicles[j].value.toUInt();

    else if(vihicles[j].type.toLower()=="events")
      tanks[ID].events=vihicles[j].value;

    else if(vihicles[j].type.toLower()=="isteamkiller")
    {
      if (vihicles[j].value.toLower()=="true")
	tanks[ID].isTeamKiller=1;
      else
	tanks[ID].isTeamKiller=0;
    }
  } //end for

} //end func






//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

unsigned int ReadWotReplay::ParseIntList(char str[],
			  unsigned int pos,
			  QList <unsigned int> &tempList)
{
//  if(Debug)
//    qDebug() <<"ParseIntList" ;
  unsigned int j=pos;
  QString tempStr;
  if (str[j+1]==']')
    return pos;

  do{
    j++;
    switch(str[j])
    {
      case('{'):
	j=ParseMap(str,j,Map)+1;
	isMapParsed=1;
	isListParsed=0;
	break;
      case(' '):
	continue;
	break;
      case('['):
	continue;
	break;
      case(']'):
	ks=ks-1;
	break;
      case('}'):
	fs=fs-1;
	break;
      case(','):
	tempList.append(tempStr.toUInt());
	tempStr.clear();
	break;
      default:
	tempStr.append(str[j]);
	break;
    }

  }
  while( str[j]!=']');
//  if(Debug)
//    qDebug() <<"end ParseIntList" ;
  tempList.append(tempStr.toUInt());
  tempStr.clear();
  return (j);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

unsigned int ReadWotReplay::ParseMap(char str[],
			  unsigned int pos,
			  QMap <QString, unsigned int> &tempMap)
{
//  if(Debug)
//    qDebug() <<"ParseMap" ;
  unsigned int j=pos;
  QString tempStr1, tempStr2;
  bool isTypeRead=0;
  char c;

  if (str[j+1]=='}')
    return pos;
  do
  {    
    j++;
    c=str[j];
    switch(str[j])
    {
      case(' '):
	continue;
	break;
      case('{'):
	continue;
	break;
      case('"'):
	if (s>0)
	{
	  s--;
	}
	else
	{
	  s++;
	}
	break;
      case('}'):
	fs=fs-1;
	if(isTypeRead==1)
	{
	  tempMap[tempStr1] = tempStr2.toUInt();
	  tempStr1.clear();
	  tempStr2.clear();
	  isTypeRead=0;
	}
	else
	  isTypeRead=1;
	break;
      case(','):
	if(isTypeRead==1)
	{
	  tempMap[tempStr1] = tempStr2.toUInt();
	  tempStr1.clear();
	  tempStr2.clear();
	  isTypeRead=0;
	}
	else
	  isTypeRead=1;
	break;
      case(':'):
	isTypeRead=1;
	break;
      case(']'):
	ks=ks-1;
	break;

      default:
	if(isTypeRead==0)
	  tempStr1.append(str[j]);
	else
	  tempStr2.append(str[j]);
	break;
    }
  }while(str[j]!='}');
//  if(Debug)
//    qDebug() <<"end ParseMap" ;
  isTypeRead=0;
  if(str[j+1]==']')
    ks=ks-1;
  return (j+1);
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ReadWotReplay::ReadSecondPart(char str[],
				   unsigned int pos)
{
//  if(Debug)
//    qDebug() <<" ReadSecondPart" ;
  json1 tempjson;
  isListParsed=0;
  isMapParsed=0;
  char c;


  for(unsigned int i = pos; i < MAX; i++)
  {
    c=str[i];
    switch(str[i])
    {
      case('{'):
	fs=fs+1;
	if (WotReplay->EndVihiles.count()>10)
	{
	   i=ReadFrags(str, i, WotReplay->EndVihiles);
	   isSecondTable=1;
	   i=MAX;
	}
	else if (isTypeReaded==1 && fs>0 && WotReplay->EndVihiles.count()==0)
	{
//	  if(Debug)
//	    qDebug() <<tempjson.type;
	  //запуск парсера map
	  i=ParseMap(str,i,Map)-1;
	  isMapParsed=1;
	}
	break;
      case('"'):
	if (s>0)
	{
	  s--;
	}
	else
	{
	  s++;
	}
	break;
      case('['):	
	ks=ks+1;
	if (isTypeReaded==1 && fs>0 && WotReplay->EndVihiles.count()==0)
	{
	  if(str[i+1]=='{')
	  {
	    fs=fs+1;
	    //запуск парсера map
	    i=ParseMap(str,i+1,Map);
	    isMapParsed=1;
	  }
	  else
	  {
	    //запуск парсера чисел
	    i=ParseIntList(str,i,List);
	    isListParsed=1;
	  }

	}
	break;
      case(']'):
	ks=ks-1;	
	break;
      case('}'):
	fs=fs-1;
	if (fs==0)
	{
	  tempjson.type.clear();
	  tempjson.value.clear();
	  isTypeReaded=0;
	  i=ReadVehicles(str, i+1, WotReplay->EndVihiles);

	}
	break;
      case(':'):
	isTypeReaded=1;
	break;
      case(','):	
	if(isTypeReaded==1 && fs>0)
	{
	  //запуск парсера значений таблицы
	  ParseSecond(tempjson);
	  isTypeReaded=0;
	  List.clear();
	  Map.clear();
	  tempjson.type.clear();
	  tempjson.value.clear();
	  isListParsed=0;
	  isMapParsed=0;
	}
	break;
      case(' '):
	continue;
      break;
      default:
	if(isTypeReaded==0)
	  tempjson.type.append(str[i]);
	else
	  tempjson.value.append(str[i]);
	break;
    }
  }
//  if(Debug)
//    qDebug() <<" end ReadSecondPart" ;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int ReadWotReplay::ParseSecond(json1 &temp)
{
  char c=temp.type.at(0).toLower().toAscii();
  switch (c)
  {
    case ('a'):
      if(temp.type.toLower()=="ammo")
      {
	WotReplay->EndInfo.ammo=List;
	List.clear();
      }
      else if(temp.type.toLower()=="arenatypeid")
      {
	WotReplay->EndInfo.arenaTypeID=temp.value.toUInt();
      }
      else if(temp.type.toLower()=="arenacreatetime")
      {
	WotReplay->EndInfo.arenaCreateTime=temp.value.toULong();
      }
      else if(temp.type.toLower()=="achieveindices")
      {
	WotReplay->EndInfo.achieveIndices=List;
	List.clear();
      }
      else if(temp.type.toLower()=="arenauniqueid")
      {
	WotReplay->EndInfo.arenaUniqueID=temp.value.toULongLong();
      }
      return 0;
      break;

    case ('c'):
      if(temp.type.toLower()=="crewactivityflags")
      {
	WotReplay->EndInfo.crewActivityFlags=List;
	List.clear();
      }
      else if(temp.type.toLower()=="creditscontributionout")
      {
	WotReplay->EndInfo.creditsContributionOut=temp.value.toUInt();
      }
      else if(temp.type.toLower()=="creditspenalty")
      {
	WotReplay->EndInfo.creditsPenalty=temp.value.toUInt();
      }
      else if(temp.type.toLower()=="creditscontributionin")
      {
	WotReplay->EndInfo.creditsContributionIn=temp.value.toUInt();
      }
      else if(temp.type.toLower()=="credits")
      {
	WotReplay->EndInfo.credits=temp.value.toUInt();
      }
      else if(temp.type.toLower()=="capturepoints")
      {
	WotReplay->EndInfo.capturePoints=temp.value.toUInt();
      }
      return 0;
      break;

    case ('d'):
      if(temp.type.toLower()=="damagedealt")
      {
	WotReplay->EndInfo.damageDealt=temp.value.toUInt();
      }
      else if(temp.type.toLower()=="droppedcapturepoints")
      {
	WotReplay->EndInfo.droppedCapturePoints=temp.value.toUInt();
      }
      else if(temp.type.toLower()=="damaged")
      {
	WotReplay->EndInfo.damaged=List;
	List.clear();
      }
      else if(temp.type.toLower()=="damagereceived")
      {
	WotReplay->EndInfo.damageReceived=temp.value.toUInt();
      }
      return 0;
      break;

    case ('e'):
      if(temp.type.toLower()=="eventindices")
      {
	WotReplay->EndInfo.eventIndices=List;
	List.clear();
      }
      else if(temp.type.toLower()=="epicachievements")
      {
	WotReplay->EndInfo.epicAchievements=List;
	List.clear();
      }
      return 0;
      break;

    case ('f'):
      if(temp.type.toLower()=="factors")
      {
	WotReplay->EndInfo.factors=Map;
	Map.clear();
      }
      else if(temp.type.toLower()=="freexp")
      {
	WotReplay->EndInfo.freeXP=temp.value.toUInt();
      }
      return 0;
      break;

    case ('h'):
      if(temp.type.toLower()=="herovehicleids")
      {
	WotReplay->EndInfo.heroVehicleIDs=List;
	List.clear();
      }
      else if(temp.type.toLower()=="health")
      {
	WotReplay->EndInfo.health=temp.value.toUInt();
      }
      else if(temp.type.toLower()=="honortitles")
      {
	WotReplay->EndInfo.honorTitles=List;
	List.clear();
      }
      else if(temp.type.toLower()=="hits")
      {
	WotReplay->EndInfo.hits=temp.value.toUInt();
      }
      return 0;
      break;

    case ('k'):
      if(temp.type.toLower()=="killedtypecompdescrs")
      {
	WotReplay->EndInfo.killedTypeCompDescrs=List;
	List.clear();
      }
      else if(temp.type.toLower()=="killerid")
      {
	WotReplay->EndInfo.killerID=temp.value.toUInt();
      }
      else if(temp.type.toLower()=="killed")
      {
	WotReplay->EndInfo.killed=List;
	List.clear();
      }
      return 0;
      break;

    case ('p'):
      if(temp.type.toLower()=="potentialdamagereceived")
      {
	WotReplay->EndInfo.potentialDamageReceived=temp.value.toUInt();
      }
      else if(temp.type.toLower()=="potentialdamagedealt")
      {
	WotReplay->EndInfo.potentialDamageDealt=temp.value.toUInt();
      }
      return 0;
      break;

    case ('s'):
      if(temp.type.toLower()=="shotsreceived")
      {
	WotReplay->EndInfo.shotsReceived=temp.value.toUInt();
	List.clear();
      }
      else if(temp.type.toLower()=="spotted")
      {
	WotReplay->EndInfo.spotted=List;
	List.clear();
      }
      else if(temp.type.toLower()=="shots")
      {
	WotReplay->EndInfo.shots=temp.value.toUInt();
      }
      return 0;
      break;

    case ('t'):
      if(temp.type.toLower()=="tmenxp")
      {
	WotReplay->EndInfo.tmenXP=temp.value.toUInt();
      }
      else if(temp.type.toLower()=="tkillrating")
      {
	WotReplay->EndInfo.tkillRating=temp.value.toDouble();
      }
      else if(temp.type.toLower()=="tkilllog")
      {
	WotReplay->EndInfo.tkillLog=Map;
	Map.clear();
      }
      return 0;
      break;

    case ('x'):
      if(temp.type.toLower()=="xppenalty")
      {
	WotReplay->EndInfo.xpPenalty=temp.value.toUInt();
      }
      else if(temp.type.toLower()=="xp")
      {
	WotReplay->EndInfo.xp=temp.value.toUInt();
      }
      return 0;
      break;

    case ('v'):
       if(temp.type.toLower()=="vehicleid")
      {
	WotReplay->EndInfo.vehicleID=temp.value.toUInt();
      }
      else if(temp.type.toLower()=="vehiclelockmode")
      {
	WotReplay->EndInfo.vehicleLockMode=temp.value.toUInt();
      }
      return 0;
      break;

    default:
      if(temp.type.toLower()=="bonustype")
      {
	WotReplay->EndInfo.bonusType=temp.value.toUInt();
      }
      else if(temp.type.toLower()=="gold")
      {
	WotReplay->EndInfo.gold=temp.value.toUInt();
      }
      else if(temp.type.toLower()=="iswinner")
      {
	WotReplay->EndInfo.isWinner=temp.value.toShort();
      }
      else if(temp.type.toLower()=="lifetime")
      {
	WotReplay->EndInfo.lifeTime=temp.value.toUInt();
      }
      else if(temp.type.toLower()=="markofmastery")
      {
	WotReplay->EndInfo.markOfMastery=temp.value.toUInt();
      }
      else if(temp.type.toLower()=="repair")
      {
	WotReplay->EndInfo.repair=temp.value.toUInt();
      }
      return 0;
      break;
  }


}
