QT += charts

HEADERS += \
    mainwidget.h \
    resultstat.h \
    logfile.h \
    templates.h \
    dbrfpath.h \
    dbmbrfpath.h \
    datainterface.h \
    parseutil.h \
    execobjlist.h \
    dbatten.h \
    pmiloss.h \
    pmdsterm.h \
    pmdsopen.h \
    sbddscal.h

SOURCES += \
    main.cpp \
    mainwidget.cpp \
    resultstat.cpp \
    logfile.cpp \    
    dbrfpath.cpp \
    dbmbrfpath.cpp \
    parseutil.cpp \
    execobjlist.cpp \
    dbatten.cpp \
    pmiloss.cpp \
    pmdsterm.cpp \
    pmdsopen.cpp \
    sbddscal.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/charts/legendmarkers
INSTALLS += target
