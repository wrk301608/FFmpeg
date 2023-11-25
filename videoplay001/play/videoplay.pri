


#引入ffmpeg
LIBS += -L$$PWD/ffmpeg/lib -lavcodec -lavfilter -lavformat -lswscale -lavutil -lswresample -lavdevice
#加入头文件搜索路径
INCLUDEPATH += $$PWD/ffmpeg/include
#加入依赖项搜索路径
DEPENDPATH += $$PWD/ffmpeg/include









HEADERS += \
    $$PWD/readthread.h \
    $$PWD/videodocode.h

SOURCES += \
    $$PWD/readthread.cpp \
    $$PWD/videodocode.cpp
