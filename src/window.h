/*
 * Copyright 2014 benaryorg (benaryorg@benary.org)
 *
 * This file is part of discovery.
 *
 * discovery is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * discovery is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with discovery.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <receiver.h>
#include <QtCore>
#include <QWidget>
#include <QSystemTrayIcon>
#include <QVBoxLayout>
#include <QListWidget>
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
	void copy(QListWidgetItem *item);
	void changeName(void);
	void toggleNotifications(void);
private:
	QMap<QString,QListWidgetItem *> peers;
	Receiver *receiver;
	QTimer *timer;
	QSystemTrayIcon *tray;
	QVBoxLayout *layout;
	QListWidget *list;
	QLabel *label;
	QAction *quitaction;
	QMenu *traymenu;
	QPushButton *namechanger;
	QPushButton *notifications;
};

#endif /** WINDOW_H **/
