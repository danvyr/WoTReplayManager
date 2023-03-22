#include "settings.h"
#include <QXmlStreamWriter>
#include <QDebug>
#include <QTextStream>

Settings::Settings(QObject *parent) :
  QObject(parent)
{

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void Settings::Init()
{
  FileName="settings.ini";
  QFile File;
  File.setFileName(FileName);

  if(File.open(QIODevice::ReadOnly))
  {
    QXmlStreamReader xml(&File);
    while(!xml.atEnd() &&
	  !xml.hasError())
    {
	QXmlStreamReader::TokenType token = xml.readNext();

	if(token == QXmlStreamReader::StartDocument)
	{
	    continue;
	}
	if(token == QXmlStreamReader::StartElement)
	{
	    if(xml.name() == "Settings")
		continue;
	    if(xml.name() == "ReplayDir")
	    {
		xml.readNext();
		ReplayDir.setPath(xml.text().toString());
	    }
	    if(xml.name() == "FavoriteDir")
	    {
		xml.readNext();
		FavoriteDir.setPath(xml.text().toString());
	    }
	    if(xml.name() == "PathToWOT")
	    {
		xml.readNext();
		PathToWOT=xml.text().toString();
	    }
	    if(xml.name() == "RecusiveDir")
	    {
		xml.readNext();
		RecursivDir=xml.text().toString().toUInt();
	    }
	}
    }

    File.close();
  }
  else
  {
    //открыть диалог с настройками
    emit NoSetting();
  }
#ifdef Q_OS_LINUX
  if ( !File.exists("wotreplay.sh" ))
       CreateSH();
#endif

}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


int Settings::Save()
{
  QFile file(FileName);
  if (!file.open(QIODevice::WriteOnly)) {
      return 0;
  }

  QXmlStreamWriter stream(&file);
  stream.setAutoFormatting(true);
  stream.writeStartDocument();
  stream.writeStartElement("Settings" );

  stream.writeTextElement("ReplayDir",  ReplayDir.absolutePath().toAscii());
  stream.writeTextElement("FavoriteDir", FavoriteDir.absolutePath().toAscii());
  stream.writeTextElement("PathToWOT",   PathToWOT);
  stream.writeTextElement("RecusiveDir", QString::number(RecursivDir));

  stream.writeEndElement();
  stream.writeEndDocument();
  file.close();

  return 1;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifdef Q_OS_LINUX
int Settings::CreateSH()
{
  QFile file;
  file.setFileName("wotreplay.sh");

  if (!file.open(QIODevice::WriteOnly)) {
      return 0;
  }
  QTextStream stream;
  stream.setDevice(&file);

  stream<<"#!/bin/sh \n\n";
  stream<<"exec  2> /dev/null \n";
  stream<<"cd " +  PathToWOT.remove("WorldOfTanks.exe") +  " \n";
  stream<<"wine WorldOfTanks.exe ${1##} \n";
  file.setPermissions("wotreplay.sh",
		      QFile::ExeOther  | QFile::ExeOwner  |QFile::ExeGroup |
		      QFile::ReadOther | QFile::ReadOwner |QFile::ReadGroup |
		      QFile::WriteOwner| QFile::WriteGroup );

  file.close();

  return 1;
}
#endif
