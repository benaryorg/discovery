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

#include <receiver.h>
#include <QtCore>
#include <QtNetwork>

Receiver::Receiver(void)
{
	this->peers=QMap<QString,QPair<int,QString>>();
	this->socket=new QUdpSocket();

	connect(this->socket,SIGNAL(readyRead()),this,SLOT(udpPacket()));
	if(this->socket->bind(QHostAddress::Any,13370,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint)==-1)
	{
		throw -1;
	}
}

Receiver::~Receiver(void)
{
	delete this->socket;
}

QString Receiver::getIp(QString name)
{
	if(!this->peers.contains(name))
	{
		return QString();
	}
	return this->peers[name].second;
}

void Receiver::ping(QHostAddress dest,QByteArray msg)
{
	if(dest==QHostAddress("255.255.255.255"))
	{
		foreach(QNetworkInterface interface,QNetworkInterface::allInterfaces())
		{
			foreach(QNetworkAddressEntry entry,interface.addressEntries())
			{
				if(entry.broadcast()!=QHostAddress::Null)
				{
					this->ping(entry.broadcast(),QByteArray("ping").append(QSettings().value("name","").toString()));
				}
			}
		}
		return;
	}
	foreach(QString key,this->peers.keys())
	{
		if(this->peers[key].first++>20)
		{
			emit lostPeer(key);
			this->peers.remove(key);
		}
	}
	if(this->socket->writeDatagram(msg,dest,13370)==-1)
	{
	}
}

void Receiver::udpPacket(void)
{
	while(this->socket->hasPendingDatagrams())
	{
		quint16 port;
		QHostAddress sender;
		QByteArray message;
		message.resize(socket->pendingDatagramSize());
		if(this->socket->readDatagram(message.data(),message.size(),&sender,&port)==-1)
		{
			continue;
		}
		else
		{
			emit this->message(sender,message);
		}
	}
}

void Receiver::message(QHostAddress peer,QByteArray message)
{
	if(message.startsWith("pong")||message.startsWith("ping"))
	{
		QString name=message.length()>4?message.mid(4):peer.toString();
		if(!this->peers.contains(name))
		{
			this->peers[name]=QPair<int,QString>();
			this->peers[name].second=peer.toString();
			emit newPeer(name);
		}
		this->peers[name].first=0;
		if(message.startsWith("ping"))
		{
			this->ping(peer,QByteArray("pong").append(QSettings().value("name","").toString()));
		}
	}
}

