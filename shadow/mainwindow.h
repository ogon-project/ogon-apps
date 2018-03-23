/**
 * ogon - Free Remote Desktop Services
 * Shadowing Application - QMainWindow Subclass
 *
 * Copyright (c) 2014-2018 Thincast Technologies GmbH
 *
 * Authors:
 * Bernhard Miklautz <bernhard.miklautz@thincast.com>
 * Norbert Federa <norbert.federa@thincast.com>
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressDialog>
#include <winpr/input.h>

class QPushButton;
class QProgressDialog;
class QTableView;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QGroupBox;
class QComboBox;
class QCheckBox;
class QModelIndex;

class PleaseWaitDialog : public QProgressDialog
{
	Q_OBJECT
public:
	PleaseWaitDialog(QWidget *parent = NULL) : QProgressDialog(parent) {
		setMinimum(0);
		setLabelText(tr("Please wait ..."));
		setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
		setCancelButton(NULL);
		setWindowModality(Qt::WindowModal);
		setMinimumDuration(0);
		deactivate();

	};

	~ PleaseWaitDialog() { };

	void activate() {
		setMaximum(0);
		setValue(0);
		show();
	}

	void deactivate() {
		setMaximum(1);
		setValue(1);
		hide();
	}

public slots:
	virtual void reject() {
		// We ignore the Escape key
	};
};

class MainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		MainWindow(QWidget *parent = 0);

	private:
		QWidget *mCentralWidget;
		QVBoxLayout *mMainLayout;
		QTableView  *mTableView;

		QHBoxLayout *mHBoxLayout;
		QVBoxLayout *mleftLayout;

		QHBoxLayout *mKeyGroupLayout;
		QGroupBox *mKeyGroup;
		QCheckBox *mShift;
		QCheckBox *mCtrl;
		QCheckBox *mAlt;
		QComboBox *mKeyCode;
		QCheckBox *mViewOnly;

		QGroupBox *mOptionGroup;
		QHBoxLayout *mOptions;

		QVBoxLayout *mButtonLayout;
		QPushButton *mConnect;
		QPushButton *mCancel;
		QPushButton *mRefresh;

		PleaseWaitDialog *mPleaseWaitDialog;

		bool mWorkerRunning;
		void initKeyBox();

	public slots:
		void activatePleaseWaitDialog();
		void connectClicked(bool checked);
		void tableClicked(const QModelIndex &);
		void refreshClicked(bool checked);
		void finishedWorker(bool result, unsigned int lastError, unsigned long sessionID);
};

#endif // MAINWINDOW_H
