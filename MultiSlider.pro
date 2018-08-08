QT += core gui widgets

TARGET = MultiSlider
TEMPLATE = app

SOURCES += \
    main.cpp\
    MultiSlider/MultiSliderWidget.cpp \
    MultiSlider/SliceInfo.cpp

HEADERS += \
    MultiSlider/MultiSlider.h \
    MultiSlider/MultiSliderWidget.h \
    MultiSlider/SliceInfo.h

RESOURCES += \
    MultiSlider/MultiSlider.qrc
