#ifndef MULTISLIDERWIDGET_H
#define MULTISLIDERWIDGET_H

#include <QFrame>

class MultiSlider;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QEvemt;
class QSignalMapper;

class MultiSliderWidget : public QFrame {
    Q_OBJECT
    Q_PROPERTY(bool labelsUnder READ labelsUnder WRITE setLabelsUnder NOTIFY labelsUnderChanged)
    Q_PROPERTY(bool showPositions READ showPositions WRITE setShowPositions NOTIFY showPositionsChanged)

public:
    MultiSliderWidget(QWidget *parent = 0);
    MultiSlider *GetMultiSlider();
    QPushButton *GetButtonAddToLeft();
    QPushButton *GetButtonAddToTop();
    QPushButton *GetButtonRemoveFromLeft();
    QPushButton *GetButtonRemoveFromRight();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    class SpinBox;
    SpinBox *createSpinBox(int index);
    void initMultiSlider();
    void createWidget();
    void updateSpinBoxValue(int i);
    void selectNextSpinBox();
    void onLabelsUnderChanged();

private slots:
    void updateButtonsEnable();
    void onSliderCountChanged(int count);
    void onSelectedHandleChanged(int handle);
    void onSliderPositionsChanged(QVector<int> values);
    void onSliderRangeChanged(int min, int max);
    void onSpinBoxValueChanged(int value);

private:
    QVBoxLayout *labelsLayout;
    QHBoxLayout *sliderLayout;
    QVBoxLayout *widgetLayout;
    MultiSlider *multiSlider;
    QList<SpinBox *> spinBoxes;
    QPushButton *addToLeftButton;
    QPushButton *addToTopButton;
    QPushButton *removeFromLeftButton;
    QPushButton *removeFromRightButton;
    QPushButton *removeAllButton;

    //properties section
public:
    bool labelsUnder() const;
    bool showPositions() const;
    void updataMultiSliderData(QVector<int> positions);

public slots:
    void setLabelsUnder(bool arg);
    void setShowPositions(bool arg);
    void setShowDifferences(bool arg);

signals:
    void labelsUnderChanged(bool arg);
    void showPositionsChanged(bool arg);
    void showDifferencesChanged(bool arg);

private:
    bool m_labelsUnder = false;
    bool _showDifferences = true;

public:
    // default slice data
    const float m_modelHeight = 10.0;
    const float m_sliceHeightDefault = 0.01;
    int m_sliceCountDefault;
public:
    void getAllHeights2Bot(QVector<float> &allHeights2Bot);
    void getAllExpoTimes(QVector<float> &allExpoTimes);
    void getAllLiftHs(QVector<int> &allLiftHs);
};

#endif // MULTISLIDERWIDGET_H
