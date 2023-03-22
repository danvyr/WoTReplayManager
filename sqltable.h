#ifndef SQLTABLE_H
#define SQLTABLE_H

#include <QObject>
#include <QtGui/QApplication>
//#include <QtSql>
#include <QTableView>


class SqlTable : public QObject
{
  Q_OBJECT
public:
  explicit SqlTable(QObject *parent = 0);
  
signals:
  
public slots:
  
};

#endif // SQLTABLE_H
