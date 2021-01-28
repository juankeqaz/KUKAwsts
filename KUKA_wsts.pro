QT       += core gui opengl network 3dextras 3drender 3dcore

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += WNT

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    #LBRIII_trajectory_planning/mathtype.c \
    #LBRIII_trajectory_planning/trajectory_planning.c \
    box.cpp \
    circular.cpp \
    cone.cpp \
    cylinder.cpp \
    dataanysis.cpp \
    detecsting.cpp \
    globalvar.cpp \
    homeseting.cpp \
    imagebutton.cpp \
    ipinputbox.cpp \
    kinematics.cpp \
    line.cpp \
    main.cpp \
    maintitle.cpp \
    mainwindow.cpp \
    modelview.cpp \
    msgserver.cpp \
    mybutton.cpp \
    myview.cpp \
    occstlreader.cpp \
    occview.cpp \
    pipes.cpp \
    planner.cpp \
    quiwidget.cpp \
    robotmodel.cpp \
    runpage.cpp \
    scannersetting.cpp \
    scenemodifier.cpp \
    startwidget.cpp \
    stlmodels.cpp \
    stlmodule.cpp \
    sweeps.cpp \
    tcpserver.cpp \
    toolcoord.cpp \
    torus.cpp \
    uitrasonicsettings.cpp \
    wedge.cpp \
    workpiece.cpp

HEADERS += \
    DataTypes.h \
    KinematicConstants.h \
    #LBRIII_trajectory_planning/mathtype.h \
    #LBRIII_trajectory_planning/trajectory_planning.h \
    box.h \
    circular.h \
    cone.h \
    cylinder.h \
    dataanysis.h \
    detecsting.h \
    globalvar.h \
    head.h \
    homeseting.h \
    imagebutton.h \
    ipinputbox.h \
    kinematics.h \
    line.h \
    maintitle.h \
    mainwindow.h \
    modelview.h \
    msgserver.h \
    mybutton.h \
    myview.h \
    occ.h \
    occstlreader.h \
    occview.h \
    pipes.h \
    planner.h \
    quiwidget.h \
    robotmodel.h \
    runpage.h \
    scannersetting.h \
    scenemodifier.h \
    startwidget.h \
    stlmodels.h \
    stlmodule.h \
    sweeps.h \
    tcpserver.h \
    toolcoord.h \
    torus.h \
    uitrasonicsettings.h \
    wedge.h \
    workpiece.h \
    $$PWD/matrix/*.hpp

FORMS += \
    dataanysis.ui \
    detecsting.ui \
    homeseting.ui \
    imagebutton.ui \
    mainwindow.ui \
    runpage.ui \
    scannersetting.ui \
    startwidget.ui \
    uitrasonicsettings.ui \
    workpiece.ui

INCLUDEPATH += ./../KUKA_wsts/inc ./../KUKA_wsts/inc/assimp ./../KUKA_wsts/assimp/Compiler ./../KUKA_wsts/glut ./../KUKA_wsts/Eigen
LIBS += -L./../KUKA_wsts/lib -L./../KUKA_wsts/lib/libb -L./../KUKA_wsts/lib/assimp -lTKBin -lTKBinL -lTKBinTObj -lTKBinXCAF -lTKBO -lTKBool -lTKBRep -lTKCAF -lTKCDF -lTKD3DHost -lTKDCAF -lTKDFBrowser -lTKDraw -lTKernel -lTKFeat -lTKFillet -lTKG2d\
 -lTKG3d -lTKGeomAlgo -lTKGeomBase -lTKHLR -lTKIGES -lTKIVtk -lTKIVtkDraw -lTKLCAF -lTKMath -lTKMesh -lTKMeshVS -lTKOffset -lTKOpenGl -lTKPrim -lTKQADraw -lTKRWMesh -lTKService -lTKShapeView\
 -lTKShHealing -lTKStd -lTKStdL -lTKSTEP -lTKSTEP209 -lTKSTEPAttr -lTKSTEPBase -lTKSTL -lTKTInspector -lTKTInspectorAPI -lTKTObj -lTKTObjDRAW -lTKToolsDraw -lTKTopAlgo -lTKTopTest -lTKTreeModel\
 -lTKV3d -lTKVCAF -lTKView -lTKViewerTest -lTKVInspector -lTKVRML -lTKXDEDRAW -lTKXDEIGES -lTKXDESTEP -lTKXMesh -lTKXml -lTKXmlL -lTKXmlTObj -lTKXmlXCAF -lTKXSBase -lTKXSDRAW -lavcodec -lavdevice\
 -lavfilter -lavformat -lavutil -lswscale -lFreeImage -lFreeImagePlus -lfreetype -ltbb -ltbb_debug -ltbb_preview -ltbb_preview_debug -ltbbmalloc -ltbbmalloc_debug -ltbbmalloc_proxy -ltbbmalloc_proxy_debug\
 -lOpengl32 -lglu32 -lgdi32 -lGLU32 -lassimp
LIBS += -L./../KUKA_wsts/glut -lglut

RESOURCES += \
    other/main.qrc \
    other/qss.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



