 #include "qcheckboxpath.h"

QCheckBoxPath::QCheckBoxPath()
{
  int i;
  i=0;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void QCheckBoxPath::setIndex(unsigned int temp)
{
  Indexbox=temp;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

unsigned int QCheckBoxPath::getIndex()
{
  return Indexbox;
}
