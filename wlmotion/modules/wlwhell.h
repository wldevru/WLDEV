#ifndef WLWHELL_H
#define WLWHELL_H

#include <QObject>
#include <QDataStream>
#include <QTimer>
#include <QDebug>

#include "wlmodule.h"
#include "wlmoduleencoder.h"
#include "wlmoduleioput.h"
#include "wlflags.h"

//Whell
#define comWhell_setEnable        1 //enable whell
#define comWhell_setInputAxis     2 //set inputs select axis
#define comWhell_setInputX        3 //set inputs select X1 X10... axis
#define comWhell_setInputVmode    4 //set input select Vmode/Pmode
#define comWhell_setFlag          5 //set type input data
#define comWhell_setDataAxis      6 //set axis data for track
#define comWhell_setOutputENB     7 //set output enable
#define comWhell_setManualIA      8 //set manula
#define comWhell_setManualIX      9 //set manula
#define comWhell_setManualVmode  10 //set manula
#define comWhell_setEncoder      11 //set encoder
#define comWhell_getData         12 //call data

#define sendWhell_data 1

#define errorWhell_setdata 1
#define errorWhell_enable  2

#define WHF_enable    1<<0
#define WHF_inv       1<<2
#define WHF_inAbinary 1<<3 //use input axis binary
#define WHF_inXbinary 1<<4 //use input x binary
#define WHF_manualA    1<<5 //use input A
#define WHF_manualX    1<<6 //use input X
#define WHF_manualV    1<<7 //use input V

const QString errorWhell("0,no error\
,1,set data\
,2,wrong enable");

#define errorWhell_setdata 1
#define errorWhell_enable  2

#define sizeWhellData 8
#define sizeInAxis 8
#define sizeInX 4

class WLWhell : public WLElement
{
	Q_OBJECT

public:
	
 WLWhell(QObject *parent=nullptr);
~WLWhell(); 

private:

 quint8 error;
 WLFlags Flags;

 quint8 iEncoder;
quint16 m_pulses;

 quint8 iInAxis[sizeInAxis];
 quint8 iInX[sizeInX];
 quint8 iInVmode;
 quint8 iOutENB;

 quint8 curIndexAxis;
 quint8 curIndexX;
   bool curVmode;

WLModuleIOPut *MIOPut;
WLModuleEncoder *MEncoder;

public:

   void setError(quint8 err)  {emit changedError(error=err);}
   
   quint8 getEncoder() {return iEncoder;}
   quint8 getFlag()    {return Flags.m_Data;}

   quint8* getIndexInAxis() {return iInAxis;}
   quint8* getIndexInX()    {return iInX;}

   quint8 getCurIndexAxis() {return curIndexAxis;}
   quint8 getCurIndexX()    {return curIndexX;}
   quint8 getCurVmode()     {return curVmode;}

    bool isManualAxis() {return getFlag()&WHF_manualA;}
    bool isManualX()    {return getFlag()&WHF_manualX;}
    bool isManualV()    {return getFlag()&WHF_manualV;}

signals:
 
 void changedError(quint8);

 void changedCurIndexAxis(quint8);
 void changedCurIndexX(quint8);
 void changedCurVmode(bool);

public:

   quint8 getInVmode() {return iInVmode;}
   quint8 getOutENB()  {return iOutENB;}

	bool setEncoder(quint8 _iEncoder);

    bool setIndexAxis(quint8 m_index);
    bool setIndexX(quint8 m_index);
    bool setVmode(quint8 m_index);

    bool setInAxis(quint8 *indexs,quint8 size);
    bool setInX(quint8 *indexs,quint8 size);

    bool setInVmode(quint8 m_index);

    bool setOutENB(quint8 m_index);

    bool setFlag(quint8 flag);

    bool setEnable(bool enable);                                 

    bool setDataAxis(quint8 m_index,quint8 iAxis,float kTrack);

	bool sendGetData();

    void setData(quint8 Flag,quint8 indexA,quint8 indexX,bool Vmode);

    void setPulses(const quint16 &pulses);
 quint16 getPulses() const;

public:

virtual void writeXMLData(QXmlStreamWriter &stream);
virtual void  readXMLData(QXmlStreamReader &stream);

};


#endif // WLWHELL_H
