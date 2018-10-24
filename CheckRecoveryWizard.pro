#-------------------------------------------------
#
# Project created by QtCreator 2018-10-18T12:08:56
# Rust comm
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CheckRecoveryWizard
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    loggingcategories.cpp \
    ConnectionDialog/connectiondialog.cpp \
    EditConnDialog/editconndialog.cpp \
    RecoveryWizard/recoverywizard.cpp \
    RecoveryWizard/connectionspage.cpp \
    RecoveryWizard/terminalspage.cpp \
    RecoveryWizard/shiftspage.cpp

HEADERS += \
        mainwindow.h \
    database.h \
    loggingcategories.h \
    ConnectionDialog/connectiondialog.h \
    EditConnDialog/editconndialog.h \
    RecoveryWizard/recoverywizard.h \
    RecoveryWizard/pagelist.h \
    RecoveryWizard/connectionspage.h \
    RecoveryWizard/terminalspage.h \
    RecoveryWizard/shiftspage.h

FORMS += \
        mainwindow.ui \
    ConnectionDialog/connectiondialog.ui \
    EditConnDialog/editconndialog.ui \
    RecoveryWizard/recoverywizard.ui \
    RecoveryWizard/connectionspage.ui \
    RecoveryWizard/terminalspage.ui \
    RecoveryWizard/shiftspage.ui

DISTFILES += \
    SQL/database.sql

RESOURCES += \
    resource.qrc


