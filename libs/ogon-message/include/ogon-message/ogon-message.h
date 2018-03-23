/**
 * ogon - Free Remote Desktop Services
 * MessageBox Library for Qt Applications
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
 * Under the GNU Affero General Public License version 3 section 7 the
 * copyright holders grant the additional permissions set forth in the
 * ogon Library AGPL Exceptions version 1 as published by
 * Thincast Technologies GmbH.
 *
 * For more information see the file LICENSE in the distribution of this file.
 */

#ifndef __QOGON_MESSAGE_BASE_H__
#define __QOGON_MESSAGE_BASE_H__

#include <QTimer>
#include <QKeyEvent>
#include <QTranslator>

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

class QOgonMessageBoxBase : public QWidget {
	Q_OBJECT

public:

	QOgonMessageBoxBase(uint id, uint type, uint style, uint timeout,
			QString para1, QString para2, QString para3,
			QString para4, QString para5);

	virtual ~QOgonMessageBoxBase();

	bool prepare();
	void start();
	void setupUi(QWidget *messageBox);
	void retranslateUi(QWidget *messageBox);

	QTranslator *getTranslator();
	const QStringList &getLocales();

	uint getId();
	uint getResponse();


private slots:

	void updateTime();
	void buttonclicked();
	void keyPressEvent(QKeyEvent * event);

private:
	uint mId;
	uint mType;
	uint mStyle;
	uint mTimeout;
	uint mCurrentTimeout;
	int mResponse;

	QString mPara1;
	QString mPara2;
	QString mPara3;
	QString mPara4;
	QString mPara5;

	QTimer *mTimer;

	virtual void sendResponse() = 0;
	virtual void showMessagebox() = 0;

	/* gui elements */

	QVBoxLayout *verticalLayout_2;
	QVBoxLayout *content;
	QLabel *titleLabel;
	QSpacerItem *topSpacer;
	QLabel *contentLabel;
	QSpacerItem *buttomSpacer;
	QHBoxLayout *timeoutBox;
	QSpacerItem *timeoutSpacer;
	QLabel *timeoutLabel;
	QLabel *timeout;
	QHBoxLayout *buttonBox;
	QSpacerItem *horizontalSpacer;
	QPushButton *buttonOK;
	QPushButton *buttonYes;
	QPushButton *buttonNo;
	QPushButton *buttonAbort;
	QPushButton *buttonRetry;
	QPushButton *buttonIgnore;
	QPushButton *buttonCancel;
	QPushButton *buttonTry;
	QPushButton *buttonContine;

	QStringList mLocalizationFiles;

	QTranslator mMessageTranslator;
};

#endif //__QOGON_MESSAGE_BASE_H__
