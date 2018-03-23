/**
 * ogon - Free Remote Desktop Services
 * Qt-based Message Application
 *
 * Copyright (c) 2014-2018 Thincast Technologies GmbH
 *
 * Authors:
 * Martin Haimberger <martin.haimberger@thincast.com>
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

#include <QtWidgets/QApplication>
#include "qogonmessage.h"

#include <iostream>

void printhelp() {
	std::cout << "usage: ogon-message <messageid> <messagetype> <style> <timeout> <string1> <string2> <string3> <string4> <string5>\n";
	std::cout << " <messageid> ... is the identifier of the message\n";
	std::cout << " <messagetype> ... 1 ... Custom message\n";
	std::cout << "                   2 ... Remote control request message\n";
	std::cout << " <style> ... 0 .. OK\n";
	std::cout << "             1 .. OK Cancel\n";
	std::cout << "             2 .. Abort Retry Ignore\n";
	std::cout << "             3 .. Yes No Cancel\n";
	std::cout << "             4 .. Yes No\n";
	std::cout << "             5 .. Retry Cancel\n";
	std::cout << "             6 .. Cancel Try Continue\n";
	std::cout << " <timeout> ... Timeout in sec before the dialog closes and returns timeout result\n\n";
	std::cout << "Example: ogon-message 0 1 4 0 \"Do you see the message?\"\n";

}

bool convert(char *string, uint &value) {
	QString convertHelper;
	bool ok;

	convertHelper = string;
	value = convertHelper.toInt(&ok, 10);
	return ok;
}

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	if (argc < 6) {
		printhelp();
		return 0;
	}
	uint id;
	uint type;
	uint style;
	uint timeout;
	bool error = false;

	error |= !convert(argv[1], id);
	error |= !convert(argv[2], type);
	error |= !convert(argv[3], style);
	error |= !convert(argv[4], timeout);

	if (error) {
		printhelp();
		return 0;
	}

	/* convert string parameter*/
	QString para1 = "";
	QString para2 = "";
	QString para3 = "";
	QString para4 = "";
	QString para5 = "";

	switch (argc) {
		case 10:
			para5 = argv[9];
			/*no break*/
		case 9:
			para4 = argv[8];
			/*no break*/
		case 8:
			para3 = argv[7];
			/*no break*/
		case 7:
			para2 = argv[6];
			/*no break*/
		case 6:
			para1 = argv[5];
			/*no break*/
		default:
			break;
	}

	QOgonMessageBox box(id, type, style, timeout, para1, para2, para3, para4, para5);

	app.installTranslator(box.getTranslator());
	if (!box.prepare()) {
		printhelp();
		return 0;
	}
	box.start();
	app.exec();

	return box.getResponse();
}
