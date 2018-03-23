/**
 * ogon - Free Remote Desktop Services
 * Shadowing Application
 *
 * Copyright (c) 2014-2018 Thincast Technologies GmbH
 *
 * Authors:
 * Bernhard Miklautz <bernhard.miklautz@thincast.com>
 *
 * This file may be used under the terms of the GNU Affero General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE-AGPL included in the distribution
 * of this file.
 *
 * Under the GNU Affero General Public License version 3 section 7 the
 * copyright holders grant the additional permissions set forth in the
 * ogon Core AGPL Exceptions version 1 as published by
 * Thincast Technologies GmbH.
 *
 * For more information see the file LICENSE in the distribution of this file.
 */

#include <QApplication>
#include <QDesktopWidget>
#include "mainwindow.h"

#define DEFAULT_HEIGHT 712

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	MainWindow mw;
	QRect size;

	size.setHeight(DEFAULT_HEIGHT);
	QRect screenSize = a.desktop()->availableGeometry();
	if (screenSize.height() <= DEFAULT_HEIGHT) {
		size.setHeight(screenSize.height() - 20);
	}
	size.setY(24);
	mw.setGeometry(size);
	if(QGuiApplication::platformName() == "ogon") {
		mw.showFullScreen();
	}
	mw.show();
	return a.exec();
}
