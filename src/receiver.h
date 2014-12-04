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

#ifndef RECEIVER_H
#define RECEIVER_H

#include <QtCore>
#include <QtNetwork>

class Receiver:public QObject
{
	Q_OBJECT
public:
	Receiver(void);
	~Receiver(void);

	QString getIp(QString name);
public slots:
	void ping(QHostAddress dest=QHostAddress("255.255.255.255"),QByteArray msg=QByteArray("ping"));
	void udpPacket(void);
	void message(QHostAddress,QByteArray);
signals:
	void newPeer(QString peer);
	void lostPeer(QString peer);
private:
	QMap<QString,QPair<int,QString>> peers;
	QUdpSocket *socket;
};

#endif /** RECEIVER_H **/
