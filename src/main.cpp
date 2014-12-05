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

#include <window.h>
#include <QApplication>
#include <QTranslator>
#include <QLocale>

int main(int argc,char **argv)
{
	//create the application
	QApplication app(argc,argv);

	//set the config-keys
	app.setOrganizationName("benaryorg");
	app.setOrganizationDomain("benary.org");
	app.setApplicationName("discover");

	//load our favourite locale
	QLocale locale=QLocale::system();
	QTranslator translator;
	translator.load(QString(":/lang/")+locale.name());
	app.installTranslator(&translator);

	//create the window
	Window win;

	//execute
	return app.exec();
}
