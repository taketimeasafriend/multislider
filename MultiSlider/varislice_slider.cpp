#include "varislice_slider.h"
#include <QDebug>
#include <QGroupBox>
#include <QLabel>

VariSliceSlider::VariSliceSlider(Qt::Orientation o, QWidget *parent) : MultiSlider(o, parent) {
    this->setMaximum(1000);
    m_currentHandle = -1;
    m_dglHandle = 0;
    initParamDlg();

    resetParam();
}

VariSliceSlider::~VariSliceSlider() {}

void VariSliceSlider::initParamDlg() {
    m_paramDlg.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    //begin add dataDlg
    groupboxLayout = new QVBoxLayout;

    //up
    QLabel *labelLayerH_Up = new QLabel;
    labelLayerH_Up->setText(tr("Slice H(mm):"));
    m_dataComBoxLayerH_Up = new QComboBox;
    m_dataComBoxLayerH_Up->addItem(QString::number(m_LayerH));
    m_dataComBoxLayerH_Up->addItem(QString::number(m_LayerH + 0.01));
    m_dataComBoxLayerH_Up->addItem(QString::number(m_LayerH + 0.01 + 0.01));
    QLabel *labelTime_Up = new QLabel;
    labelTime_Up->setText(tr("Exposure Time(ms):"));
    m_dataComBoxTime_Up = new QComboBox;
    m_dataComBoxTime_Up->addItem(QString::number(m_ExpoTime));
    m_dataComBoxTime_Up->addItem(QString::number(m_ExpoTime + 300));
    m_dataComBoxTime_Up->addItem(QString::number(m_ExpoTime + 300 + 500));
    QLabel *labelLiftH_Up = new QLabel;
    labelLiftH_Up->setText(tr("Lift H(mm):"));
    m_dataComBoxLiftH_Up = new QComboBox;
    m_dataComBoxLiftH_Up->addItem(QString::number(m_LiftH));
    m_dataComBoxLiftH_Up->addItem(QString::number(m_LiftH + 3.0));
    m_dataComBoxLiftH_Up->addItem(QString::number(m_LiftH + 3.0 + 3.0));
    QVBoxLayout *layoutSlice_Up = new QVBoxLayout;
    layoutSlice_Up->addWidget(labelLayerH_Up);
    layoutSlice_Up->addWidget(m_dataComBoxLayerH_Up);
    layoutSlice_Up->addWidget(labelTime_Up);
    layoutSlice_Up->addWidget(m_dataComBoxTime_Up);
    layoutSlice_Up->addWidget(labelLiftH_Up);
    layoutSlice_Up->addWidget(m_dataComBoxLiftH_Up);
    //down
    QLabel *labelLayerH_Down = new QLabel;
    labelLayerH_Down->setText(tr("Slice H(mm):"));
    m_dataComBoxLayerH_Down = new QComboBox;
    m_dataComBoxLayerH_Down->addItem(QString::number(m_LayerH));
    m_dataComBoxLayerH_Down->addItem(QString::number(m_LayerH + 0.01));
    m_dataComBoxLayerH_Down->addItem(QString::number(m_LayerH + 0.01 + 0.01));
    QLabel *labelTime_Down = new QLabel;
    labelTime_Down->setText(tr("Exposure Time(ms):"));
    m_dataComBoxTime_Down = new QComboBox;
    m_dataComBoxTime_Down->addItem(QString::number(m_ExpoTime));
    m_dataComBoxTime_Down->addItem(QString::number(m_ExpoTime + 300));
    m_dataComBoxTime_Down->addItem(QString::number(m_ExpoTime + 300 + 500));
    QLabel *labelLiftH_Down = new QLabel;
    labelLiftH_Down->setText(tr("Lift H(mm):"));
    m_dataComBoxLiftH_Down = new QComboBox;
    m_dataComBoxLiftH_Down->addItem(QString::number(m_LiftH));
    m_dataComBoxLiftH_Down->addItem(QString::number(m_LiftH + 3.0));
    m_dataComBoxLiftH_Down->addItem(QString::number(m_LiftH + 3.0 + 3.0));
    QVBoxLayout *layoutSlice_Down = new QVBoxLayout;
    layoutSlice_Down->addWidget(labelLayerH_Down);
    layoutSlice_Down->addWidget(m_dataComBoxLayerH_Down);
    layoutSlice_Down->addWidget(labelTime_Down);
    layoutSlice_Down->addWidget(m_dataComBoxTime_Down);
    layoutSlice_Down->addWidget(labelLiftH_Down);
    layoutSlice_Down->addWidget(m_dataComBoxLiftH_Down);

//    m_dataComBoxLayerH_Up = new ueDoubleEdit(0.1, 0.01, 0.5, 0.01, 2);
//    m_dataComBoxTime_Up = new ueDoubleEdit(4000, 500, 5000, 100, 0);
//    m_dataComBoxLiftH_Up = new ueDoubleEdit(8, 0, 20, 1, 0);

//    m_dataComBoxLayerH_Down = new ueDoubleEdit(0.1, 0.01, 0.5, 0.01, 2);
//    m_dataComBoxTime_Down = new ueDoubleEdit(4000, 500, 5000, 100, 0);
//    m_dataComBoxLiftH_Down = new ueDoubleEdit(8, 0, 20, 1, 0);

//    QGridLayout *loUp = new QGridLayout;
//    int r = 0;
//    loUp->addWidget(new QLabel(tr("Layer height(mm)")), ++r, 0);
//    loUp->addWidget(m_dataComBoxLayerH_Up, r, 1);
//    loUp->addWidget(new QLabel(tr("Exposure time(ms)")), ++r, 0);
//    loUp->addWidget(m_dataComBoxTime_Up, r, 1);
//    loUp->addWidget(new QLabel(tr("Lift distance(mm)")), ++r, 0);
//    loUp->addWidget(m_dataComBoxLiftH_Up, r, 1);
//    QGridLayout *loDown = new QGridLayout;
//    r = 0;
//    loDown->addWidget(new QLabel(tr("Layer height(mm)")), ++r, 0);
//    loDown->addWidget(m_dataComBoxLayerH_Down, r, 1);
//    loDown->addWidget(new QLabel(tr("Exposure time(ms)")), ++r, 0);
//    loDown->addWidget(m_dataComBoxTime_Down, r, 1);
//    loDown->addWidget(new QLabel(tr("Lift distance(mm)")), ++r, 0);
//    loDown->addWidget(m_dataComBoxLiftH_Down, r, 1);

    groupbox_Up = new QGroupBox;
    groupbox_Down = new QGroupBox;
    groupbox_Up->setGeometry(this->pos().rx(), this->pos().ry(), this->width(), this->height() * 0.5);
    groupbox_Down->setGeometry(this->pos().rx(), this->height() * 0.5, this->width(), this->height());
    groupbox_Up->setLayout(layoutSlice_Up);
    groupbox_Down->setLayout(layoutSlice_Down);

    groupboxLayout->addWidget(groupbox_Up);
    //QFrame *line = new QFrame(this);
    //line->setLineWidth(2);
    //line->setFrameShape(QFrame::HLine);
    //line->setFrameShadow(QFrame::Sunken);
    //groupboxLayout->addWidget(line);
    groupboxLayout->addWidget(groupbox_Down);
    //end add dataDlg

    m_paramDlg.setLayout(groupboxLayout);
}

void VariSliceSlider::resetParam() {
    MultiSlider::setCount(1);
    MultiSlider::setPosition(0, 0);

    // slider.size = down.size
    // param.size = slider.size + 1(up)
    SliceParam defaultSLAParam = {m_LayerH, m_ExpoTime, m_LiftH};
    m_SliceParams.clear();
    m_SliceParams.push_back(defaultSLAParam);
    m_SliceParams.push_back(defaultSLAParam);

    MultiSliceSegment defaultSegment = {0.0, outlineColor};
    sliceSegments.clear();
    sliceSegments.push_back(defaultSegment);

    dlgGetData(0);
    dlgSetData(0);
    m_paramDlg.hide();

    currentZ = 0.0;
}

void VariSliceSlider::paintEvent(QPaintEvent *e) {
    QStyleOptionSlider option;
    this->initStyleOption(&option);
    QStylePainter painter(this);
    option.subControls = QStyle::SC_SliderGroove;
    if (this->tickPosition() != NoTicks) {
        option.subControls |= QStyle::SC_SliderTickmarks;
    }
    option.sliderValue = this->minimum() - this->maximum(); //0 - 99
    option.sliderPosition = this->minimum() - this->maximum();
    painter.drawComplexControl(QStyle::CC_Slider, option);

    int pos = minimum();
    int nextPos = this->count() ? this->position(0) : maximum();
    drawColoredRect(pos, nextPos, painter, color(0, 0.5));

    if (this->count() == 0) {
        return;
    }
    for (int i = 0; i < this->count() - 1; ++i) {
        pos = this->position(i);
        nextPos = this->position(i + 1);
        drawColoredRect(pos, nextPos, painter, color(i + 1, 0.5));
    }
    pos = this->positions().last();
    nextPos = maximum();
    drawColoredRect(pos, nextPos, painter, color(this->count() + 1, 0.5));

    for (int i = 0; i < this->count(); ++i) {
        this->drawHandle(i, &painter);
    }
}

void VariSliceSlider::drawColoredRect(int pos, int nextPos, QStylePainter &painter, QColor highlight) {
    QStyleOptionSlider option;
    this->initStyleOption(&option);
    option.sliderPosition = pos;
    const QRect lr = style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, this);

    option.sliderPosition = nextPos;

    const QRect ur = style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, this);

    QRect groove = style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderGroove, this);
    if (option.orientation == Qt::Horizontal) {
        const int padding = (style()->objectName() == "macintosh") ? 5 : 2;
        groove = QRect(QPoint(qMin(lr.left(), ur.center().x()) + (pos != 0 ? 10 : 0), groove.center().y() - 2),
                       QPoint(qMax(lr.left(), ur.right() - padding), groove.center().y() + 1));
    } else {
        groove = QRect(QPoint(groove.center().x() - 11, qMin(lr.center().y(), ur.center().y() - 4)), //topleft
                       QPoint(groove.center().x() + 10, qMax(lr.center().y(), ur.top()) + 3)); //bottomright
    }

    painter.setPen(QPen(highlight.darker(300), 0));
    painter.setBrush(highlight);
    painter.drawRect(groove);
}

void VariSliceSlider::drawHandle(int num, QStylePainter *painter) {
    QStyleOptionSlider option;
    this->initSliderStyleOption(num, &option);

    option.subControls = QStyle::SC_SliderHandle;
    option.sliderValue = this->values().at(num);
    option.sliderPosition = this->positions().at(num);

//    if (_selectedHandles.contains(num)) {
//        option.activeSubControls = QStyle::SC_SliderHandle;
//        option.state |= QStyle::State_Sunken;
//    }

    painter->drawComplexControl(QStyle::CC_Slider, option);

    QRect rect = this->style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, this);
    QColor color_handle(Qt::lightGray);

    // draw slider Position tag on slider left
    QFont wordfont;
    wordfont.setPointSize(10);
    painter->setFont(wordfont);

    // cal layer number at position
    // this->maximum() = modelH*100;
    int layers = 0;
    float frontHandleH2Bot = 0.0;
    for(int i=0; i<num+1; ++i){
        float sliceH = m_SliceParams.at(i).layer_sliceheight*100;
        int perSegLayers = (this->positions().at(i) - frontHandleH2Bot)/sliceH;
        layers += perSegLayers;
        frontHandleH2Bot += perSegLayers*m_SliceParams.at(i).layer_sliceheight*100;
    }

    QString sliderNum = QString::number(9999, 10);

    QFontMetrics fm(wordfont);
    int widthFontMetrics = fm.boundingRect("0000").width();
    QPoint tickPts_position[] = {
        QPoint(rect.center().rx()-widthFontMetrics*0.5, rect.bottom()+0.2*rect.height()),
        QPoint(rect.center().rx()-widthFontMetrics*0.5, rect.top()   -0.2*rect.height()),
        QPoint(rect.center().rx()+widthFontMetrics*0.5, rect.top()   -0.2*rect.height()),
        QPoint(rect.center().rx()+widthFontMetrics*0.5, rect.bottom()+0.2*rect.height()),
    };
    QPen pen(color_handle);
    painter->setPen(pen);
    QBrush brush(color_handle);
    painter->setBrush(brush);
    painter->drawPolygon(tickPts_position, 4);
    pen.setColor(Qt::black);
    painter->setPen(pen);
    painter->drawText(rect.center().rx()-widthFontMetrics*0.5, rect.bottom(), sliderNum);

    // draw shutdown tag on slider right
    int offset = rect.height()*0.75;
    QPoint tickPts_shutdown(rect.right() - offset*0.8, rect.center().y());
    brush.setColor(Qt::red);
    painter->setBrush(brush);
    pen.setColor(Qt::lightGray);
    painter->setPen(pen);
    painter->drawEllipse(tickPts_shutdown, offset, offset);
    qreal lengthUnit = offset*qSin(M_PI_4);
    painter->drawLine(tickPts_shutdown.x(),tickPts_shutdown.y(),
                      tickPts_shutdown.x()+lengthUnit,tickPts_shutdown.y()-lengthUnit);
    painter->drawLine(tickPts_shutdown.x(),tickPts_shutdown.y(),
                      tickPts_shutdown.x()+lengthUnit,tickPts_shutdown.y()+lengthUnit);
    painter->drawLine(tickPts_shutdown.x(),tickPts_shutdown.y(),
                      tickPts_shutdown.x()-lengthUnit,tickPts_shutdown.y()+lengthUnit);
    painter->drawLine(tickPts_shutdown.x(),tickPts_shutdown.y(),
                      tickPts_shutdown.x()-lengthUnit,tickPts_shutdown.y()-lengthUnit);
}

QColor VariSliceSlider::segmentColor(float h) {
    float minH = 0.01;
    float maxH = 0.03;
    float bright = 0.2 + (0.7 - 0.5) * (h - minH) / (maxH - minH);
    // bright: 0-1 dark-light [0.2-0.7] red
    return QColor::fromHslF(0 * 1.0 / 7, 1, bright);
}

void VariSliceSlider::mousePressEvent(QMouseEvent *e) {
    MultiSlider::mousePressEvent(e);
    m_currentHandle = MultiSlider::selectedHandle();

    if (-1 < m_currentHandle) {
        if (m_dglHandle != m_currentHandle) {
            dlgSetData(m_dglHandle);
        }
        dlgGetData(m_currentHandle);
        m_paramDlg.show();
        paramStickOn(e, m_currentHandle);
    } else {
        dlgSetData(m_dglHandle);
        m_paramDlg.hide();
    }
}

void VariSliceSlider::mouseReleaseEvent(QMouseEvent *e) {
    int countHandleB = MultiSlider::positions().size();
    MultiSlider::mouseReleaseEvent(e);
    int countHandleE = MultiSlider::positions().size();
    if (countHandleB > countHandleE) {
        m_paramDlg.hide();
        deleteParam(m_currentHandle);
    }
}

void VariSliceSlider::mouseMoveEvent(QMouseEvent *e) {
    MultiSlider::mouseMoveEvent(e);

    if (m_currentHandle < 0)
        e->ignore();
    else
        paramStickOn(e, m_currentHandle);
}

void VariSliceSlider::mouseDoubleClickEvent(QMouseEvent *e) {
    MultiSlider::mouseDoubleClickEvent(e);
    addParam(MultiSlider::selectedHandle());
    m_currentHandle = MultiSlider::selectedHandle();
}

void VariSliceSlider::paramStickOn(QMouseEvent *e, int indexHandle) {
    currentZ = MultiSlider::position(indexHandle) * m_Precision;
    //qDebug() << "currentZ" << currentZ;

    QPoint moveToPos(e->globalX() - e->pos().rx() - m_paramDlg.width(),
                     e->globalY() - m_paramDlg.height() * 0.5);

    if (e->pos().ry() >= 0 && e->pos().ry() <= 777) {
        m_paramDlg.move(moveToPos);
    }

    MultiSliceSegment tempSegment = {currentZ, outlineColor};
    if (sliceSegments.size() - indexHandle > 0) {
        *(sliceSegments.begin() + indexHandle) = tempSegment;
    }

    m_dglHandle = indexHandle;
}

void VariSliceSlider::dlgGetData(int index) {
    //qDebug() << "dlgGetData----------------" << index;

    SliceParam tempUp = *(m_SliceParams.begin() + index + 1);
    SliceParam tempDown = *(m_SliceParams.begin() + index);

//    m_dataComBoxLayerH_Up->setValue(tempUp.layer_sliceheight);
//    m_dataComBoxTime_Up->setValue(tempUp.exposure_time);
//    m_dataComBoxLiftH_Up->setValue(tempUp.lift_distance);
//    m_dataComBoxLayerH_Down->setValue(tempDown.layer_sliceheight);
//    m_dataComBoxTime_Down->setValue(tempDown.exposure_time);
//    m_dataComBoxLiftH_Down->setValue(tempDown.lift_distance);
        m_dataComBoxLayerH_Up->setCurrentText(QString::number(tempUp.layer_sliceheight));
        m_dataComBoxTime_Up->setCurrentText(QString::number(tempUp.exposure_time));
        m_dataComBoxLiftH_Up->setCurrentText(QString::number(tempUp.lift_distance));
        m_dataComBoxLayerH_Down->setCurrentText(QString::number(tempDown.layer_sliceheight));
        m_dataComBoxTime_Down->setCurrentText(QString::number(tempDown.exposure_time));
        m_dataComBoxLiftH_Down->setCurrentText(QString::number(tempDown.lift_distance));
}

void VariSliceSlider::dlgSetData(int index) {
    //qDebug() << "dlgSetData----------------" << index;

    editParam(index);
}

void VariSliceSlider::addParam(int index) {
    if(index == -1){
        return;
    }

    //qDebug() << "addParam---------------->" << index;

    SliceParam currentAdd = *(m_SliceParams.begin() + index);
    m_SliceParams.insert(index, currentAdd);

    for (int i = 0; i < m_SliceParams.size(); i++) {
        //qDebug() << m_SliceParams.at(i).layer_sliceheight << m_SliceParams.at(i).exposure_time << m_SliceParams.at(i).lift_distance;
    }

    MultiSliceSegment currentSegment = {MultiSlider::position(index) * m_Precision, outlineColor};
    sliceSegments.insert(sliceSegments.begin() + index, currentSegment);
}

void VariSliceSlider::deleteParam(int index) {
    //qDebug() << "deleteParam---------------->" << index;

    if (m_SliceParams.size() - index > 0)
        m_SliceParams.erase(m_SliceParams.begin() + index);

    for (int i = 0; i < m_SliceParams.size(); i++) {
        //qDebug() << m_SliceParams.at(i).layer_sliceheight << m_SliceParams.at(i).exposure_time << m_SliceParams.at(i).lift_distance;
    }

    if (sliceSegments.size() - index > 0) {
        sliceSegments.erase(sliceSegments.begin() + index);
        //emit sliceSegmentsChanged();
        if(sliceSegments.size() > 0){
            currentZ = sliceSegments.at(sliceSegments.size() - 1).z;
        }
        else{
            currentZ = -1.0;// out of model
        }

    }
}

void VariSliceSlider::editParam(int index) {
    //qDebug() << "editParam---------------->" << index;

    SliceParam tempDown = {m_dataComBoxLayerH_Down->currentData().toFloat(), m_dataComBoxTime_Down->currentData().toInt(), m_dataComBoxLiftH_Down->currentData().toFloat()};
    SliceParam tempUp = {m_dataComBoxLayerH_Up->currentData().toFloat(), m_dataComBoxTime_Up->currentData().toInt(), m_dataComBoxLiftH_Up->currentData().toFloat()};

//    SliceParam tempDown = {m_dataComBoxLayerH_Down->value(), m_dataComBoxTime_Down->value(), m_dataComBoxLiftH_Down->value()};
//    SliceParam tempUp = {m_dataComBoxLayerH_Up->value(), m_dataComBoxTime_Up->value(), m_dataComBoxLiftH_Up->value()};
    if (m_SliceParams.size() - index > 0) {
        *(m_SliceParams.begin() + index) = tempDown;
        *(m_SliceParams.begin() + index + 1) = tempUp;
    }

    for (int i = 0; i < m_SliceParams.size(); i++) {
        //qDebug() << m_SliceParams.at(i).layer_sliceheight << m_SliceParams.at(i).exposure_time << m_SliceParams.at(i).lift_distance;
    }
}
