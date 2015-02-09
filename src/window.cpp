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

#include <window.h>
#include <receiver.h>
#include <QtGui>
#include <QtNetwork>

#include <QMessageBox>
#include <QApplication>
#include <QInputDialog>
#include <QIcon>
#include <QLineEdit>

Window::Window(QWidget *parent):QWidget(parent)
{
	QSettings set;

	this->peers=QMap<QString,QListWidgetItem *>();

	this->setWindowTitle(tr("Peers"));
	this->move(set.value("window_pos",QPoint(0,0)).value<QPoint>());
	this->resize(set.value("window_dim",QSize(192,300)).value<QSize>());

	this->layout=new QVBoxLayout();
	this->setLayout(this->layout);

	this->list=new QListWidget();
	connect(this->list,SIGNAL(itemDoubleClicked(QListWidgetItem *)),this,SLOT(copy(QListWidgetItem *)));

	this->label=new QLabel(tr("Double click an entry to copy it."));

	this->namechanger=new QPushButton(set.value("name",tr("Change your name!")).toString());
	connect(this->namechanger,SIGNAL(clicked()),this,SLOT(changeName()));

	this->notifications=new QPushButton(set.value("notifications",true).toBool()?tr("Stop Notifications!"):tr("Start Notifications!"));
	connect(this->notifications,SIGNAL(clicked()),this,SLOT(toggleNotifications()));

	this->layout->addWidget(this->label);
	this->layout->addWidget(this->list);
	this->layout->addWidget(this->namechanger);
	this->layout->addWidget(this->notifications);

	this->tray=new QSystemTrayIcon(QIcon(":/data/network.png"),this);
	this->tray->setToolTip(tr("Peer Discovery"));

	this->quitaction=new QAction(tr("&Quit"),this);
	this->quitaction->setShortcut(Qt::Key_Q|Qt::CTRL);
	connect(this->quitaction,SIGNAL(triggered()),qApp,SLOT(quit()));
	this->addAction(this->quitaction);
	this->traymenu=new QMenu(this);
	this->traymenu->addAction(this->quitaction);
	this->tray->setContextMenu(this->traymenu);
	this->tray->show();

	connect(this->tray,SIGNAL(messageClicked()),this,SLOT(openWindow()));
	connect(this->tray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

	try
	{
		this->receiver=new Receiver();
	}
	catch(int ex)
	{
		QMessageBox::critical(this,tr("Port is occupied"),tr("The port this program uses is either occupied by another program or another instance of this program!"));
		qApp->exec();
		throw;
	}
	connect(this->receiver,SIGNAL(newPeer(QString)),this,SLOT(newPeer(QString)));
	connect(this->receiver,SIGNAL(lostPeer(QString)),this,SLOT(lostPeer(QString)));

	this->timer=new QTimer();
	connect(this->timer,SIGNAL(timeout()),this->receiver,SLOT(ping()));
	this->timer->setInterval(250);
	this->timer->start();
}

Window::~Window(void)
{
	QSettings set;
	set.setValue("window_pos",this->pos());
	set.setValue("window_dim",this->size());

	this->timer->stop();
	delete this->receiver;
	delete this->timer;
	delete this->layout;
	delete this->traymenu;
	delete this->quitaction;
	delete this->tray;
	delete this->label;
	delete this->namechanger;
	QList<QListWidgetItem *> list=this->peers.values();
	foreach(QListWidgetItem *item,list)
	{
		delete item;
	}
	delete this->list;
}

void Window::closeEvent(QCloseEvent *event)
{
	if(this->tray->isVisible()&&QSettings().value("notifications",true).toBool())
	{
		this->tray->showMessage(tr("Still Alive"),tr("I am still running!"),QSystemTrayIcon::Information,5000);
	}
	this->hide();
	event->ignore();
}

void Window::newPeer(QString peer)
{
	if(this->tray->isVisible()&&QSettings().value("notifications",true).toBool())
	{
		this->tray->showMessage(tr("New Peer"),tr("A wild %1 appears!").arg(peer),QSystemTrayIcon::Information,2500);
	}
	this->peers[peer]=new QListWidgetItem(peer,this->list);
}

void Window::lostPeer(QString peer)
{
	if(this->tray->isVisible()&&QSettings().value("notifications",true).toBool())
	{
		this->tray->showMessage(tr("Lost Peer"),tr("The wild %1 fled!").arg(peer),QSystemTrayIcon::Information,2500);
	}
	delete this->peers[peer];
	this->peers.remove(peer);
}

void Window::openWindow(void)
{
	this->show();
	this->raise();
}

void Window::copy(QListWidgetItem *item)
{
	QApplication::clipboard()->setText(this->receiver->getIp(item->text()));
	if(this->tray->isVisible()&&QSettings().value("notifications",true).toBool())
	{
		this->tray->showMessage(tr("Copied"),tr("You just copied an IP!"),QSystemTrayIcon::Information,2500);
	}
}

void Window::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch(reason)
	{
		case QSystemTrayIcon::DoubleClick:
			if(this->isVisible())
			{
				this->hide();
			}
			else
			{
				this->openWindow();
			}
			break;
		default:
			break;
	}
}

void Window::changeName(void)
{
	QSettings set;
	QString name=QInputDialog::getText(this,tr("Change your Name"),tr("Name:"),QLineEdit::Normal,set.value("name","").toString());
	if(name.isEmpty())
	{
		set.remove("name");
	}
	else
	{
		set.setValue("name",name);
	}
	this->namechanger->setText(set.value("name",tr("Change your name!")).toString());
	set.sync();
}

void Window::toggleNotifications(void)
{
	QSettings set;
	set.setValue("notifications",!set.value("notifications",true).toBool());
	this->notifications->setText(set.value("notifications",true).toBool()?tr("Stop Notifications!"):tr("Start Notifications!"));
	set.sync();
}

