#include <QtGui/QApplication>
#include "mainwindow.h"


/*
сделать общую базу данных
проверить сопадают ли ID играков и ники
если да, можно сделать статистику кто кого часто встречал
сделать статиситку по картам/танкам


   Модули
   Сортировки (по папкам)

   Статистики
по картам,
машинам,
по тому в какое время лучше были игры,
и с премом и без)

   Расчёт финансов
С премом и без



дизайн
меню
опции
(настройка папок (потом пробовать найти по реестру)
, )

открывать реплей для проигрывания (сделать и для линуха тоже)

загрузка на вотреплей.ком

+ в подробном просмотре теперь имена игроков - это ссылки на их профиль (за идею спсб thecrucio)

*/
int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  
  return a.exec();
}
