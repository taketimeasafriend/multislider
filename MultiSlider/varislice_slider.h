#include "MultiSlider.h"
#include <QComboBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QDialog>

class ueDoubleEdit;

typedef struct {
    float layer_sliceheight;
    int exposure_time;
    float lift_distance;
} SliceParam;

struct MultiSliceSegment {
    float z;
    QColor c;
};
typedef std::vector<MultiSliceSegment> MultiSliceSegments;

class VariSliceSlider : public MultiSlider {
    Q_OBJECT

public:
    explicit VariSliceSlider(Qt::Orientation o = Qt::Vertical, QWidget *parent = nullptr);
    ~VariSliceSlider();

protected:
    virtual void paintEvent(QPaintEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseDoubleClickEvent(QMouseEvent *e);

    void drawColoredRect(int pos, int nextPos, QStylePainter &painter, QColor highlight);
    void drawHandle(int num, QStylePainter *painter);
public:
    QColor segmentColor(float h);
    void initParamDlg();
    void resetParam();

//signals:
//    void sliceSegmentsChanged();

public:
    void paramStickOn(QMouseEvent *e, int indexHandle);
    void dlgGetData(int index);
    void dlgSetData(int index);

    void addParam(int index);
    void deleteParam(int index);
    void editParam(int index);

public:
    // properties session
    QDialog m_paramDlg;

    QVBoxLayout *groupboxLayout;

    QGroupBox *groupbox_Up;
    QComboBox *m_dataComBoxLayerH_Up;
    QComboBox *m_dataComBoxTime_Up;
    QComboBox *m_dataComBoxLiftH_Up;
//    ueDoubleEdit *m_dataComBoxLayerH_Up;
//    ueDoubleEdit *m_dataComBoxTime_Up;
//    ueDoubleEdit *m_dataComBoxLiftH_Up;

    QGroupBox *groupbox_Down;
    QComboBox *m_dataComBoxLayerH_Down;
    QComboBox *m_dataComBoxTime_Down;
    QComboBox *m_dataComBoxLiftH_Down;
//    ueDoubleEdit *m_dataComBoxLayerH_Down;
//    ueDoubleEdit *m_dataComBoxTime_Down;
//    ueDoubleEdit *m_dataComBoxLiftH_Down;
public:
    MultiSliceSegments sliceSegments;
    float currentZ;

public:
    // default slice data
    const float m_LayerH = 0.1;
    const int m_ExpoTime = 4000;
    const float m_LiftH = 8.0;

    const float m_Precision = 0.01;

    const QColor outlineColor = QColor::fromHslF(0 * 1.0 / 7, 1, 0.5);
public:
    QVector<SliceParam> m_SliceParams;

    int m_currentHandle;
    int m_dglHandle;
};
