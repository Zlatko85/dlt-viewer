/**
 * @licence app begin@
 * Copyright (C) 2011-2012  BMW AG
 *
 * This file is part of GENIVI Project Dlt Viewer.
 *
 * Contributions are licensed to the GENIVI Alliance under one or more
 * Contribution License Agreements.
 *
 * \copyright
 * This Source Code Form is subject to the terms of the
 * Mozilla Public License, v. 2.0. If a  copy of the MPL was not distributed with
 * this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \file dltsystemviewerplugin.cpp
 * For further information see http://www.genivi.org/.
 * @licence end@
 */

#include <QtGui>

#include "dltsystemviewerplugin.h"

DltSystemViewerPlugin::DltSystemViewerPlugin()
{
    dltFile = 0;
}

DltSystemViewerPlugin::~DltSystemViewerPlugin()
{

}

QString DltSystemViewerPlugin::name()
{
    return QString("DLT System Viewer Plugin");
}

QString DltSystemViewerPlugin::pluginVersion(){
    return DLT_SYSTEM_VIEWER_PLUGIN_VERSION;
}

QString DltSystemViewerPlugin::pluginInterfaceVersion(){
    return PLUGIN_INTERFACE_VERSION;
}

QString DltSystemViewerPlugin::description()
{
    return QString();
}

QString DltSystemViewerPlugin::error()
{
    return QString();
}

bool DltSystemViewerPlugin::loadConfig(QString /* filename */)
{
    return true;
}

bool DltSystemViewerPlugin::saveConfig(QString /* filename */)
{
    return true;
}

QStringList DltSystemViewerPlugin::infoConfig()
{
    return QStringList();
}

QWidget* DltSystemViewerPlugin::initViewer()
{
    form = new Form();
    return form;
}


void DltSystemViewerPlugin::selectedIdxMsg(int index, QDltMsg &msg) {

}

void DltSystemViewerPlugin::selectedIdxMsgDecoded(int index, QDltMsg &msg){

}

void DltSystemViewerPlugin::initFileStart(QDltFile *file){

    dltFile = file;

    counterMessages = dltFile->size();

    form->deleteAllProccesses();

}

void DltSystemViewerPlugin::initMsg(int index, QDltMsg &msg){

    updateProcesses(index, msg);

}
void DltSystemViewerPlugin::initMsgDecoded(int index, QDltMsg &msg){

}

void DltSystemViewerPlugin::initFileFinish(){

}

void DltSystemViewerPlugin::updateFileStart(){

}
void DltSystemViewerPlugin::updateMsg(int index, QDltMsg &msg){

    if(!dltFile)
        return;

    updateProcesses(index, msg);

    counterMessages = dltFile->size();

}

void DltSystemViewerPlugin::updateMsgDecoded(int index, QDltMsg &msg){

}
void DltSystemViewerPlugin::updateFileFinish(){


}


void DltSystemViewerPlugin::updateProcesses(int index, QDltMsg &msg)
{
    QStringList datalist;
    QDltArgument arg;
    int pid,seq;

    if(!dltFile)
        return;



            if(msg.getMode() == QDltMsg::DltModeVerbose)
            {
                counterVerboseMessages++;
            }
            if(msg.getMode() == QDltMsg::DltModeNonVerbose)
            {
                counterNonVerboseMessages++;
            }

        if(msg.getApid()=="SYS" && msg.getCtid()=="PROC") {
            msg.getArgument(0,arg);
            pid = arg.toString().toInt();
            msg.getArgument(1,arg);
            if(arg.toString()=="stat") {
                msg.getArgument(2,arg);
                form->addProcesses(pid,arg.toString(),msg);
            }
        }        
        if(msg.getApid()=="SYS" && msg.getCtid()=="STAT") {
            msg.getArgument(0,arg);
            seq = arg.toString().toInt();
            if(seq==1) {
                msg.getArgument(1,arg);
                datalist = arg.toString().split(" ");
                form->setUser(QString("%1").arg(((datalist.at(2).toInt())-lastValueUser)*10000/(msg.getTimestamp()-lastTimeStamp)));
                form->setNice(QString("%1").arg(((datalist.at(3).toInt())-lastValueNice)*10000/(msg.getTimestamp()-lastTimeStamp)));
                form->setSystem(QString("%1").arg(((datalist.at(4).toInt())-lastValueKernel)*10000/(msg.getTimestamp()-lastTimeStamp)));
                lastValueUser = datalist.at(2).toInt();
                lastValueNice = datalist.at(3).toInt();;
                lastValueKernel = datalist.at(4).toInt();;
                lastTimeStamp = msg.getTimestamp();
            }
        }


}

Q_EXPORT_PLUGIN2(dltsystemviewerplugin, DltSystemViewerPlugin);
