/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 benaryorg (benaryorg@benary.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <receiver.h>
#include <QtCore>
#include <QWidget>
#include <QSystemTrayIcon>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QLabel>
#include <QPushButton>
#include <QAction>
#include <QMenu>

class Window:public QWidget
{
	Q_OBJECT

public:
	Window(QWidget *parent=0);
	~Window(void);

	void closeEvent(QCloseEvent *event);
public slots:
	void newPeer(QString peer);
	void lostPeer(QString peer);
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
	void openWindow(void);
	void copy(const QModelIndex &index);
	void changeName(void);
	void toggleNotifications(void);
private:
	QMap<QString,QTreeWidgetItem *> peers;
	Receiver *receiver;
	QTimer *timer;
	QSystemTrayIcon *tray;
	QVBoxLayout *layout;
	QTreeWidget *list;
	QLabel *label;
	QAction *quitaction;
	QMenu *traymenu;
	QPushButton *namechanger;
	QPushButton *notifications;
};

#endif /** WINDOW_H **/
