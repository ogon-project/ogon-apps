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

#include <winpr/user.h>
#include <QDesktopWidget>
#include <ogon-message/ogon-message.h>
#include <ogon/message.h>

QOgonMessageBoxBase::QOgonMessageBoxBase(uint id, uint type, uint style, uint timeout,
		QString para1, QString para2, QString para3, QString para4, QString para5) {
	setupUi(this);
	hide();
	mId = id;
	mType = type;
	mStyle = style;
	mTimeout = timeout;

	mPara1 = para1;
	mPara2 = para2;
	mPara3 = para3;
	mPara4 = para4;
	mPara5 = para5;
	mCurrentTimeout = timeout;
	mTimer = new QTimer(this);

	connect(mTimer, SIGNAL(timeout()), this, SLOT(updateTime()));
	connect(buttonOK, SIGNAL(clicked()), this, SLOT(buttonclicked()));
	connect(buttonYes, SIGNAL(clicked()), this, SLOT(buttonclicked()));
	connect(buttonNo, SIGNAL(clicked()), this, SLOT(buttonclicked()));
	connect(buttonAbort, SIGNAL(clicked()), this, SLOT(buttonclicked()));
	connect(buttonRetry, SIGNAL(clicked()), this, SLOT(buttonclicked()));
	connect(buttonIgnore, SIGNAL(clicked()), this, SLOT(buttonclicked()));
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(buttonclicked()));
	connect(buttonTry, SIGNAL(clicked()), this, SLOT(buttonclicked()));
	connect(buttonContine, SIGNAL(clicked()), this, SLOT(buttonclicked()));
	mLocalizationFiles << "en" << "de";

	QString local = QLocale::system().name();
	local = local.section('_', 0, 0);

	if(!mLocalizationFiles.contains(local))
		local = "en";

	mMessageTranslator.load("ogon-message_" + local, TRANSLATIONS_DIR);
	mResponse = -1;
}

QTranslator *QOgonMessageBoxBase::getTranslator() {
	return &mMessageTranslator;
}

const QStringList &QOgonMessageBoxBase::getLocales() {
	return mLocalizationFiles;
}


void QOgonMessageBoxBase::buttonclicked() {
	QObject* obj = sender();
	if (obj == buttonOK) {
		mResponse = 1;
	} else if (obj == buttonYes) {
		mResponse = 6;
	} else if (obj == buttonNo) {
		mResponse = 7;
	} else if (obj == buttonAbort) {
		mResponse = 3;
	} else if (obj == buttonRetry) {
		mResponse = 4;
	} else if (obj == buttonIgnore) {
		mResponse = 5;
	} else if (obj == buttonCancel) {
		mResponse = 2;
	} else if (obj == buttonTry) {
		mResponse = 10;
	} else if (obj == buttonContine) {
		mResponse = 11;
	} else {
		qDebug("got signal buttonclicked, but without known source");
		mResponse = -1;
	}
	if (mResponse != -1) {
		sendResponse();
	}
}

bool QOgonMessageBoxBase::prepare() {

	retranslateUi(this);

	buttonOK->setVisible(false);
	buttonYes->setVisible(false);;
	buttonNo->setVisible(false);;
	buttonAbort->setVisible(false);;
	buttonRetry->setVisible(false);;
	buttonIgnore->setVisible(false);;
	buttonCancel->setVisible(false);;
	buttonTry->setVisible(false);;
	buttonContine->setVisible(false);;

	setFocusPolicy(Qt::StrongFocus);

	switch (mStyle) {
		case MB_OK:
			buttonOK->setVisible(true);
			buttonOK->setDefault(true);
			buttonOK->setFocus();
			break;
		case MB_OKCANCEL:
			buttonOK->setVisible(true);
			buttonCancel->setVisible(true);
			buttonOK->setDefault(true);
			buttonOK->setFocus();
			break;
		case MB_ABORTRETRYIGNORE:
			buttonAbort->setVisible(true);
			buttonRetry->setVisible(true);
			buttonIgnore->setVisible(true);
			buttonAbort->setDefault(true);
			buttonAbort->setFocus();
			break;
		case MB_YESNOCANCEL:
			buttonYes->setVisible(true);
			buttonNo->setVisible(true);
			buttonCancel->setVisible(true);
			buttonYes->setDefault(true);
			buttonYes->setFocus();
			break;
		case MB_YESNO:
			buttonYes->setVisible(true);
			buttonNo->setVisible(true);
			buttonYes->setDefault(true);
			buttonYes->setFocus();
			break;
		case MB_RETRYCANCEL:
			buttonRetry->setVisible(true);
			buttonCancel->setVisible(true);
			buttonRetry->setDefault(true);
			buttonRetry->setFocus();
			break;
		case MB_CANCELTRYCONTINUE:
			buttonCancel->setVisible(true);
			buttonTry->setVisible(true);
			buttonContine->setVisible(true);
			buttonCancel->setDefault(true);
			buttonCancel->setFocus();
			break;
		default:
			qDebug("Unknow style %d! Returning timeout!", mStyle);
			mResponse = -1;
			sendResponse();
			return false;
			break;
	}

	if (mTimeout) {
		timeoutLabel->setVisible(true);
		timeout->setVisible(true);
		mCurrentTimeout++;
		updateTime();
	} else {
		timeoutLabel->setVisible(false);
		timeout->setVisible(false);
	}

	switch (mType) {
		case MESSAGE_REQUEST_REMOTE_CONTROL:
			titleLabel->setText(tr("Remote control request"));
			contentLabel->setText(tr("%1 is requesting to control your session remotely.\nDo you accept the request ?").arg(mPara1));
			break;
		case MESSAGE_CUSTOM_TYPE:
			titleLabel->setText(mPara1);
			contentLabel->setText(mPara2);
			break;
		default:
			qDebug("Unknow message %d type! Returning timeout!", mType);
			mResponse = -1;
			sendResponse();
			return false;
			break;
	}
	return true;
}

void QOgonMessageBoxBase::start() {
	if (!prepare()) {
		return;
	}

	if(mTimeout) {
		mTimer->start(1000);
	}
	adjustSize();
	setGeometry(
		QStyle::alignedRect(
			Qt::LeftToRight,
			Qt::AlignCenter,
			size(),
			qApp->desktop()->availableGeometry()
	));
	showMessagebox();
}

void QOgonMessageBoxBase::updateTime() {
	mCurrentTimeout--;
	timeout->setText(QString::number(mCurrentTimeout));
	if (!mCurrentTimeout){
		mTimer->stop();
		mResponse = -1;
		sendResponse();
	}
}

QOgonMessageBoxBase::~QOgonMessageBoxBase() {
	mTimer->stop();
	disconnect(this, 0, 0, 0);
	delete mTimer;
}

void QOgonMessageBoxBase::keyPressEvent(QKeyEvent* event)
{
	int response = -1;
	if (event->key() == Qt::Key_Escape) {
		switch (mStyle) {
			case MB_OK:
				response = 1;
				break;
			case MB_OKCANCEL:
				response = 2;
				break;
			case MB_ABORTRETRYIGNORE:
				// no action
				break;
			case MB_YESNOCANCEL:
				response = 2;
				break;
			case MB_YESNO:
				// no action
				break;
			case MB_RETRYCANCEL:
				response = 2;
				break;
			case MB_CANCELTRYCONTINUE:
				response = 2;
				break;
			default:
				qDebug("Unknow style %d! Ignoring esc key", mStyle);
				break;
		}
		if (response != -1) {
			mResponse = response;
			sendResponse();
			return;
		}
	}
	QWidget::keyPressEvent(event);
}

void QOgonMessageBoxBase::setupUi(QWidget *messageBox) {

	if (messageBox->objectName().isEmpty()) {
		messageBox->setObjectName(QStringLiteral("messageBox"));
	}
	messageBox->resize(835, 321);
	messageBox->setWindowTitle(QStringLiteral("Ogon message"));
	verticalLayout_2 = new QVBoxLayout(messageBox);
	verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
	content = new QVBoxLayout();
	content->setObjectName(QStringLiteral("content"));
	titleLabel = new QLabel(messageBox);
	titleLabel->setObjectName(QStringLiteral("titleLabel"));
	QFont font;
	font.setBold(true);
	font.setWeight(75);
	titleLabel->setFont(font);
	titleLabel->setText(QStringLiteral("TextLabel"));
	titleLabel->setAlignment(Qt::AlignCenter);

	content->addWidget(titleLabel);

	topSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

	content->addItem(topSpacer);

	contentLabel = new QLabel(messageBox);
	contentLabel->setObjectName(QStringLiteral("contentLabel"));
	contentLabel->setText(QStringLiteral("TextLabel"));

	content->addWidget(contentLabel);

	buttomSpacer = new QSpacerItem(20, 128, QSizePolicy::Minimum, QSizePolicy::Expanding);

	content->addItem(buttomSpacer);

	timeoutBox = new QHBoxLayout();
	timeoutBox->setObjectName(QStringLiteral("timeoutBox"));
	timeoutSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	timeoutBox->addItem(timeoutSpacer);

	timeoutLabel = new QLabel(messageBox);
	timeoutLabel->setObjectName(QStringLiteral("timeoutLabel"));

	timeoutBox->addWidget(timeoutLabel);

	timeout = new QLabel(messageBox);
	timeout->setObjectName(QStringLiteral("timeout"));
	timeout->setText(QStringLiteral("TextLabel"));

	timeoutBox->addWidget(timeout);


	content->addLayout(timeoutBox);

	buttonBox = new QHBoxLayout();
	buttonBox->setObjectName(QStringLiteral("buttonBox"));
	horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	buttonBox->addItem(horizontalSpacer);

	buttonOK = new QPushButton(messageBox);
	buttonOK->setObjectName(QStringLiteral("buttonOK"));

	buttonBox->addWidget(buttonOK);

	buttonYes = new QPushButton(messageBox);
	buttonYes->setObjectName(QStringLiteral("buttonYes"));

	buttonBox->addWidget(buttonYes);

	buttonNo = new QPushButton(messageBox);
	buttonNo->setObjectName(QStringLiteral("buttonNo"));

	buttonBox->addWidget(buttonNo);

	buttonAbort = new QPushButton(messageBox);
	buttonAbort->setObjectName(QStringLiteral("buttonAbort"));

	buttonBox->addWidget(buttonAbort);

	buttonRetry = new QPushButton(messageBox);
	buttonRetry->setObjectName(QStringLiteral("buttonRetry"));

	buttonBox->addWidget(buttonRetry);

	buttonIgnore = new QPushButton(messageBox);
	buttonIgnore->setObjectName(QStringLiteral("buttonIgnore"));

	buttonBox->addWidget(buttonIgnore);

	buttonCancel = new QPushButton(messageBox);
	buttonCancel->setObjectName(QStringLiteral("buttonCancel"));

	buttonBox->addWidget(buttonCancel);

	buttonTry = new QPushButton(messageBox);
	buttonTry->setObjectName(QStringLiteral("buttonTry"));

	buttonBox->addWidget(buttonTry);

	buttonContine = new QPushButton(messageBox);
	buttonContine->setObjectName(QStringLiteral("buttonContine"));

	buttonBox->addWidget(buttonContine);


	content->addLayout(buttonBox);


	verticalLayout_2->addLayout(content);


	retranslateUi(messageBox);

	QMetaObject::connectSlotsByName(messageBox);
} // setupUi

void QOgonMessageBoxBase::retranslateUi(QWidget *messageBox) {

	timeoutLabel->setText(QApplication::translate("messageBox", "Timeout:", 0));
	buttonOK->setText(QApplication::translate("messageBox", "OK", 0));
	buttonYes->setText(QApplication::translate("messageBox", "Yes", 0));
	buttonNo->setText(QApplication::translate("messageBox", "No", 0));
	buttonAbort->setText(QApplication::translate("messageBox", "Abort", 0));
	buttonRetry->setText(QApplication::translate("messageBox", "Retry", 0));
	buttonIgnore->setText(QApplication::translate("messageBox", "Ignore", 0));
	buttonCancel->setText(QApplication::translate("messageBox", "Cancel", 0));
	buttonTry->setText(QApplication::translate("messageBox", "Try", 0));
	buttonContine->setText(QApplication::translate("messageBox", "Continue", 0));
	Q_UNUSED(messageBox);
} // retranslateUi

uint QOgonMessageBoxBase::getId() {
	return mId;
}
uint QOgonMessageBoxBase::getResponse() {
	return mResponse;
}
