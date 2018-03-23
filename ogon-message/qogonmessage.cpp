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

#include "qogonmessage.h"

QOgonMessageBox::QOgonMessageBox(uint id, uint type, uint style, uint timeout,
		QString para1, QString para2, QString para3, QString para4, QString para5) :
		QOgonMessageBoxBase(id, type, style, timeout, para1, para2, para3, para4, para5){
	setWindowFlags((Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowSystemMenuHint)  & ~Qt::WindowMaximizeButtonHint);

}

QOgonMessageBox::~QOgonMessageBox() {

}

void QOgonMessageBox::sendResponse() {

	hide();
	close();
}

void QOgonMessageBox::showMessagebox() {
	show();
}


bool QOgonMessageBox::prepare() {

	bool returnval = QOgonMessageBoxBase::prepare();
	return returnval;
}
