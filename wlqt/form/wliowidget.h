#ifndef WLIOWidget_H
#define WLIOWidget_H

#include <QTabWidget>
#include <QTableWidget>
#include <QAbstractTableModel>
#include <QModelIndex>

#include "ui_wliowidget.h"

#include "wlmoduleioput.h"
#include "wlmoduleaioput.h"
#include "wlmodulepwm.h"
#include "wlmotion.h"

#define Table_in  0
#define Table_out 1
#define Table_pwm 2

class WLAIOPutViewModel: public QAbstractTableModel
{
Q_OBJECT

public:
    WLAIOPutViewModel(WLModuleAIOPut *moduleAIOPut,bool input,QObject *parent=nullptr):QAbstractTableModel(parent)
    {
    m_input=input;
    m_moduleAIOPut=moduleAIOPut;
    if(m_input)
      connect(moduleAIOPut,SIGNAL(changedInput(int)),SLOT(update(int)));
    else
      connect(moduleAIOPut,SIGNAL(changedOutput(int)),SLOT(update(int)));
    }

// QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent)   const {Q_UNUSED(parent); return m_input ? m_moduleAIOPut->getSizeInputs():m_moduleAIOPut->getSizeOutputs();}
    int columnCount(const QModelIndex &parent)const {Q_UNUSED(parent); return 2;}
    QVariant data(const QModelIndex &index, int role) const
    {
    QVariant ret;
    if(index.isValid())
        switch(role)
        {
        case Qt::EditRole:        if(!m_moduleAIOPut->getInput(index.row())->isEnable()) ret=0;
                                  break;

        case Qt::DisplayRole:         if(index.column()==0)                
                                               ret=m_input ? m_moduleAIOPut->getInput(index.row())->value()
                                                            :m_moduleAIOPut->getOutput(index.row())->value();

                                      break;

        case Qt::DecorationRole:      switch(index.column())
                                       {
                                       case 1: ret=m_moduleAIOPut->getInput(index.row())->isInv()? QIcon(":/data/icons/ion.png")
                                                                                                 : QIcon(":/data/icons/ioff.png");
                                               break;
                                       }
                                       break;

        }

    return ret;
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
    if (role != Qt::DisplayRole)
              return QVariant();

    if(orientation==Qt::Vertical
     &&section)  return section;

    if(orientation==Qt::Horizontal)
        switch(section)
        {
        case 0: return m_input ? tr("ainput") : tr("aoutput");
        case 1: return tr("inv");
        }

     return QVariant();
    }


    Qt::ItemFlags flags(const QModelIndex &index) const
    {
    Qt::ItemFlags ret=Qt::NoItemFlags;
    if(m_input)
     {
      if(index.isValid()&&m_moduleAIOPut->getInput(index.row())->isEnable())
      {
      ret=Qt::ItemIsEnabled;
      }
     }else if(index.isValid()&&m_moduleAIOPut->getOutput(index.row())->isEnable())
             {
             ret=Qt::ItemIsEnabled;
             }

    return ret;
    }

    WLModuleAIOPut *moduleAIOPut() const {return m_moduleAIOPut;}

private slots:
    void update(int n) {emit changedData(index(n,0));
                        emit changedData(index(n,1));}

signals:
   void changedData(QModelIndex);

private:
    WLModuleAIOPut *m_moduleAIOPut;
    bool m_input;

};


class WLIOPutViewModel: public QAbstractTableModel
{
Q_OBJECT

public:
    WLIOPutViewModel(WLModuleIOPut *moduleIOPut,bool input,QObject *parent=nullptr):QAbstractTableModel(parent)
    {
    m_input=input;
    m_moduleIOPut=moduleIOPut;
    if(m_input)
      connect(moduleIOPut,SIGNAL(changedInput(int)),SLOT(update(int)));
    else
      connect(moduleIOPut,SIGNAL(changedOutput(int)),SLOT(update(int)));
    }

// QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent)   const {Q_UNUSED(parent); return m_input ? m_moduleIOPut->getSizeInputs():m_moduleIOPut->getSizeOutputs();}
    int columnCount(const QModelIndex &parent)const {Q_UNUSED(parent); return 2;}
    QVariant data(const QModelIndex &index, int role) const
    {
    QVariant ret;
    if(index.isValid())
        switch(role)
        {
//        case Qt::EditRole:         if(!m_moduleIOPut->getInput(index.row())->isEnable()) ret=0;
//                                   break;

        case Qt::DisplayRole:         if(index.column()==0)
                                               ret=m_input ?
                                                   m_moduleIOPut->getInput(index.row())->getComment()
                                                  :m_moduleIOPut->getOutput(index.row())->getComment();
                                      break;

        case Qt::DecorationRole:      if(m_input)
                                      switch(index.column())
                                       {
                                        case 0: ret=m_moduleIOPut->getInput(index.row())->getNow()? QIcon(":/data/icons/ion.png")
                                                                                                 :  QIcon(":/data/icons/ioff.png");  break;

                                        case 1: ret=m_moduleIOPut->getInput(index.row())->isInv()? QIcon(":/data/icons/ion.png")
                                                                                                 : QIcon(":/data/icons/ioff.png");  break;
                                        }
                                       else
                                        switch(index.column())
                                          {
                                          case 0: ret=m_moduleIOPut->getOutput(index.row())->getNow()? QIcon(":/data/icons/ion.png")
                                                                                                    :  QIcon(":/data/icons/ioff.png"); break;

                                          case 1: ret=m_moduleIOPut->getOutput(index.row())->isInv()? QIcon(":/data/icons/ion.png")
                                                                                                     : QIcon(":/data/icons/ioff.png"); break;
                                          }
                                       break;

        }

    return ret;
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
    if (role != Qt::DisplayRole)
              return QVariant();

    if(orientation==Qt::Vertical
     &&section)  return section;

    if(orientation==Qt::Horizontal)
        switch(section)
        {
        case 0: return m_input ? tr("input"):tr("output");
        case 1: return tr("inv");
        }

     return QVariant();
    }


    Qt::ItemFlags flags(const QModelIndex &index) const
    {
    Qt::ItemFlags ret=Qt::NoItemFlags;

    if(m_input)
     {
      if(index.isValid()&&m_moduleIOPut->getInput(index.row())->isEnable())
      {
      ret=Qt::ItemIsEnabled;
      }
     }
    else if(index.isValid()&&m_moduleIOPut->getOutput(index.row())->isEnable())
          {
          ret=Qt::ItemIsEnabled;
          }
    return ret;
    }

    WLModuleIOPut *moduleIOPut() const {return m_moduleIOPut;}

private slots:
    void update(int n) {emit changedData(index(n,0));
                        emit changedData(index(n,1));}

signals:
   void changedData(QModelIndex);

private:
    WLModuleIOPut *m_moduleIOPut;
    bool m_input=true;

};


class WLOutPWMViewModel: public QAbstractTableModel
{
Q_OBJECT

public:
    WLOutPWMViewModel(WLModulePWM *modulePWM,QObject *parent=nullptr):QAbstractTableModel(parent)
    {
    m_modulePWM=modulePWM;
    connect(m_modulePWM,SIGNAL(changedOutPWM(int)),SLOT(updateOutPWM(int)));
    }

// QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent)   const {Q_UNUSED(parent); return m_modulePWM->getSizeOutPWM();}
    int columnCount(const QModelIndex &parent)const {Q_UNUSED(parent); return 4;}
    QVariant data(const QModelIndex &index, int role) const
    {
    QVariant ret;
    if(index.isValid())
        switch(role)
        {
        case Qt::EditRole:        if(!m_modulePWM->getOutPWM(index.row())->isEnable()) ret=0;
                                  break;
        case Qt::DisplayRole:         switch(index.column())
                                      {
                                      case 0: ret=QString::number(m_modulePWM->getOutPWM(index.row())->getKOut(),'f',2);break;
                                      case 1: ret=QString::number(m_modulePWM->getOutPWM(index.row())->power(),'f',2)+"%";break;
                                      case 3: ret=QString::number(m_modulePWM->getOutPWM(index.row())->freq(),'f',2)+"Hz";break;
                                      }
                                      break;

        case Qt::DecorationRole:      switch(index.column())
                                       {
                                       case 2: ret=m_modulePWM->getOutPWM(index.row())->isInv()? QIcon(":/data/icons/ion.png")
                                                                                               : QIcon(":/data/icons/ioff.png");
                                               break;
                                       }
                                       break;

        }

    return ret;
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
    if (role != Qt::DisplayRole)
              return QVariant();

    if(orientation==Qt::Vertical
     &&section)  return section;

    if(orientation==Qt::Horizontal)
        switch(section)
        {
        case 0: return    ("K");
        case 1: return tr("value");
        case 2: return tr("inv");
        case 3: return tr("Freq");
        }

     return QVariant();
    }


    Qt::ItemFlags flags(const QModelIndex &index) const
    {
    Qt::ItemFlags ret=Qt::NoItemFlags;
    if(index.isValid()&&m_modulePWM->getOutPWM(index.row())->isEnable())
      {
      ret=Qt::ItemIsEnabled;
      }

    return ret;
    }

    WLModulePWM *modulePWM() const {return m_modulePWM;}

signals:
    void changedData(QModelIndex);

private slots:
    void updateOutPWM(int n) {emit changedData(index(n,0));
                              emit changedData(index(n,1));
                              emit changedData(index(n,2));
                              emit changedData(index(n,3));}
private:
    WLModulePWM *m_modulePWM;

};

namespace Ui {
class  WLIOWidget;
}

class WLIOWidget : public QTabWidget
{
	Q_OBJECT

public:
    WLIOWidget(QWidget *parent = nullptr);
    ~WLIOWidget();

    void setDevice(WLMotion *Device);

    void setModuleIOPut(WLModuleIOPut *_ModuleIOPut)    {m_inputViewModel  = new WLIOPutViewModel(_ModuleIOPut,true);
                                                         m_outputViewModel = new WLIOPutViewModel(_ModuleIOPut,false);}

    void setModulePWM(WLModulePWM *_ModulePWM)          {m_outPWMViewModel = new WLOutPWMViewModel(_ModulePWM);}

    void setModuleAIOPut(WLModuleAIOPut *_ModuleAIOPut) {m_ainputViewModel  = new WLAIOPutViewModel(_ModuleAIOPut,true);
                                                         m_aoutputViewModel = new WLAIOPutViewModel(_ModuleAIOPut,false);}
	void Init();

private:
    Ui::WLIOWidget *ui;
	
    WLIOPutViewModel    *m_inputViewModel;
    WLIOPutViewModel    *m_outputViewModel;
    WLAIOPutViewModel   *m_ainputViewModel;
    WLAIOPutViewModel   *m_aoutputViewModel;
    WLOutPWMViewModel   *m_outPWMViewModel;



    QTableView   *m_tableViewIn;
    QTableView   *m_tableViewOut;
    QTableView   *m_tableViewOutPWM;

    QTableView   *m_tableViewAIn;
    QTableView   *m_tableViewAOut;


    //QTableWidget *TableIn;
   // QTableWidget *TableOut;
    //QTableWidget *TableOutPWM;


    QIcon *m_IconOn;
    QIcon *m_IconOff;


private slots:

    void setDCTableInput(QModelIndex);
    void setDCTableOutput(QModelIndex);
    void setDCTableOutPWM(QModelIndex);    
    void setDCTableAOutput(QModelIndex);

};

#endif // WLIOWidget_H
