QT += core gui widgets

TARGET = MultiSlider
TEMPLATE = app

SOURCES += \
    main.cpp\
    MultiSlider/MultiSliderWidget.cpp \
    MultiSlider/SliceInfo.cpp \
    MultiSlider/varislice_slider.cpp

HEADERS += \
    MultiSlider/MultiSlider.h \
    MultiSlider/MultiSliderWidget.h \
    MultiSlider/SliceInfo.h \
    MultiSlider/varislice_slider.h

RESOURCES += \
    MultiSlider/MultiSlider.qrc
