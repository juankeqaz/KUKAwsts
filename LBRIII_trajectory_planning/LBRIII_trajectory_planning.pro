TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    kinematics.c \
    mathtype.c \
    trajectory_planning.c

HEADERS += \
    kinematics.h \
    mathtype.h \
    trajectory_planning.h
