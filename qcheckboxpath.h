#ifndef QCHECKBOXPATH_H
#define QCHECKBOXPATH_H

#include <QCheckBox>

class QCheckBoxPath : public QCheckBox
{
//  Q_OBJECT

public:
  QCheckBoxPath();

  virtual void setIndex(unsigned int temp);
  virtual unsigned int getIndex();
  unsigned int Indexbox;

private:


};

#endif // QCHECKBOXPATH_H
