#ifndef SLICEINFO_H
#define SLICEINFO_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>

class SliceInfo : public QDialog
{
    Q_OBJECT
public:
    explicit SliceInfo(QDialog *parent = 0);

signals:

public slots:
    void Save();
    void Cancel();

public:
    void initWidget();
public:
    float m_RetLayerH;
    float m_RetExpoTime;
    int m_RetLiftH;
    //int m_RetSpeed;
    bool m_Saved;
    QComboBox *m_dataComBoxLayerH;
    QComboBox *m_dataComBoxTime;
    QComboBox *m_dataComBoxLiftH;
    QPushButton *m_BtnSave;
    QPushButton *m_BtnCancel;
};

#endif // SLICEINFO_H
