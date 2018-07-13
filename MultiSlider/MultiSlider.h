#ifndef _MULTISLIDER_H_
#define _MULTISLIDER_H_

#include <QSlider>
#include <QStyleOptionSlider>
#include <QStylePainter>
#include <QToolTip.h>
#include <qevent.h>
#include <QDebug>
#include <QtMath>
#include <QDialog>

#define cout qDebug() << "[" << __FILE__ << ":" << __LINE__ << "]"

class QStylePainter;
class MultiSlider;

class MultiSliderPrivate;

class MultiSlider : public QSlider {
    Q_OBJECT

    Q_PROPERTY(QVector<int> values READ values WRITE setValues NOTIFY valuesChanged)
    Q_PROPERTY(QVector<int> positions READ positions NOTIFY positionsChanged)
    Q_PROPERTY(int minimumRange READ minimumRange WRITE setMinimumRange NOTIFY minimumRangeChanged)
    Q_PROPERTY(QString handleToolTip READ handleToolTip WRITE setHandleToolTip)
    Q_PROPERTY(int count READ count WRITE setCount NOTIFY countChanged)
    Q_PROPERTY(int maxCount READ maxCount NOTIFY maxCountChanged)
    Q_PROPERTY(int selectedHandle READ selectedHandle WRITE selectHandle NOTIFY selectedHandleChanged)

    class MultiSliderPrivate : public QObject {
        Q_DECLARE_PUBLIC(MultiSlider)
    protected:
        MultiSlider *const q_ptr;

    public:
        MultiSliderPrivate(MultiSlider &object)
            : q_ptr(&object), _subclassClickOffset(0), _subclassPosition(0), _subclassWidth(0.0), _count(0), _maxCount(0), _minimumRange(0) {}

        void init() {
            Q_Q(MultiSlider);
            q->refreshMaxCount();
            q->connect(q, &MultiSlider::rangeChanged, q, &MultiSlider::onRangeChanged);
            q->connect(q, &MultiSlider::rangeChanged, q, &MultiSlider::refreshMaxCount);
            q->connect(q, &MultiSlider::countChanged, q, &MultiSlider::refreshMaxCount);
        }

        // function return first handle at given pos.
        // param[in]  pos given position
        // param[out] if handle was found param is equal to founded handle rect.
        // Otherwise return empty rect \return the handle at the given pos.
        // Otherwise return -1
        int handleAtPos(const QPoint &pos, QRect &handleRect) const {
            Q_Q(const MultiSlider);

            QStyleOptionSlider option;
            q->initStyleOption(&option);

            // The functinos hitTestComplexControl only know about 1 handle. As we
            // have d->m_count, we change the position of the handle and test if the
            // pos correspond to any of the 2 positions.

            for (int i = _count - 1; i >= 0; --i) {
                option.sliderPosition = this->_positions.at(i);
                option.sliderValue = this->_values.at(i);

                if (q->style()->hitTestComplexControl(QStyle::CC_Slider, &option, pos, q) == QStyle::SC_SliderHandle) {
                    handleRect = q->style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, q);
                    return i;
                }
            }
            return -1;
        }

        // posBetweenHandles checks that point is between two handles and
        // param point pos
        // return index of left handle, if not returns -1
        // modify: before begin = -1, after end = 999
        int posBetweenHandles(QPoint pos) const {
            Q_Q(const MultiSlider);
            int mepos = q->orientation() == Qt::Horizontal ? pos.x() : pos.y();

            QStyleOptionSlider option;
            q->initStyleOption(&option);
            // search in every sub-rect
            int minInAllCenter = 999;
            int maxInAllCenter = 0;
            for (int i = 0; i < _count - 1; ++i) {
                option.sliderPosition = _positions.at(i);
                option.sliderValue = _values.at(i);
                QRect handleRect = q->style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, q);
                option.sliderPosition = _positions.at(i + 1);
                option.sliderValue = _values.at(i + 1);
                QRect nextHandleRect = q->style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, q);
                if (q->style()->objectName() == "macintosh") {
                    handleRect = AdjustRectForMac(handleRect);
                    nextHandleRect = AdjustRectForMac(nextHandleRect);
                }
                int minCenter = (q->orientation() == Qt::Horizontal ? handleRect.center().x() : nextHandleRect.center().y());
                int maxCenter = (q->orientation() == Qt::Horizontal ? nextHandleRect.center().x() : handleRect.center().y());
                minInAllCenter = minInAllCenter > minCenter ? minCenter : minInAllCenter;
                maxInAllCenter = maxInAllCenter < maxCenter ? maxCenter : maxInAllCenter;
                if (mepos > minCenter && mepos < maxCenter) {
                    return i;
                }
            }
            if (mepos < minInAllCenter && _count > 0){
                return 999;
            }
//            else if (mepos > maxCenter){
//                return -1;
//            }
            return -1; // none handle or before begin
        }

        // Copied verbatim from QSliderPrivate class (see QSlider.cpp)
        int pixelPosToRangeValue(int pos) const {
            Q_Q(const MultiSlider);
            QStyleOptionSlider option;
            q->initStyleOption(&option);

            QRect gr = q->style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderGroove, q);
            QRect sr = q->style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, q);
            int sliderMin, sliderMax, sliderLength;
            if (option.orientation == Qt::Horizontal) {
                sliderLength = sr.width();
                sliderMin = gr.x();
                sliderMax = gr.right() - sliderLength + 1;
            } else {
                sliderLength = sr.height();
                sliderMin = gr.y();
                sliderMax = gr.bottom() - sliderLength + 1;
            }

            return QStyle::sliderValueFromPosition(q->minimum(), q->maximum(), pos - sliderMin, sliderMax - sliderMin, option.upsideDown);
        }

        int pixelPosFromRangeValue(int val) const {
            Q_Q(const MultiSlider);
            QStyleOptionSlider option;
            q->initStyleOption(&option);

            QRect gr = q->style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderGroove, q);
            QRect sr = q->style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, q);
            int sliderMin, sliderMax, sliderLength;
            if (option.orientation == Qt::Horizontal) {
                sliderLength = sr.width();
                sliderMin = gr.x();
                sliderMax = gr.right() - sliderLength + 1;
            } else {
                sliderLength = sr.height();
                sliderMin = gr.y();
                sliderMax = gr.bottom() - sliderLength + 1;
            }

            return QStyle::sliderPositionFromValue(q->minimum(), q->maximum(), val, sliderMax - sliderMin, option.upsideDown) + sliderMin;
        }

        // draw slider handle
        // param[in]  num handle number from left to right
        // param[in]  painter painter to draw handle
        void drawHandle(int num, QStylePainter *painter) const {
            Q_Q(const MultiSlider);
            QStyleOptionSlider option;
            q->initSliderStyleOption(num, &option);

            option.subControls = QStyle::SC_SliderHandle;
            option.sliderValue = _values.at(num);
            option.sliderPosition = _positions.at(num);

            if (_selectedHandles.contains(num)) {
                option.activeSubControls = QStyle::SC_SliderHandle;
                option.state |= QStyle::State_Sunken;
            }
            if (q->style()->objectName() == "macintosh") {
                // On mac style, drawing just the handle actually draws also the groove.
                QRect clip = q->style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, q);
                clip = AdjustRectForMac(clip);
                QString path = QString(":/res/icon/knob") + (_selectedHandles.contains(num) ? "_selected" : "") + ".png";
                painter->drawPixmap(clip, QPixmap(path));
            } else {
                painter->drawComplexControl(QStyle::CC_Slider, option);

//                QRect clip = q->style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, q);
//                painter->fillRect(clip, Qt::transparent);
//                QString path = QString(":/res/icon/add") + ".png";
//                painter->drawPixmap(clip, QPixmap(path));

                QRect rect = q->style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, q);
                QColor color_handle(Qt::lightGray);
                //painter->fillRect(rect,color_handle);

                // draw slider Position tag on slider left
                int offset = 6;
                QPoint tickPts_position[] = {
                    QPoint(rect.left() + offset*4,rect.bottom() + offset),
                    QPoint(rect.left() + offset*4,rect.bottom() - offset*3),
                    QPoint(rect.left(),rect.bottom() - offset*3),
                    QPoint(rect.left(),rect.bottom() + offset),
                };
                QPen pen(color_handle);
                painter->setPen(pen);
                QBrush brush(color_handle);
                painter->setBrush(brush);
                painter->drawPolygon(tickPts_position, 4);
                QFont font;
                font.setPointSize(10);
                painter->setFont(font);
                QString sliderNum = QString::number(option.sliderPosition, 10);
                pen.setColor(Qt::black);
                painter->setPen(pen);
                painter->drawText(rect.left() + offset, rect.bottom(), sliderNum);

                // draw shutdown tag on slider right
                offset = 8;
                QPoint tickPts_shutdown(rect.right() - offset + 1, (rect.bottom()+rect.top())/2);
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
        }

        // Values on slider
        QVector<int> _values;

        // Positions on slider.
        QVector<int> _positions;

        // See QSliderPrivate::clickOffset.
        // Overrides this var
        int _subclassClickOffset;

        // See QSliderPrivate::position
        // Overrides this var.
        int _subclassPosition;

        // Original width between the 2 bounds before any moves
        float _subclassWidth;

        // vector of selected handles numbers
        QVector<int> _selectedHandles;

        // tooltip to be displayed on handle
        QString _handleToolTip;

        // count of slider handles
        int _count;
        // maximum available count of handles
        int _maxCount;
        // minimum range between handles
        int _minimumRange;

    private:
        static QRect AdjustRectForMac(const QRect &rect) {
            return rect.adjusted(3, 2, -3, -2);
        }

    private:
        Q_DISABLE_COPY(MultiSliderPrivate)
    };

public:
    typedef QSlider SuperClass;

    static QColor color(int index, double bright) {
        return QColor::fromHslF((index % 7) * 1.0 / 7, 1, bright);
    }

    explicit MultiSlider(Qt::Orientation o, QWidget *parent = nullptr) : QSlider(o, parent), d_ptr(new MultiSliderPrivate(*this)) {
        Q_D(MultiSlider);
        d->init();
    }

    explicit MultiSlider(QWidget *parent = nullptr) : QSlider(parent), d_ptr(new MultiSliderPrivate(*this)) {
        Q_D(MultiSlider);
        d->init();
    }

    ~MultiSlider() = default;

    // check that handle at given index is down
    // param index    index of handle from left to right
    // return handle selected state
    bool isHandleDown(int index) const {
        Q_D(const MultiSlider);
        return d->_selectedHandles.contains(index);
    }

    // holds the slider's current values.
    // return slider current values for all handles.
    QVector<int> values() const {
        Q_D(const MultiSlider);
        return d->_values;
    }

    // holds the current slider positions.
    // return slider current positions for all handles.
    QVector<int> positions() const {
        Q_D(const MultiSlider);
        return d->_positions;
    }

    // position at given index.
    int position(int index) const {
        Q_D(const MultiSlider);
        Q_ASSERT(index >= 0);
        Q_ASSERT(index < d->_count);
        return d->_positions.at(index);
    }

    // value at given index.
    int value(int index) const {
        Q_D(const MultiSlider);
        Q_ASSERT(index >= 0);
        Q_ASSERT(index < d->_count);
        return d->_values.at(index);
    }

    // return first selected handle
    // return if handle was selected by mouse or by focus - return first of
    // selected handles from left to right. Otherwise return -1.
    int selectedHandle() const {
        Q_D(const MultiSlider);
        return d->_selectedHandles.isEmpty() ? -1 : d->_selectedHandles.first();
    }

    // set absolute position at given index
    // note this function can change other positions
    // note if tracking is enabled (the default), this is identical to setValue
    // param index    number of handle from left to right
    // param arg  absolute value of new position
    void setPosition(int index, int arg) {
        Q_D(MultiSlider);
        Q_ASSERT(index >= 0);
        Q_ASSERT(index <= d->_count);
        if (d->_positions.at(index) != arg) {
            //arg is new, d->_positions.at(index) is previous, every 1 point changed
            movePosition(index, arg - d->_positions.at(index));
            emit positionsChanged(d->_positions);
            if (hasTracking()) {
                d->_values = d->_positions;
                emit valuesChanged(d->_values);
            }
            update();
        }
    }

    // set absolute value at given index
    // note this function can change other values
    // note this function also will set position at given index
    // param index    number of handle from left to right
    // param arg  absolute value of new value
    void setValue(int index, int arg) {
        Q_D(MultiSlider);
        Q_ASSERT(index >= 0);
        Q_ASSERT(index <= d->_count);
        if (d->_values.at(index) != arg) {
            setPosition(index, arg);
            d->_values = d->_positions;
            emit valuesChanged(d->_values);
            update();
        }
    }

    // set absolute values
    // note this function will override
    // note this function also will change all positions
    // note if values count must be equeal to slider count
    // param values  absolute values
    void setValues(QVector<int> values) {
        Q_D(MultiSlider);
        Q_ASSERT(d->_values.size() == this->count());
        if (d->_values != values) {
            d->_positions = values;
            d->_values = values;
            normalize(false);
            emit positionsChanged(d->_positions);
            emit valuesChanged(d->_values);
            update();
        }
    }

    // returns current tooltip
    QString handleToolTip() const {
        Q_D(const MultiSlider);
        return d->_handleToolTip;
    }

    // Controls the text to display for the handle tooltip. It is in
    // addition to the widget tooltip. \note "%1" is replaced by the current
    // value of the slider. \note Empty string (by default) means no tooltip.
    void setHandleToolTip(const QString &toolTip) {
        Q_D(MultiSlider);
        d->_handleToolTip = toolTip;
    }

    // This property holds the current sliders count.
    int count() const {
        Q_D(const MultiSlider);
        return d->_count;
    }

    // this property holds minimum range between two slider handles
    int minimumRange() const {
        Q_D(const MultiSlider);
        return d->_minimumRange;
    }

    // this property holds maximum count of sliders
    int maxCount() const {
        Q_D(const MultiSlider);
        return d->_maxCount;
    }

Q_SIGNALS:
    // This signal is emitted when the slider values has changed.
    // param new slider values.
    void valuesChanged(QVector<int> arg);

    // This signal is emitted when the slider moves.
    // param new slider positions.
    // This signal is emitted even when tracking is turned off.
    void positionsChanged(QVector<int> arg);

    // this signal is emitted when the sliders count changed
    // param The argument is the new sliders count.
    void countChanged(int arg);

    // this signal is emitted whem minimum range between handles changed
    // param The argument is the new value of minimum range between two handles.
    void minimumRangeChanged(uint arg);

    // this signal is emitted when maximum count of handles on slider
    // changed \param The argument is the new maximum count of slider handles
    void maxCountChanged(int arg);

    // this signal is emitted when new handle selected
    // param The argument is the new maximum count of slider handles
    void selectedHandleChanged(int arg);

public Q_SLOTS:
    // This property holds the slider's count.
    // param argument is count to set.
    // note arg wil be cutted to maximumCount value
    void setCount(int arg) {
        Q_D(MultiSlider);
        Q_ASSERT(arg >= 0);
        Q_ASSERT(arg < maxCount());
        if (d->_count == arg) {
            return;
        }
        if (arg > d->_count) {
            addToTop(arg - d->_count);
        } else {
            removeFromTop(d->_count - arg);
        }
    }

    // add handles to the left.
    // param count    count of handles to be added
    // note total count can not be more than maximumCount
    void addToLeft(int count) {
        Q_D(MultiSlider);
        if (d->_count == maxCount()) {
            return;
        }
        count = qMin(count, maxCount() - d->_count);
        int minDist = d->_minimumRange;
        if (0 == minDist) {
            if (d->_positions.isEmpty()) {
                minDist = (maximum() - minimum()) / 5;
            } else {
                minDist = (d->_positions.first() - minimum()) / 5;
            }
        }
        if (d->_count && d->_positions.first() - minimum() < minDist * 2) // margin is min range
        {
            movePosition(0, count * minDist);
        }
        for (int i = 0; i < count; ++i) {
            int position = i * minDist + minimum() + minDist;
            d->_count++;
            d->_positions.insert(i, position);
            d->_values.insert(i, position);
        }
        emit countChanged(d->_count);
        normalize(true);
        update();
    }

    // add one handle to the left.
    // note total count can not be more than maximumCount
    void addOneToleft() {
        addToLeft(1);
    }

    // add handles to the right.
    // param count    count of handles to be added
    // note total count can not be more than maximumCount
    void addToTop(int count) {
        Q_D(MultiSlider);
        if (d->_count == maxCount()) {
            return;
        }
        count = qMin(count, maxCount() - d->_count);
        int minDist = d->_minimumRange;
        if (0 == minDist) {
            if (d->_positions.isEmpty()) {
                minDist = (maximum() - minimum()) / 5;
            } else {
                minDist = (maximum() - d->_positions.last()) / 5;
            }
        }
        if (d->_count && maximum() - d->_positions.last() < minDist * 2) // margin is min range
        {
            movePosition(d->_count - 1, -1 * count * minDist);
        }
        int startPos = d->_count != 0 ? (d->_positions.last() + minDist) : minimum();
        for (int i = 0; i < count; ++i) {
            int position = i * minDist + startPos;
            d->_positions.insert(d->_count, position);
            d->_values.insert(d->_count, position);
            d->_count++;
        }
        emit countChanged(d->_count);
        normalize(true);
        update();
    }

    // add one handle at any position
    void addOneToPosition(int index, int insertPosition) {
        Q_D(MultiSlider);
        if (d->_count == maxCount()) {
            return;
        }
        // position = value
        if (-1 == index){
            d->_positions.insert(0, insertPosition);
            d->_values.insert(0, insertPosition);
            d->_count++;
        }
        else if (999 == index){
            d->_positions.insert(d->_count, insertPosition);
            d->_values.insert(d->_count, insertPosition);
            d->_count++;
        }
        else {
            d->_positions.insert(index + 1, insertPosition);
            d->_values.insert(index + 1, insertPosition);
            d->_count++;
        }
        emit countChanged(d->_count);
        normalize(true);
        update();
    }

    // add one handle to the right.
    // note total count can not be more than maximumCount
    void addOneToTop() {
        addToTop(5);
    }

    // remove handles from the left.
    // param count    count of handles to be removed
    void removeFromLeft(int count) {
        Q_D(MultiSlider);
        count = qMin(d->_count, count);
        for (int i = 0; i < count; ++i) {
            d->_count--;
            d->_positions.removeFirst();
            d->_values.removeFirst();
        }
        emit countChanged(d->_count);
        normalize(true);
        update();
    }

    // remove one handle from the left.
    void removeOneFromLeft() {
        removeFromLeft(1);
    }

    // remove handles from the top.
    // param count    count of handles to be removed
    void removeFromTop(int count) {
        Q_D(MultiSlider);
        count = qMin(d->_count, count);
        for (int i = 0; i < count; ++i) {
            d->_count--;
            d->_positions.removeLast();
            d->_values.removeLast();
        }
        emit countChanged(d->_count);
        normalize(true);
        update();
    }

    // remove one handle from the top.
    void removeOneFromTop() {
        removeFromTop(1);
    }

    // remove all handles
    void removeAll() {
        Q_D(MultiSlider);
        int count = d->_count;
        for (int i = 0; i < count; ++i) {
            d->_count--;
            d->_positions.removeLast();
            d->_values.removeLast();
        }
        emit countChanged(d->_count);
        normalize(true);
        update();
    }

    // remove one handle
    void removeonehandle(int index) {
        Q_D(MultiSlider);
        if(d->_count - index > 0){
            d->_count--;
            d->_positions.erase(d->_positions.begin() + index);
            d->_values.erase(d->_values.begin() + index);
        }
        emit countChanged(d->_count);
        //normalize(true);
        update();
    }

    // this function will hold minimum range between two closest handles
    // param arg  number of positions between thwo closest handles
    void setMinimumRange(int arg) {
        Q_D(MultiSlider);
        if (d->_minimumRange == arg) {
            return;
        }

        d->_minimumRange = arg;
        refreshMaxCount();
        normalize(true);
        emit minimumRangeChanged(arg);
        update();
    }

    // clear selection and mark given handle as selected
    void selectHandle(int handle) {
        Q_D(MultiSlider);
        Q_ASSERT(handle >= 0);
        Q_ASSERT(handle < d->_count);
        if (!d->_selectedHandles.contains(handle)) {
            d->_selectedHandles.clear();
            d->_selectedHandles.push_back(handle);
            emit selectedHandleChanged(handle);
            update();
        }
    }

    // clear selection and mark given handle as selected
    void selectTwoHandles(int firstHandle, int secondHandle) {
        Q_D(MultiSlider);
        Q_ASSERT(firstHandle >= 0 && secondHandle >= 1);
        Q_ASSERT(firstHandle < d->_count - 1);
        Q_ASSERT(secondHandle < d->_count);
        if (!d->_selectedHandles.contains(firstHandle) || !d->_selectedHandles.contains(secondHandle)) {
            d->_selectedHandles.clear();
            d->_selectedHandles.push_back(firstHandle);
            d->_selectedHandles.push_back(secondHandle);
            emit selectedHandleChanged(firstHandle);
            update();
        }
    }

protected Q_SLOTS:
    // recalculate maximum count and cure extra handles from right
    void refreshMaxCount() {
        Q_D(MultiSlider);
        int newMaxCount =
            d->_minimumRange ? ((maximum() - minimum()) / d->_minimumRange - 1) : std::numeric_limits<int>::max(); // first position can be zero and
                                                                                                                   // we set margin as
                                                                                                                   // d->m_minimumRange
        if (d->_maxCount != newMaxCount) {
            d->_maxCount = newMaxCount;
            if (d->_count > d->_maxCount) {
                setCount(d->_maxCount);
            }
            emit maxCountChanged(d->_maxCount);
        }
    }

    // check that old positions not outside range
    void onRangeChanged(int /*min*/, int /*max*/) {
        normalize(true);
    }

protected:
    MultiSlider(MultiSliderPrivate *impl, Qt::Orientation o, QWidget *parent = nullptr) : QSlider(o, parent), d_ptr(impl) {
        Q_D(MultiSlider);
        d->init();
    }

    MultiSlider(MultiSliderPrivate *impl, QWidget *parent = nullptr) : QSlider(parent), d_ptr(impl) {
        Q_D(MultiSlider);
        d->init();
    }

    // normalize positions
    void normalize(bool emitIfChanged) {
        Q_D(MultiSlider);
        if (d->_count == 0) {
            return;
        }
        QVector<int> oldPositions = d->_positions;
        for (int i = -1; i < d->_count - 1; ++i) // do not check last element
        {
            int diff = i == -1 ? d->_positions.first() - minimum() : d->_positions.at(i + 1) - d->_positions.at(i);
            if (diff < d->_minimumRange) {
                movePosition(i + 1, d->_minimumRange - diff);
            }
        }
        for (int i = d->_count; i > 0; --i) {
            int diff = i == d->_count ? maximum() - d->_positions.last() : d->_positions.at(i) - d->_positions.at(i - 1);
            if (diff < d->_minimumRange) {
                movePosition(i - 1, d->_minimumRange - diff);
            }
        }
        if (emitIfChanged && d->_positions != oldPositions) {
            emit positionsChanged(d->_positions);
            if (hasTracking()) {
                d->_values = d->_positions;
                emit valuesChanged(d->_values);
            }
        }
    }

    virtual void mousePressEvent(QMouseEvent *e) override {
        Q_D(MultiSlider);
        if (minimum() == maximum() || (e->buttons() ^ e->button())) {
            e->ignore();
            return;
        }
        int mepos = this->orientation() == Qt::Horizontal ? e->pos().x() : e->pos().y();

        QStyleOptionSlider option;
        this->initStyleOption(&option);

        QRect handleRect;
        int handle = d->handleAtPos(e->pos(), handleRect);
        cout << "pressed index = " << handle << "handle";

        if (handle != -1) {
            d->_subclassPosition = d->_positions.at(handle);

            // save the position of the mouse inside the handle for later
            d->_subclassClickOffset = mepos - (this->orientation() == Qt::Horizontal ? handleRect.left() : handleRect.top());

            this->setSliderDown(true);
            selectHandle(handle);

            // Accept the mouseEvent
            e->accept();
            return;
        }

        // if we are here, no handles have been pressed
        // Check if we pressed on the groove between the 2 handles

        QStyle::SubControl control = this->style()->hitTestComplexControl(QStyle::CC_Slider, &option, e->pos(), this);
        int index = d->posBetweenHandles(e->pos());
        cout << "between index and index+1: index = " << index;
        if (control == QStyle::SC_SliderGroove && -1 != index && index < 999) {
            // warning lost of precision it might be fatal
            d->_subclassPosition = (d->_positions.at(index) + d->_positions.at(index + 1)) / 2.;
            d->_subclassClickOffset = mepos - d->pixelPosFromRangeValue(d->_subclassPosition);
            d->_subclassWidth = (d->_positions.at(index + 1) - d->_positions.at(index)) / 2.;
            this->setSliderDown(true);
            if (!this->isHandleDown(index) || !this->isHandleDown(index + 1)) {
                d->_selectedHandles.clear();
                selectTwoHandles(index, index + 1);
            }
            e->accept();
            return;
        }

        e->ignore();
    }

    virtual void mouseMoveEvent(QMouseEvent *e) override {
        Q_D(MultiSlider);
        if (d->_selectedHandles.isEmpty()) {
            e->ignore();
            return;
        }
        int mepos = this->orientation() == Qt::Horizontal ? e->pos().x() : e->pos().y();

        QStyleOptionSlider option;
        this->initStyleOption(&option);

        int newPosition = d->pixelPosToRangeValue(mepos - d->_subclassClickOffset);
        switch (d->_selectedHandles.size()) {
        case 1:
            setPosition(d->_selectedHandles.first(), newPosition);
            break;
        case 2:
            //setPosition(d->_selectedHandles.at(0), newPosition - static_cast<int>(d->_subclassWidth));
            //setPosition(d->_selectedHandles.at(1), newPosition + static_cast<int>(d->_subclassWidth + .5));
            break;
        default:
            Q_ASSERT(!"error selected handles count");
            break;
        }
        e->accept();
    }

    virtual void mouseReleaseEvent(QMouseEvent *e) override {
        Q_D(MultiSlider);
        this->QSlider::mouseReleaseEvent(e);

        // if clicked on the shutdowm position, delete handle
        // note not happen in mousePressEvent
        QRect handleRect;
        int handle = d->handleAtPos(e->pos(), handleRect);
        if (handle != -1) {
            int offset = 8;
            int left = handleRect.right()-offset*2;
            int right = handleRect.right();
            int bottom = handleRect.bottom();
            int top = handleRect.top();
            int x = e->x();
            int y = e->y();
            if(x>=left && x<=right && y>=top && y<=bottom){
                removeonehandle(handle);
            }
        }

        setSliderDown(false);
        d->_selectedHandles.clear();
        if (d->_values != d->_positions) {
            d->_values = d->_positions;
            emit valuesChanged(d->_values);
        }
        update();
    }

    virtual void mouseDoubleClickEvent(QMouseEvent *e) override {
        Q_D(MultiSlider);

        QRect handleRect;
        int handle = d->handleAtPos(e->pos(), handleRect);

        if (handle != -1){
            cout << "double clicked handle" << handle;
            // todo...
            QDialog test;
            test.setWindowTitle("slicer info");
            test.exec();
        }
        else{
            int insertIndex = d->posBetweenHandles(e->pos());
            int mouse_Y = e->pos().ry();
            int sliderBottom = this->rect().bottom(); // max
            int sliderTop = this->rect().top(); // min = 0
            int insertValue = maximum() - (maximum() * (mouse_Y - sliderTop)) / (sliderBottom - sliderTop);
            addOneToPosition(insertIndex, insertValue);
        }
    }

    virtual void paintEvent(QPaintEvent *ev) override {
        Q_D(MultiSlider);
        QStyleOptionSlider option;
        this->initStyleOption(&option);
        QStylePainter painter(this);
        option.subControls = QStyle::SC_SliderGroove;
        if (this->tickPosition() != NoTicks) {
            option.subControls |= QStyle::SC_SliderTickmarks;
        }
        // Move to minimum to not highlight the SliderGroove.
        // On mac style, drawing just the slider groove also draws the handles,
        // therefore we give a negative (outside of view) position.
        option.sliderValue = this->minimum() - this->maximum(); //0 - 99
        option.sliderPosition = this->minimum() - this->maximum();
        painter.drawComplexControl(QStyle::CC_Slider, option);

        int pos = minimum();
        int nextPos = d->_count ? d->_positions.at(0) : maximum();
        drawColoredRect(pos, nextPos, painter, color(0, 0.5));

        if (d->_count == 0) {
            return;
        }
        for (int i = 0; i < d->_count - 1; ++i) {
            pos = d->_positions.at(i);
            nextPos = d->_positions.at(i + 1);
            drawColoredRect(pos, nextPos, painter, color(i + 1, 0.5));
        }
        pos = d->_positions.last();
        nextPos = maximum();
        drawColoredRect(pos, nextPos, painter, color(d->_count + 1, 0.5));

        for (int i = 0; i < d->_count; ++i) {
            d->drawHandle(i, &painter);
        }
    }

    virtual void initSliderStyleOption(int num, QStyleOptionSlider *option) const {
        Q_UNUSED(num);
        this->initStyleOption(option);
    }

    virtual bool event(QEvent *e) override {
        Q_D(MultiSlider);
        switch (e->type()) {
        case QEvent::ToolTip: {
            QHelpEvent *helpEvent = static_cast<QHelpEvent *>(e);
            QRect rect;
            int handle = d->handleAtPos(helpEvent->pos(), rect);
            if (handle != -1) {
                QToolTip::showText(helpEvent->globalPos(), d->_handleToolTip.arg(d->_positions.at(handle)));
                e->accept();
            }
        }
        default:
            break;
        }
        return this->SuperClass::event(e);
    }

protected:
    QScopedPointer<QObject> d_ptr;

private:
    void movePosition(int index, int arg) {
        Q_D(MultiSlider);
        Q_ASSERT(index >= 0);
        Q_ASSERT(index < d->_count);
        int position = d->_positions.at(index);
        if (index == (arg < 0 ? 0 : (d->_count - 1))) {
            // first and last handle
            d->_positions.replace(index, qBound(minimum() + minimumRange(), position + arg, maximum() - minimumRange()));
        } else {
            // other mid handles
            // down:-1 up:1
            int nextStep = arg < 0 ? -1 : 1;
            int minimumRange = d->_minimumRange * nextStep;

            int nextPosition = d->_positions.at(index + nextStep);
            if (qAbs(arg) < qAbs(position - nextPosition + minimumRange)) {
                d->_positions.replace(index, position + arg);
            } else {
                // note superposition
                //movePosition(index + nextStep, arg - (nextPosition - position) + minimumRange);
                //nextPosition = d->_positions.at(index + nextStep);
                //d->_positions.replace(index, nextPosition - minimumRange);
            }
        }
        update();
    }

    void drawColoredRect(int pos, int nextPos, QStylePainter &painter, QColor highlight) {
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
            groove = QRect(QPoint(groove.center().x() - 15, qMin(lr.center().y(), ur.center().y() - 4)), //topleft
                           QPoint(groove.center().x() + 14, qMax(lr.center().y(), ur.top()) + 3)); //bottomright
        }

        painter.setPen(QPen(highlight.darker(300), 0));
        painter.setBrush(highlight);
        painter.drawRect(groove);

        //cout << this->pos().rx();
        //cout << this->pos().ry(); //25
        //cout << this->width(); //80
        //cout << this->height();
    }

    Q_DECLARE_PRIVATE(MultiSlider)
    Q_DISABLE_COPY(MultiSlider)
};

#endif // _MULTISLIDER_H_
