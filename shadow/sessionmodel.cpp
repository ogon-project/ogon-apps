/**
 * ogon - Free Remote Desktop Services
 * Shadowing Application - Basic Data Model for Sessions
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

#include "sessionmodel.h"
#include <QDebug>

int SessionModel::rowCount(const QModelIndex &parent) const {
	return mSessions.count();
}

int SessionModel::columnCount (const QModelIndex &parent) const {
	return FinalElement;
}

QVariant SessionModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid() || index.row() >= mSessions.count() ) {
		return QVariant();
	}

	if (role == Qt::DisplayRole) {
		switch(index.column()) {
			case SessionID:
				return QVariant((qulonglong)mSessions[index.row()]->mSessionId);
			case UserName:
				return mSessions[index.row()]->mUserName;
			case ClientName:
				return mSessions[index.row()]->mClientName;
			case WinstationName:
				return mSessions[index.row()]->mWinStationName;
			case LogonTime:
				return mSessions[index.row()]->mConnectionTime;
			case State:
				return QString(stateToString(mSessions[index.row()]->mState));
			default:
				return QVariant();
		}
	}
	return QVariant();
}

QVariant SessionModel::headerData(int section, Qt::Orientation orientation, int role ) const {

	if (role != Qt::DisplayRole) {
		return QVariant();
	}

	if (orientation == Qt::Horizontal) {
		switch (section) {
			case SessionID:
				return QString(tr("ID"));
			case UserName:
				return QString(tr("User"));
			case ClientName:
				return QString(tr("Client Name"));
			case State:
				return QString(tr("Status"));
			case LogonTime:
				return QString(tr("Logon Time"));
			case WinstationName:
				return QString(tr("Session"));
			default:
				return QVariant();
		}
	} else {
		return QVariant();
	}
}

void SessionModel::getSessions() {
	int ret;
	DWORD count;
	PWTS_SESSION_INFO pSessionInfo;

	ret = WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1, &pSessionInfo, &count);
	if (!ret) {
		qDebug("Session enumeration failed: %u\n", GetLastError());
		return;
	}

	for (int i = 0; i < count; ++i) {
		Session *session = new Session;
		session->mSessionId = (unsigned int)pSessionInfo[i].SessionId;
		getSessionDetails(session);
		mSessions.append(session);
	}
	WTSFreeMemory(pSessionInfo);

#if 0 // test data when testing/developing outside of a session
	Session *sess1 = new Session;
	Session *sess2 = new Session;
	Session *sess3 = new Session;

	sess1->mSessionId = 1;
	sess2->mSessionId = 2;
	sess3->mSessionId = 4;

	sess1->mWinStationName = "winstation1";
	sess2->mWinStationName = "winstation2winstation2winstation2";
	sess3->mWinStationName = "winstation3";

	sess1->mClientName = "client1";
	sess2->mClientName = "client2";
	sess3->mClientName = "client3";

	sess1->mState = WTSActive;
	sess2->mState = WTSConnected;
	sess3->mState = WTSDisconnected;

	sess1->mConnectionTime.setTime(QTime::currentTime());
	sess2->mConnectionTime.setTime(QTime::currentTime());
	sess3->mConnectionTime.setTime(QTime::currentTime());

	sess1->mUserName = "user1";
	sess2->mUserName = "user2";
	sess3->mUserName = "user3";

	mSessions.append(sess1);
	mSessions.append(sess2);
	mSessions.append(sess3);
#endif

}

SessionModel::SessionModel(QObject *parent) : QAbstractTableModel(parent) {
	getSessions();
}

const char *SessionModel::stateToString(WTS_CONNECTSTATE_CLASS connectState) const {
	switch (connectState) {
		case WTSActive:
			return "Active";
		case WTSConnected:
			return "Connected";
		case WTSConnectQuery:
			return "ConnectQuery";
		case WTSShadow:
			return "Shadow";
		case WTSDisconnected:
			return "Disconnected";
		case WTSIdle:
			return "Idle";
		case WTSListen:
			return "Listen";
		case WTSReset:
			return "Reset";
		case WTSDown:
			return "Down";
		case WTSInit:
			return "Init";
		default:
			return "Unkown";
	}
}

bool SessionModel::getSessionDetails(Session *session) {
	unsigned int bytes;
	int ret;
	LPSTR buf = NULL;
	DWORD sessionId = session->mSessionId;

	ret = WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, session->mSessionId, WTSSessionInfo, &buf, &bytes);
	if (!ret) {
		qDebug("WTSQuerySessionInformation WTSSessionInfo failed: %u\n", GetLastError());
		return false;
	}

	PWTSINFO info = (PWTSINFO)buf;
	session->mUserName = QString::fromLocal8Bit(info->UserName);
	session->mState = info->State;
	session->mConnectionTime.setTime_t(WindowsFileTimeToTimeT((unsigned long long int) info->ConnectTime.QuadPart));
	session->mWinStationName = QString::fromLocal8Bit(info->WinStationName);
	WTSFreeMemory(buf);

	ret = WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, session->mSessionId, WTSClientName, &buf, &bytes);
	if (!ret) {
		qDebug("WTSQuerySessionInformation WTSClientName failed: %u\n", GetLastError());
		return false;
	}

	session->mClientName = QString(buf);
	WTSFreeMemory(buf);
	return true;
}

time_t SessionModel::WindowsFileTimeToTimeT(const unsigned long long int &ft) {
	unsigned long long int ull = ft;
	ull -= 116444736000000000;
	ull /= 10000000;
	return static_cast<time_t>(ull);
}

bool SessionModel::refresh() {
	beginResetModel();
	mSessions.clear();
	getSessions();
	endResetModel();
	return true;
}
