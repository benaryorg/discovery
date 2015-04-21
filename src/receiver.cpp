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

#include <receiver.h>
#include <QtCore>
#include <QtNetwork>

Receiver::Receiver(QObject *parent):QObject(parent)
{
	this->peers=QMultiMap<QString,QPair<QHostAddress,int>>();
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

QStringList Receiver::getIps(QString name)
{
	QStringList list;
	if(!this->peers.contains(name))
	{
		return list;
	}
	for(QPair<QHostAddress,int> p:this->peers.values(name))
	{
		list<<p.first.toString();
	}
	return list;
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
		for(QPair<QHostAddress,int> p:this->peers.values(key))
		{
			if(p.second++>20)
			{
				emit lostPeer(key);
				this->peers.remove(key,p);
			}
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
		QString name=message.length()>4?message.mid(4):"unknown";
		bool contained=this->peers.contains(name);
		if(contained)
		{
			contained=false;
			for(QPair<QHostAddress,int> p:this->peers.values(name))
			{
				if(p.first==peer)
				{
					p.second=0;
					contained=true;
				}
			}
		}
		if(!contained)
		{
			this->peers.insert(name,QPair<QHostAddress,int>(peer,0));
			emit newPeer(name);
		}
		
		if(message.startsWith("ping"))
		{
			this->ping(peer,QByteArray("pong").append(QSettings().value("name","").toString()));
		}
	}
}

