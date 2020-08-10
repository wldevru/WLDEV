#ifndef WLGCODE_H
#define WLGCODE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDebug>
#include "WL3DPoint.h"
#include "WL6DPoint.h"
#include "WLFrame.h"

#define GNO_err  1
#define GNO_use  0
#define GErr    -1

#define GCodeSize 100
#define MCodeSize 100
#define sizeSC 7

#define sizeTools 24+1

struct WLGTool
{
double d;
double l;

WLGTool() {d=l=0.0;}

QString toString()
{
return QString::number(d)+","+QString::number(l);
}

bool fromString(QString str)
{
QStringList List=str.split(",");
if(List.size()==2)
 {
 d=List[0].toDouble();
 l=List[1].toDouble();
 return true;
 }
return false;
}

};

typedef struct par
{
double value;
bool  activ;

par() {value=0;activ=false;}
void set(double _val) {value=_val; activ=true;}

}par;

typedef struct WLGPoint
{    
double x;
double y;
double z;
double a;
double b;
double c;
double u;
double v;
double w;

WLGPoint() {x=y=z=a=b=c=u=v=w=0;}


double get(QString name,bool *ok=nullptr)
{
name=name.toUpper();

if(ok!=nullptr) *ok=true;

if(name=="X") return x;
if(name=="Y") return y;
if(name=="Z") return z;
if(name=="A") return a;
if(name=="B") return b;
if(name=="C") return c;
if(name=="U") return u;
if(name=="V") return v;
if(name=="W") return w;

if(ok!=nullptr) *ok=false;
return 0;
}

QString toString()
{
return
     "X "+QString::number(x)
   +",Y "+QString::number(y)
   +",Z "+QString::number(z)
   +",A "+QString::number(a)
   +",B "+QString::number(b)
   +",C "+QString::number(c);
}

bool fromString(QString str)
{
str.remove(QRegExp("[XYZABCUVW]"));
str.remove(QRegExp("[xyzabcuvw]"));

QStringList List=str.split(",");

switch(List.size())
{
case 9: u=List[6].toDouble();
        v=List[7].toDouble();
        w=List[8].toDouble();

case 6: a=List[3].toDouble();
        b=List[4].toDouble();
        c=List[5].toDouble();

case 3: x=List[0].toDouble();
        y=List[1].toDouble();
        z=List[2].toDouble();
        return true;
}

return false;
}

QString toString(bool all)
{
if(all)
return QString("X "+QString::number(x)+","
              +"Y "+QString::number(y)+","
              +"Z "+QString::number(z)+","
              +"A "+QString::number(a)+","
              +"B "+QString::number(b)+","
              +"C "+QString::number(c)+","
              +"U "+QString::number(u)+","
              +"V "+QString::number(v)+","
              +"W "+QString::number(w));
else {
    return QString("X "+QString::number(x)+","
                  +"Y "+QString::number(y)+","
                  +"Z "+QString::number(z));

}
}
WLGPoint normalize()
{
WLGPoint ret;
const double F=sqrt(x*x+y*y+z*z+a*a+b*b+c*c+u*u+v*v+w*w);

ret.x=x/F;
ret.y=y/F;
ret.z=z/F;
ret.a=a/F;
ret.b=b/F;
ret.c=c/F;
ret.u=u/F;
ret.v=v/F;
ret.w=w/F;

return ret;
}

WL3DPoint to3D()
{
WL3DPoint ret;

ret.x=x;
ret.y=y;
ret.z=z;

return ret;
}

void from6D(WL6DPoint A)
{
x=A.x;
y=A.y;
z=A.z;

a=A.a;
b=A.b;
c=A.c;
}

WL6DPoint to6D()
{
WL6DPoint ret;

ret.x=x;
ret.y=y;
ret.z=z;

ret.a=a;
ret.b=b;
ret.c=c;

return ret;
}

bool operator==(WLGPoint A)
{
return x==A.x
     &&y==A.y
     &&z==A.z
     &&a==A.a
     &&b==A.b
     &&c==A.c
     &&u==A.u
     &&v==A.v
     &&w==A.w;
}

WLGPoint operator+(WLGPoint A)
{
WLGPoint ret;

ret.x=x+A.x;
ret.y=y+A.y;
ret.z=z+A.z;
ret.a=a+A.a;
ret.b=b+A.b;
ret.c=c+A.c;
ret.u=u+A.u;
ret.v=v+A.v;
ret.w=w+A.w;

return ret;
}

WLGPoint operator-(WLGPoint A)
{
WLGPoint ret;

ret.x=x-A.x;
ret.y=y-A.y;
ret.z=z-A.z;
ret.a=a-A.a;
ret.b=b-A.b;
ret.c=c-A.c;
ret.u=u-A.u;
ret.v=v-A.v;
ret.w=w-A.w;

return ret;
}

WLGPoint operator*(double A)
{
WLGPoint ret;

ret.x=x*A;
ret.y=y*A;
ret.z=z*A;
ret.a=a*A;
ret.b=b*A;
ret.c=c*A;
ret.u=u*A;
ret.v=v*A;
ret.w=w*A;

return ret;
}


WLGPoint operator/(double A)
{
WLGPoint ret;

ret.x=x/A;
ret.y=y/A;
ret.z=z/A;
ret.a=a/A;
ret.b=b/A;
ret.c=c/A;
ret.u=u/A;
ret.v=v/A;
ret.w=w/A;

return ret;
}


}WLGPoint;

struct WLGCodeData
{

 WLGPoint    offsetSC[sizeSC]; //������� ��������� ��������
 WLGPoint refPoint0SC[sizeSC]; //��������
 WLGPoint refPoint1SC[sizeSC]; //��������

 WLGPoint homePosition;

 WLGTool Tools[sizeTools];

 WLGPoint G51Scale;

 int iCurTool;

int iSC;
int iSCBuf;

float drillPlane;

bool GCode[GCodeSize];
bool GCodeBuf[GCodeSize];

bool absIJK;

bool MCode[MCodeSize];
bool MCodeBuf[MCodeSize];


par gX;
par gY;
par gZ;

par gI;
par gJ;
par gK;

par gA;
par gB;
par gC;

par gF;

par gR;
par gQ;

par gS;
par gT;

par gH;

WLGCodeData();
};

class WLGCode: public QObject
{
 Q_OBJECT

public:

	enum Code
	 {
     fast_motion=00,
    
	 line       =01,
     circle_cw  =02,
     circle_ccw =03,

	 wait_motion=04,

     plane_xy   =17,
	 plane_zx   =18,
	 plane_yz   =19,
     
	 drill      =81,
	 drill_long =83,
	 drill_off  =80,

	 absolute   =90,
	 incremental=91,

	 plane_drill =98,
	 plane_drillR =99
     };

public:
    WLGCode();
    WLGCode(WLGCodeData data);

    ~WLGCode();

    bool isGCode(int i) {return m_data.GCode[i];}
    bool isMCode(int i) {return m_data.MCode[i];}

  double getValue(char);
    bool isValid(char);
	bool setValue(char name,double data);
	void resetGValue();

   WLGPoint getPointGCode(WLGPoint lastGPoint);
   //WLGPoint getPointG(WLGPoint lastGPoint) {return getPointActivSC(getPointGCode(lastGPoint));}
   WLGPoint getPointG28(WLGPoint lastGPoint);
   WLGPoint getPointG53(WLGPoint lastGPoint);
   WLGPoint getPointIJK(WLGPoint lastGPoint);
   WLGPoint getPointSC(int iSC,WLGPoint GPoint,bool back=false);
   WLGPoint getPointActivSC(WLGPoint GPoint,bool back=false);

   WLGPoint movPointToActivSC(int iLastSC,WLGPoint &lastGPoint);

   static WLGPoint convertPlane(WLGPoint Point,int plane,bool front);

   int getActivSC(WLGPoint *P=nullptr) {if(P!=nullptr) *P=getSC(m_data.iSC); return m_data.iSC;}
   
   double getDrillPlaneValue(char);

   WLGPoint getSC(int i,bool *ok=nullptr);
   WLGPoint getOffsetSC(int i,bool *ok=nullptr);
   WLGPoint getRefPoint0SC(int i,bool *ok=nullptr);
   WLGPoint getRefPoint1SC(int i,bool *ok=nullptr);

   bool setOffsetActivSC(WLGPoint P)    {return setOffsetSC(m_data.iSC,P);}
   bool setOffsetSC(int i,WLGPoint P);
   
   bool setRefPoint0SC(int i,WLGPoint P)  {if(0<i&&i<sizeSC) {m_data.refPoint0SC[i]=P; return 1;} else return 0;}
   bool setRefPoint1SC(int i,WLGPoint P)  {if(0<i&&i<sizeSC) {m_data.refPoint1SC[i]=P; return 1;} else return 0;}

   void rotAboutRotPointSC(int i,float a);

   bool loadData(QString name_file);
   bool saveData(QString name_file);

   void setXSC(double X) {setXSC(X,m_data.iSC);}
   void setYSC(double Y) {setXSC(Y,m_data.iSC);}
   void setZSC(double Z) {setXSC(Z,m_data.iSC);}
  // void setASC(float A) {setXSC(A,iSC);};

   void setXSC(double X,int i)       {m_data.offsetSC[i].x=X;}
   void setYSC(double Y,int i)       {m_data.offsetSC[i].y=Y;}
   void setZSC(double Z,int i)       {m_data.offsetSC[i].z=Z;}
													   					 
   void setOffsetASC(double A,int i) {m_data.offsetSC[i].a=A; }

  // void pushCode();
  // void popCode();

    bool calcCenterPointR(WLGPoint startPoint,WLGPoint endPoint);
	 
	int setGCode(QString val);
    int setGCode(int val) {return setGCode(QString::number(val));}
	
	void resetGCode(int iG=-1);

	int setMCode(int);
	void resetMCode(int iM=-1);
	     
	void reset(void) {resetGValue();resetGCode();resetMCode();}

    WLGPoint getHomePosition()        {return m_data.homePosition;}
    void setHomePosition(WLGPoint hp) {m_data.homePosition=hp;}

	int getPlaneCirc();

	QString getActivGCodeString();

    void setDataTool(int number,double _diam,double _length);

    WLGTool getCurTool()   {return m_data.Tools[m_data.iCurTool];}
    WLGTool getTool(int i) {return m_data.Tools[i];}

    void verifyG51();

    void setData(const WLGCodeData &data);

    WLGCodeData getData() const;

private:
    WLGCodeData m_data;


signals :

void changedSK(int);
void changedF();

};

#endif // WLGCODE_H