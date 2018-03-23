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

#include "workerthread.h"
#include <winpr/wtsapi.h>

WorkerThread::WorkerThread(ULONG TargetLogonId, BYTE HotkeyVk, USHORT HotkeyModifiers, DWORD flags) {
	mTargetLogonId = TargetLogonId;
	mHotkeyVk = HotkeyVk;
	mHotkeyModifiers = HotkeyModifiers;
	mflags = flags;
}

void WorkerThread::run() {
	BOOL result;
	DWORD lastError = 0;
	result = WTSStartRemoteControlSessionEx(NULL, mTargetLogonId, mHotkeyVk, mHotkeyModifiers, mflags);
	if (!result) {
		lastError = GetLastError();
	}
	emit resultReady(result, lastError, mTargetLogonId);
}
