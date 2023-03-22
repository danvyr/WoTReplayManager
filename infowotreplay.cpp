#include "infowotreplay.h"
#include <QDebug>


InfoWotReplay::InfoWotReplay(QObject *parent) :
  QObject(parent)
{
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void InfoWotReplay::Clear()
{
  EndInfo.crewActivityFlags.clear();
  EndInfo.killedTypeCompDescrs.clear();
  EndInfo.achieveIndices.clear();
  EndInfo.eventIndices.clear();
  EndInfo.damaged.clear();
  EndInfo.factors.clear();
  EndInfo.heroVehicleIDs.clear();
  EndInfo.epicAchievements.clear();
  EndInfo.honorTitles.clear();
  EndInfo.killed.clear();
  EndInfo.spotted.clear();
  EndInfo.tkillLog.clear();
  EndInfo.ammo.clear();


  StartVehicles.clear();
  EndVihiles.clear();
  FragTable.clear();
  isSecondTable=0;

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void InfoWotReplay::Parse()
{
  ParsePlayer(playerName,
	      Nick,
	      playerVehicle,
	      Nation,
	      Tank);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void InfoWotReplay::ParseVehicles(QMap <unsigned int, vehicle >  &TempVehicles)
{

  QMap<unsigned int, vehicle>::iterator i;

  for (i = TempVehicles.begin(); i != TempVehicles.end(); ++i)
  {
    ParsePlayer(TempVehicles[i.key()].name,
		TempVehicles[i.key()].Nick,
		TempVehicles[i.key()].vehicleType,
		TempVehicles[i.key()].Nation,
		TempVehicles[i.key()].Tank,
		TempVehicles[i.key()].clanAbbrev);
  }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void InfoWotReplay::ParsePlayer(QString &name,
				QString &nick,
				QString &vihecleT,
				QString &nationT,
				QString &tankT,
				const QString &clan )
{
  int j=0;

  if ( !(name.isEmpty() || vihecleT.isEmpty()) )
  {
    if ( clan.isEmpty() )
      nick = name;
    else
      nick = name	+'['+ clan +']';

    if (vihecleT.at(0) == 'u')
    {
      if(vihecleT.at(2) == 's')
      {
	j=4;
	nationT=QString().fromUtf8("СССР");
      }
      else
      {
	j=3;
	nationT=QString().fromUtf8("США");
      }
    }
    else if (vihecleT.at(0) == 'g')
    {
      j=7;
      nationT=QString().fromUtf8("Германия");
    }
    else if (vihecleT.at(0) == 'f')
    {
      j=6;
      nationT=QString().fromUtf8("Франция");
    }
    else if (vihecleT.at(0) == 'c')
    {
      j=5;
      nationT=QString().fromUtf8("Китай");
    }

    if (vihecleT.at(j)=='-')
      j=j+1;
    tankT= vihecleT.mid(j) ;

    ParseVehicle(nationT, tankT);
  }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void InfoWotReplay::ParseVehicle (QString &nationT, QString &tankT)
{

  if (nationT==QString().fromUtf8("СССР"))
  {
    if (tankT.at(0) == 'I')
    {
      tankT.replace(QString("IS"),QString().fromUtf8("ИС"));
      tankT.replace(QString("U"),QString().fromUtf8("У"));
    /*
    "IS-7"    "СССР"
    "IS-4"    "СССР"
    "IS-3"    "СССР"
    "IS"    "СССР"
    "ISU-152"    "СССР"
    */
    }
    else if (tankT.at(0) == 'K')
    {
      tankT.replace(QString("KV"),QString().fromUtf8("КВ"));
      tankT.replace(QString("s"),QString().fromUtf8("C"));
    /*
    "KV-5"    "СССР"
    "KV-3"    "СССР"
    "KV-1s"    "СССР"
    "KV"    "СССР"
    "KV-13"    "СССР"
    */
    }
    else if (tankT.at(0) == 'T')
    {
      tankT.replace(QString("T"),QString().fromUtf8("Т"));
      tankT.replace(QString("_"),QString().fromUtf8("-"));
      /*
      "T-54"    "СССР"
      "T-44"    "СССР"
      "T-43"    "СССР"
      "T-34-85"    "СССР"
      "T-34"    "СССР"
      "T-28"    "СССР"
      "T_50_2"    "СССР
      "T-50"    "СССР"
      "T-46"    "СССР"
      "T-26"    "СССР"
      "Tetrarch_LL"    "СССР"
      */
    }
    else if (tankT.at(0) == 'S')
    {
      tankT.replace(QString("S"),QString().fromUtf8("С"));
      tankT.replace(QString("U"),QString().fromUtf8("У"));
      /*
      "SU-5"    "СССР"
      "SU-26"    "СССР"
      "SU-152"    "СССР"
      "SU-85"    "СССР"
      "S-51"    "СССР"
      */
    }
    else if (tankT.at(0) == 'C')
    {
      tankT="Churchill";
      /* "Churchill_LL"    "СССР" */
    }
    else if (tankT.at(0) == 'A')
    {
      tankT.replace(QString("A"),QString().fromUtf8("А"));
      //      "A-20"    "СССР"
    }
    else if (tankT.at(0) == 'O')
    {
      tankT.replace(QString("Object"),QString().fromUtf8("Объект"));
      tankT.replace(QString("_"),QString().fromUtf8(" "));
      /*
    "Object_704"    "СССР"
    "Object_261"    "СССР"
    "Object_212"    "СССР"
    */
    }
    else if (tankT.at(0) == 'B')
    {
      tankT.replace(QString("BT"),QString().fromUtf8("БТ"));
      /*
    "BT-7"    "СССР"
    "BT-2"    "СССР"
    */
    }
    else if (tankT.at(0) == 'M')
    {
      tankT.replace(QString("MS"),QString().fromUtf8("МС"));
     /* "MS-1"    "СССР" */
    }
    else if (tankT.at(0) == 'G')
    {
      tankT=QString().fromUtf8("СУ-76");
      /*
     "GAZ-74b"    "СССР"
     */
    }
  }
  else if (nationT==QString().fromUtf8("США"))
  {
    /*
"M46_Patton"    "США"
"M3_Grant"    "США"

"M7_med"    "США"
"M2_med"    "США"
"M2_lt"    "США"

"M37"    "США"
"M103"    "США"
"M18_Hellcat"    "США"
"M8A1"    "США"
"M3_Stuart"    "США"


"T110"    "США"
"T34_hvy"    "США"
"T32"    "США"
"T29"    "США"
"T30"    "США"
"T82"    "США"
"T14"    "США"
"T92"    "США"
"T49"    "США"
"T2_lt"    "США"
"T1_Cunningham"    "США"
*/
  }
  else if (nationT==QString().fromUtf8("Германия"))
  {
    /*
"VK3001P"    "Германия"
"VK3601H"    "Германия"
"VK1602"    "Германия"
"VK2801"    "Германия"
"VK4502A"    "Германия"

"Hummel"    "Германия"
"Hetzer"    "Германия"

"G_E"    "Германия"
"Grille"    "Германия"
"G_Panther"    "Германия"
"G_Tiger"    "Германия"



"JagdTiger"    "Германия"
"JagdPzIV"    "Германия"
"StuGIII"    "Германия"
"G20_Marder_II"    "Германия"


"E-75"    "Германия"
"Lowe"    "Германия"

"PzVIB_Tiger_II"    "Германия"
"Panther_II"    "Германия"

"PzIV"    "Германия"
"PzIII_IV"    "Германия"
"PzIII_A"    "Германия"
"PzII_Luchs"    "Германия"
"PzII"    "Германия"
"Pz38_NA"    "Германия"

"PzV"    "Германия"  -tiger

"T-25"    "Германия"

"Ltraktor"    "Германия"

*/
  }
  else if (nationT==QString().fromUtf8("Франция"))
  {
    tankT.replace(QString("_"),QString().fromUtf8(" "));
	/*
   "AMX_50_100"    "Франция"

   "AMX_13_75"    "Франция"
   "AMX_13_90"    "Франция"

   "AMX_50_120"    "Франция"
   "AMX40"    "Франция"
   "AMX38"    "Франция"
"ARL_44"    "Франция"

"Lorraine40t"    "Франция"

 "RenaultFT"    "Франция"
"Bat_Chatillon25t"    "Франция"
"B1"    "Франция"
"BDR_G1B"    "Франция"
*/
  }
  else if (nationT==QString().fromUtf8("Китай"))
  {
    if(tankT.right(2)=="59")
      tankT=QString().fromUtf8("Type 59");
    /*
"Ch01_Type59"    "Китай"
*/
  }
  //qDebug()<< tankT<< "  " << nationT;


}





