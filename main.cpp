#include <QApplication>
#include "MultiSlider/MultiSliderWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MultiSliderWidget w;
    w.setGeometry(600, 100, 150, 800);
    w.show();

    return a.exec();
}
