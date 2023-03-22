#ifndef READWOTREPLAY_H
#define READWOTREPLAY_H

//#include <QObject>
#include <QThread>
#include "infowotreplay.h"




class ReadWotReplay : public QThread
{
  Q_OBJECT
public:
  explicit ReadWotReplay(QObject *parent = 0);
  
signals:
  
public slots:

  int ReadReplay( );
  void Init(QString tFileName, InfoWotReplay &tempRep);

  virtual void run();

private:
  struct json1
  {
    QString type, value;
  };

  unsigned int ReadVehicles(char str[],
			    unsigned int pos,
			    QMap <unsigned int, vehicle> &tanks);
  unsigned int ReadFrags(char str[],
			 unsigned int pos,
			 QMap <unsigned int, vehicle> &tanks);

  void ParseVihecles(QString &tempID,
		     QList<json1> &vihicles,
		     QMap<unsigned int, vehicle> &tanks);

  unsigned int ParseIntList(char str[],
			    unsigned int pos,
			    QList <unsigned int> &tempList);
  unsigned int ParseMap(char str[],
			unsigned int pos,
			QMap <QString, unsigned int> &tempMap);

  void ReadSecondPart(char str[],
		      unsigned int pos);

  int ParseSecond(json1 &temp);
  void ParseFirst(QList <json1> &tempjson);


  InfoWotReplay *WotReplay;
  QString FileName;


  bool isTypeReaded, isReplayEnded, isMapParsed, isListParsed;
  bool isFirstTable, isSecondTable, Debug;
  unsigned int  fs, s, ks;  //фигурные скобки, скобки, квадратные скобки




  //временные данные

  unsigned int ID;

  QList <unsigned int> List;
  QMap <QString, unsigned int> Map;


};

#endif // READWOTREPLAY_H
