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

#ifndef SESSIONMODEL_H
#define SESSIONMODEL_H

#include <QAbstractTableModel>
#include <QString>
#include <QTime>
#include <winpr/wtsapi.h>

class SessionModel : public QAbstractTableModel {
		Q_OBJECT

	public:
		SessionModel(QObject *parent = 0);

		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount (const QModelIndex &parent = QModelIndex()) const;
		QVariant data(const QModelIndex &index, int role) const;
		QVariant headerData(int section, Qt::Orientation orientation,
		                    int role = Qt::DisplayRole) const;
		static time_t WindowsFileTimeToTimeT(const unsigned long long int &ft);

		enum RowIndex {
			UserName,
			SessionID,
			State,
			ClientName,
			WinstationName,
			LogonTime,
			FinalElement
		};
		bool refresh();

	private:
		class Session {
			public:
				unsigned long mSessionId;
				QString mUserName;
				QString mClientName;
				QString mWinStationName;
				QDateTime mConnectionTime;
				WTS_CONNECTSTATE_CLASS mState;
		};
		void getSessions();
		bool getSessionDetails(Session *session);
		const char *stateToString(WTS_CONNECTSTATE_CLASS state) const;
		QList<Session *> mSessions;
		bool mUseNotification;
};
#endif /* SESSIONMODEL_H */
