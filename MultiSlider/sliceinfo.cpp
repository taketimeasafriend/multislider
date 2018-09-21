#include "sliceinfo.h"
#include <QLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QGroupBox>

SliceInfo::SliceInfo(QDialog *parent) : QDialog(parent)
{
    setWindowTitle(tr("Slicer Data"));
    initWidget();
    connect(m_BtnSave,&QPushButton::clicked,this,&SliceInfo::Save);
    connect(m_BtnCancel,&QPushButton::clicked,this,&SliceInfo::Cancel);

}

void SliceInfo::initWidget()
{
    m_RetLayerH = 0.0;
    m_RetExpoTime = 0.0;
    m_RetLiftH = 0;

    m_Saved = false;
    setGeometry(100,100,200,200);

    QLabel *labelLayerH = new QLabel;
    labelLayerH->setText(tr("Below Slice H"));
    m_dataComBoxLayerH = new QComboBox;
    m_dataComBoxLayerH->addItem(QString::number(0.1));
    m_dataComBoxLayerH->addItem(QString::number(0.2));
    m_dataComBoxLayerH->addItem(QString::number(0.3));

    QLabel *labelTime = new QLabel;
    labelTime->setText(tr("Exposure Time"));
    m_dataComBoxTime = new QComboBox;
    m_dataComBoxTime->addItem(QString::number(2.0));
    m_dataComBoxTime->addItem(QString::number(2.5));
    m_dataComBoxTime->addItem(QString::number(3.0));

    QLabel *labelLiftH = new QLabel;
    labelLiftH->setText(tr("Lift H"));
    m_dataComBoxLiftH = new QComboBox;
    m_dataComBoxLiftH->addItem(QString::number(3));
    m_dataComBoxLiftH->addItem(QString::number(6));
    m_dataComBoxLiftH->addItem(QString::number(9));

    QHBoxLayout *layoutSlice = new QHBoxLayout;
    layoutSlice->addWidget(labelLayerH);
    layoutSlice->addWidget(m_dataComBoxLayerH);
    layoutSlice->addWidget(labelTime);
    layoutSlice->addWidget(m_dataComBoxTime);
    layoutSlice->addWidget(labelLiftH);
    layoutSlice->addWidget(m_dataComBoxLiftH);

    m_BtnSave = new QPushButton;
    m_BtnSave->setText(tr("Save"));
    m_BtnCancel = new QPushButton;
    m_BtnCancel->setText(tr("Cancel"));
    QHBoxLayout *layoutBtn = new QHBoxLayout;
    layoutBtn->addWidget(m_BtnSave);
    layoutBtn->addWidget(m_BtnCancel);

    QVBoxLayout *layout1 = new QVBoxLayout;
    layout1->addLayout(layoutSlice);
    layout1->addLayout(layoutBtn);

    QGroupBox *groupbox1 = new QGroupBox;
    QGroupBox *groupbox2 = new QGroupBox;
    groupbox1->setGeometry(this->pos().rx(),this->pos().ry(),this->width(),this->height()*0.5);
    groupbox2->setGeometry(this->pos().rx(),this->height()*0.5,this->width(),this->height());

    groupbox1->setLayout(layout1);
    //add to groupbox2...

    QVBoxLayout *layoutAll = new QVBoxLayout;
    layoutAll->addWidget(groupbox1);
    layoutAll->addWidget(groupbox2);
    setLayout(layoutAll);
    setWindowFlags(Qt::FramelessWindowHint);
}

void SliceInfo::Save()
{
    m_RetLayerH = m_dataComBoxLayerH->currentText().toFloat();
    m_RetExpoTime = m_dataComBoxTime->currentText().toFloat();
    m_RetLiftH = m_dataComBoxLiftH->currentText().toInt();
    m_Saved = true;
    this->close();
}

void SliceInfo::Cancel()
{
    this->close();
}
