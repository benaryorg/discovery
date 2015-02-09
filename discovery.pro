# The MIT License (MIT)
# 
# Copyright (c) 2014 benaryorg
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

TEMPLATE = app
TARGET = main
DEPENDPATH += lang src
INCLUDEPATH += src
OBJECTS_DIR = build/obj
MOC_DIR = build/moc

QT += core gui widgets network svg
CONFIG += release qt
CONFIG -= debug

QMAKE_CXXFLAGS += -std=c++11

RESOURCES = qt.qrc

win32:RC_FILE = windows.rc

# Input
HEADERS += src/receiver.h src/window.h
SOURCES += src/main.cpp src/receiver.cpp src/window.cpp
TRANSLATIONS += lang/de.ts
