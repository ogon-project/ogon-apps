/**
 * ogon - Free Remote Desktop Services
 * Shadowing Application - Worker Thread Class
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

#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H

#include <qthread.h>
#include <winpr/wtypes.h>

class WorkerThread : public QThread
{
public:
	WorkerThread(ULONG TargetLogonId, BYTE HotkeyVk, USHORT HotkeyModifiers, DWORD flags);
	Q_OBJECT
	void run() Q_DECL_OVERRIDE;
signals:
	void resultReady(const bool result, const unsigned int lastError, unsigned long sessionId);

private:
	ULONG	mTargetLogonId;
	BYTE	mHotkeyVk;
	USHORT	mHotkeyModifiers;
	DWORD	mflags;
};

#endif // WORKER_THREAD_H
