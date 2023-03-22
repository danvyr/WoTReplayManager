#ifndef INFOWOTREPLAY_H
#define INFOWOTREPLAY_H

#include <QObject>
#include <QMap>

struct vehicle
{
  QString vehicleType;
  bool  isAlive;
  QString  name;
  QString  clanAbbrev;
  unsigned int  team;
  QString events;
  bool isTeamKiller;
  unsigned int frags;
  //after
  QString Nation;
  QString Nick; //with clanteg or not, but full
  QString Tank;
};

struct Info
{
  QList <unsigned int> crewActivityFlags;
  unsigned int repair;
  unsigned int potentialDamageReceived;
  unsigned int xpPenalty;
  unsigned int damageDealt;
  unsigned long arenaCreateTime;
  QList <unsigned int> killedTypeCompDescrs;
  unsigned int potentialDamageDealt;
  QList <unsigned int> achieveIndices;
  unsigned int shotsReceived;
  unsigned int xp;
  QList <unsigned int> eventIndices;
  unsigned int bonusType;
  unsigned int droppedCapturePoints;
  unsigned int killerID;
  unsigned int creditsContributionIn;
  QList <unsigned int> damaged;
  unsigned int vehicleID;
  QMap <QString, unsigned int> factors;
  unsigned int capturePoints;
  QList <unsigned int> heroVehicleIDs;
  QList <unsigned int> epicAchievements;
  unsigned int health;
  unsigned int vehicleLockMode;
  unsigned int freeXP;
  QList <unsigned int> honorTitles;
  short isWinner;
  unsigned int credits;
  unsigned int markOfMastery;
  unsigned int gold;
  QList <unsigned int> killed;
  unsigned int tmenXP;
  unsigned int hits;
  unsigned long long int arenaUniqueID;
  double tkillRating;
  QList <unsigned int> spotted;
  unsigned int creditsPenalty;
  unsigned int damageReceived;
  QMap <QString, unsigned int> tkillLog;
  unsigned int shots;
  unsigned int lifeTime;
  unsigned int creditsContributionOut;
  QList <unsigned int> ammo;
  unsigned int arenaTypeID;

};
class InfoWotReplay : public QObject
{
  Q_OBJECT
public:
  explicit InfoWotReplay(QObject *parent = 0);
  void Clear();
  void Parse();
  void ParseVehicles(QMap <unsigned int, vehicle >  &TempVehicles);


  QString mapName;
  unsigned int playerID;
  QString playerVehicle;
  QString gameplayType;
  QString playerName;
  QString mapDisplayName;
  QString dateTime;
  //after
  QString Nation;
  QString Nick; //with clanteg or not, but full
  QString Tank;

  //
  QMap <unsigned int, vehicle >  StartVehicles, EndVihiles;
  QMap <unsigned int, unsigned int> FragTable;

  Info  EndInfo;

  bool isSecondTable;


  
signals:
  
public slots:

private:
  void ParseVehicle(QString &nationT, QString &tankT);
  void ParsePlayer(QString &name,
		   QString &nick,
		   QString &vihecleT,
		   QString &nationT,
		   QString &tankT,
		   const QString &clan= QString::null);

  
};

#endif // INFOWOTREPLAY_H
