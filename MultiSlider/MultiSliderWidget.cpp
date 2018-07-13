#include "MultiSliderWidget.h"
#include "MultiSlider.h"

#include <QKeyEvent>
#include <QPushButton>
#include <QSpinBox>
#include <QStyleOptionSpinBox>
#include <QVBoxLayout>

class MultiSliderWidget::SpinBox : public QSpinBox {
public:
    SpinBox(int index, QWidget *parent = 0);
    int index() const;

protected:
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private:
    int m_index;
};

MultiSliderWidget::SpinBox::SpinBox(int index_, QWidget *parent) : QSpinBox(parent), m_index(index_) {}

int MultiSliderWidget::SpinBox::index() const {
    return m_index;
}

void MultiSliderWidget::SpinBox::focusInEvent(QFocusEvent *event) {
    selectAll();
    setStyleSheet("background-color: " + MultiSlider::color(m_index, 0.93).name() + ";");
    QSpinBox::focusInEvent(event);
}

void MultiSliderWidget::SpinBox::focusOutEvent(QFocusEvent *event) {
    setStyleSheet("background-color: white;");
    QSpinBox::focusOutEvent(event);
}

MultiSliderWidget::MultiSliderWidget(QWidget *parent) : QFrame(parent) {
    initMultiSlider();
    labelsLayout = new QVBoxLayout();
    labelsLayout->setSpacing(10);
    createWidget();

    setLayout(widgetLayout);

    setLabelsUnder(true);
    onSliderCountChanged(multiSlider->count());
    onSelectedHandleChanged(multiSlider->selectedHandle());
    onSliderPositionsChanged(multiSlider->positions());
    installEventFilter(this);

    onLabelsUnderChanged();

    updateButtonsEnable();
}

MultiSlider *MultiSliderWidget::GetMultiSlider() {
    return multiSlider;
}

QPushButton *MultiSliderWidget::GetButtonAddToLeft() {
    return addToLeftButton;
}

QPushButton *MultiSliderWidget::GetButtonAddToTop() {
    return addToTopButton;
}

QPushButton *MultiSliderWidget::GetButtonRemoveFromLeft() {
    return removeFromLeftButton;
}

QPushButton *MultiSliderWidget::GetButtonRemoveFromRight() {
    return removeFromRightButton;
}

bool MultiSliderWidget::eventFilter(QObject *obj, QEvent *event) {
    Q_UNUSED(obj);
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        SpinBox *spinBox = dynamic_cast<SpinBox *>(obj);
        int handle = -1;
        if (nullptr != spinBox) {
            handle = spinBox->index();
            if (handle >= multiSlider->count()) {
                handle = multiSlider->count() - 1;
            }
        } else {
            handle = multiSlider->selectedHandle();
        }
        switch (keyEvent->key()) {
        default:
            return false;
        case Qt::Key_Tab:
            selectNextSpinBox();
            break;
        case Qt::Key_Left:
            if (handle >= 0) {
                multiSlider->setPosition(handle, multiSlider->position(handle) - 1);
            }
            break;
        case Qt::Key_Right:
            if (handle >= 0) {
                multiSlider->setPosition(handle, multiSlider->position(handle) + 1);
            }
            break;
        }
        return true;
    }
    return false;
}

void MultiSliderWidget::createWidget() {
    widgetLayout = new QVBoxLayout();
    widgetLayout->setContentsMargins(5, 25, 5, 25);
    widgetLayout->setSpacing(50);
    QHBoxLayout *vLayout = new QHBoxLayout();
    vLayout->setSpacing(0);

    auto CreateFlatButton = [](QString iconAddr) -> QPushButton * {
        QPushButton *button = new QPushButton(QIcon(iconAddr), "");
        button->setFixedWidth(button->iconSize().width());
        button->setFlat(true);
        return button;
    };

//    addToLeftButton = CreateFlatButton(":/res/icon/add.png");
//    connect(addToLeftButton, &QPushButton::clicked, multiSlider, &MultiSlider::addOneToleft);
//    vLayout->addWidget(addToLeftButton);
//    removeFromLeftButton = CreateFlatButton(":/res/icon/remove.png");
//    vLayout->addWidget(removeFromLeftButton);
//    connect(removeFromLeftButton, &QPushButton::clicked, multiSlider, &MultiSlider::removeOneFromLeft);
//    vLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed));
//    widgetLayout->addLayout(vLayout);

    sliderLayout = new QHBoxLayout();
    sliderLayout->setSpacing(100);
    widgetLayout->addLayout(sliderLayout);

    //vLayout = new QHBoxLayout();
    addToTopButton = CreateFlatButton(":/res/icon/add.png");
    connect(addToTopButton, &QPushButton::clicked, multiSlider, &MultiSlider::addOneToTop); // add 5 handles for testing
    vLayout->addWidget(addToTopButton);

//    removeFromRightButton = CreateFlatButton(":/res/icon/knob.png");
//    connect(removeFromRightButton, &QPushButton::clicked, multiSlider, &MultiSlider::removeOneFromTop);
//    vLayout->addWidget(removeFromRightButton);
//    removeAllButton = CreateFlatButton(":/res/icon/remove.png");
//    connect(removeAllButton, &QPushButton::clicked, multiSlider, &MultiSlider::removeAll);
//    vLayout->addWidget(removeAllButton);

    vLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed));
    widgetLayout->addLayout(vLayout);
}

MultiSliderWidget::SpinBox *MultiSliderWidget::createSpinBox(int index) {
    SpinBox *spinBox = new SpinBox(index, this);
    spinBox->setMinimum(multiSlider->minimum());
    spinBox->setMaximum(multiSlider->maximum());
    spinBox->installEventFilter(this);
    spinBox->setKeyboardTracking(false);
    spinBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    spinBox->setAlignment(Qt::AlignCenter);
    return spinBox;
}

void MultiSliderWidget::initMultiSlider() {
    multiSlider = new MultiSlider(Qt::Vertical, this);
    multiSlider->setFixedWidth(80);
    //i like three. Three is a good number:)
    //just a joke. Three used because it enough to show functional and not too high to overload ui
    multiSlider->setCount(0);
    multiSlider->installEventFilter(this);
    connect(multiSlider, &MultiSlider::selectedHandleChanged, this, &MultiSliderWidget::onSelectedHandleChanged);
    connect(multiSlider, &MultiSlider::positionsChanged, this, &MultiSliderWidget::onSliderPositionsChanged);
    connect(multiSlider, &MultiSlider::countChanged, this, &MultiSliderWidget::updateButtonsEnable);
    connect(multiSlider, &MultiSlider::maxCountChanged, this, &MultiSliderWidget::updateButtonsEnable);
    connect(multiSlider, &MultiSlider::countChanged, this, &MultiSliderWidget::onSliderCountChanged);
    connect(multiSlider, &MultiSlider::rangeChanged, this, &MultiSliderWidget::onSliderRangeChanged);
}

void MultiSliderWidget::updateButtonsEnable() {
    int count = multiSlider->count();
    int maxcount = multiSlider->maxCount();
    addToTopButton->setEnabled(count < maxcount);
//    addToLeftButton->setEnabled(count < maxcount);
//    removeFromRightButton->setEnabled(count > 0);
//    removeFromLeftButton->setEnabled(count > 1);
}

void MultiSliderWidget::onSliderCountChanged(int count) {
    int spinBoxesCount = count;
    if (_showDifferences && count > 0) {
        spinBoxesCount++; //add extra spinbox for distance between maximum and last value
    }
    while (spinBoxes.size() > spinBoxesCount) //remove extra spinbox
    {
        delete spinBoxes.takeLast();
    }
    while (spinBoxes.size() < spinBoxesCount) {
        SpinBox *spinBox = createSpinBox(spinBoxes.size());

        connect(spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MultiSliderWidget::onSpinBoxValueChanged);

        spinBoxes.append(spinBox);
        labelsLayout->insertWidget(labelsLayout->count(), spinBox);
    }
    onSliderPositionsChanged(multiSlider->positions());
}

void MultiSliderWidget::onSelectedHandleChanged(int handle) {
    if (handle == -1) {
        return;
    }
    spinBoxes.at(handle)->setFocus();
}

void MultiSliderWidget::onSliderPositionsChanged(QVector<int> values) {
    if (multiSlider->count() == 0) {
        return;
    }
    for (auto spinBox : spinBoxes) {
        spinBox->blockSignals(true);
    }
    for (int i = 0; i < values.size(); i++) {
        updateSpinBoxValue(i);
    }
    if (_showDifferences) {
        spinBoxes.last()->setValue(multiSlider->maximum() - (multiSlider->count() ? multiSlider->positions().last() : multiSlider->minimum()));
    }
    for (auto spinBox : spinBoxes) {
        spinBox->blockSignals(false);
    }
}

void MultiSliderWidget::onSliderRangeChanged(int min, int max) {
    for (auto spinBox : spinBoxes) {
        spinBox->blockSignals(true);
        spinBox->setMinimum(min);
        spinBox->setMaximum(max);
        spinBox->blockSignals(false);
    }
}

void MultiSliderWidget::onSpinBoxValueChanged(int value) {
    Q_ASSERT(dynamic_cast<SpinBox *>(sender()) != nullptr);
    SpinBox *spinBox = static_cast<SpinBox *>(sender());
    Q_ASSERT(spinBox != nullptr);
    int index = spinBox->index();
    if (_showDifferences) {
        if (index == multiSlider->count()) {
            multiSlider->setValue(multiSlider->count() - 1, multiSlider->maximum() - value);
        } else {
            int sum = 0;
            for (int i = 0; i <= index; i++) {
                sum += spinBoxes.at(i)->value();
            }
            multiSlider->setValue(index, sum);
        }
    } else {
        multiSlider->setValue(index, value);
    }
}

void MultiSliderWidget::updateSpinBoxValue(int i) {
    const QVector<int> &positions = multiSlider->positions();
    if (_showDifferences) {
        spinBoxes.at(i)->setValue(positions.at(i) - (i ? positions.at(i - 1) : multiSlider->minimum()));
    } else {
        spinBoxes.at(i)->setValue(positions.at(i));
    }
}

void MultiSliderWidget::selectNextSpinBox() {
    for (int i = 0; i < spinBoxes.size(); ++i) {
        if (spinBoxes.at(i)->hasFocus()) {
            int nextIndex = (i == spinBoxes.size() - 1) ? 0 : i + 1;
            spinBoxes.at(nextIndex)->setFocus();
            return;
        }
    }
    int selectedHandle = multiSlider->selectedHandle();
    if (multiSlider->count() == 0) {
        return;
    }
    int nextIndex = (selectedHandle == spinBoxes.size() - 1) ? 0 : selectedHandle + 1;
    spinBoxes.at(nextIndex)->setFocus();
}

void MultiSliderWidget::onLabelsUnderChanged() {
    sliderLayout->takeAt(0);
    if (m_labelsUnder)
    {
        //sliderLayout->addLayout(labelsLayout);
    }
    sliderLayout->addWidget(multiSlider);
    if (!m_labelsUnder)
    {
        //sliderLayout->addLayout(labelsLayout);
    }
    //setFixedHeight(this->minimumSizeHint().height());
}

bool MultiSliderWidget::labelsUnder() const {
    return m_labelsUnder;
}

bool MultiSliderWidget::showPositions() const {
    return !_showDifferences;
}

void MultiSliderWidget::setLabelsUnder(bool arg) {
    if (m_labelsUnder == arg)
        return;
    m_labelsUnder = arg;
    onLabelsUnderChanged();
    emit labelsUnderChanged(arg);
}

void MultiSliderWidget::setShowPositions(bool arg) {
    if (!_showDifferences == arg)
        return;

    _showDifferences = !arg;
    onSliderCountChanged(multiSlider->count());
    emit showPositionsChanged(arg);
    emit showDifferencesChanged(!arg);
}

void MultiSliderWidget::setShowDifferences(bool arg) {
    if (_showDifferences == arg)
        return;

    _showDifferences = arg;
    onSliderCountChanged(multiSlider->count());
    onSliderPositionsChanged(multiSlider->positions());
    emit showDifferencesChanged(arg);
    emit showPositionsChanged(!arg);
}
