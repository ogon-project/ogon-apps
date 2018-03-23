/**
 * ogon - Free Remote Desktop Services
 * Shadowing Application - QMainWindow Subclass
 *
 * Copyright (c) 2014-2018 Thincast Technologies GmbH
 *
 * Authors:
 * Bernhard Miklautz <bernhard.miklautz@thincast.com>
 * Martin Haimberger <martin.haimberger@thincast.com>
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

#include "mainwindow.h"

#include <QPushButton>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QCheckBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QModelIndex>
#include <QTimer>
#include <QDebug>

#include "sessionmodel.h"
#include "workerthread.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	setWindowTitle(tr("Select Session"));

	mCentralWidget = new QWidget();
	mMainLayout = new QVBoxLayout;

	mTableView = new QTableView(this);
	mTableView->setModel(new SessionModel);
	mTableView->setAlternatingRowColors(false);
	mTableView->setShowGrid(false);
	mTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	mTableView->setSelectionMode(QAbstractItemView::SingleSelection);
	mTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	mTableView->setCornerButtonEnabled(false);
	//mTableView->setSortingEnabled(true);
	mTableView->setWordWrap(false);

	mTableView->verticalHeader()->hide();
	mTableView->verticalHeader()->setHighlightSections(false);

	//mTableView->horizontalHeader()->setStretchLastSection(true);
	mTableView->horizontalHeader()->setHighlightSections(false);
	mTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	mTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	mTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	mTableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
	mTableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);

	/* set the minimum with to something sensible */
	mTableView->setMinimumWidth(600);

	connect(mTableView, SIGNAL(clicked(QModelIndex)), this, SLOT(tableClicked(QModelIndex)));
	mMainLayout->addWidget(mTableView);

	mHBoxLayout = new QHBoxLayout;
	mleftLayout = new QVBoxLayout;

	mKeyGroupLayout = new QHBoxLayout;

	mShift = new QCheckBox("Shift");
	mShift->setCheckState(Qt::Checked);
	mKeyGroupLayout->addWidget(mShift);

	mCtrl = new QCheckBox("Ctrl");
	mCtrl->setCheckState(Qt::Checked);
	mKeyGroupLayout->addWidget(mCtrl);

	mAlt = new QCheckBox("Alt");
	mAlt->setCheckState(Qt::Unchecked);
	mKeyGroupLayout->addWidget(mAlt);

	mKeyCode = new QComboBox;
	mKeyGroup = new QGroupBox(tr("Shortcut to return to this session:"));
	initKeyBox();
	mKeyGroupLayout->addWidget(mKeyCode);
	mKeyGroup->setLayout(mKeyGroupLayout);
	mleftLayout->addWidget(mKeyGroup);

	mOptions = new QHBoxLayout;
	mViewOnly = new QCheckBox(tr("View Only"));
	mViewOnly->setCheckState(Qt::Unchecked);
	mOptions->addWidget(mViewOnly);

	mOptionGroup = new QGroupBox(tr("Options"));
	mOptionGroup->setLayout(mOptions);
	mleftLayout->addWidget(mOptionGroup);

	mHBoxLayout->addLayout(mleftLayout);

	mConnect = new QPushButton(tr("C&onnect"), mCentralWidget);
	mConnect->setEnabled(false);

	mRefresh = new QPushButton(tr("&Refresh"), mCentralWidget);

	mCancel = new QPushButton(tr("&Cancel"), mCentralWidget);
	connect(mCancel, SIGNAL(clicked()), this, SLOT(close()));
	connect(mConnect, SIGNAL(clicked(bool)), this, SLOT(connectClicked(bool)));
	connect(mRefresh, SIGNAL(clicked(bool)), this, SLOT(refreshClicked(bool)));

	mButtonLayout = new QVBoxLayout;
	mButtonLayout->addWidget(mRefresh);
	mButtonLayout->addWidget(mConnect);
	mButtonLayout->addWidget(mCancel);
	mHBoxLayout->insertSpacing(-1, 20);
	mHBoxLayout->addLayout(mButtonLayout);

	mMainLayout->addLayout(mHBoxLayout);
	mCentralWidget->setLayout(mMainLayout);
	setCentralWidget(mCentralWidget);

	mPleaseWaitDialog = new PleaseWaitDialog(this);
	mPleaseWaitDialog->deactivate();
	mPleaseWaitDialog->setLabelText(tr("<b>Remote control request initialized.</b><br/><br>Please wait ..."));
	mPleaseWaitDialog->setMinimumSize(320, 150);

	mWorkerRunning = false;

}

void MainWindow::activatePleaseWaitDialog() {
	if (!mWorkerRunning) {
		return;
	}
	mPleaseWaitDialog->activate();
}

void MainWindow::refreshClicked(bool checked) {
	mConnect->setEnabled(false);
	((SessionModel*)mTableView->model())->refresh();
}

void MainWindow::connectClicked(bool checked) {
	USHORT modifiers = 0;
	unsigned long sessionId;

	if (mWorkerRunning)
		return;

	QItemSelectionModel *select = mTableView->selectionModel();
	if (!select->hasSelection()) {
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setText(tr("No session selected"));
		msgBox.exec();
		return;
	}

	if (mShift->isChecked()) {
		modifiers |= REMOTECONTROL_KBDSHIFT_HOTKEY;
	}
	if (mCtrl->isChecked()) {
		modifiers |= REMOTECONTROL_KBDCTRL_HOTKEY;
	}
	if (mAlt->isChecked()) {
		modifiers |= REMOTECONTROL_KBDALT_HOTKEY;
	}

	QModelIndex index = mTableView->model()->index(select->selectedRows().at(0).row(), SessionModel::SessionID, QModelIndex());
	sessionId = index.data().toULongLong();
	//    qDebug() << "starting shadowing" << sessionId << "key" << mKeyCode->itemData(mKeyCode->currentIndex()).toUInt() << "modifiers" << modifiers;
	mConnect->setEnabled(false);

	WorkerThread *workerThread = new WorkerThread(sessionId, mKeyCode->itemData(mKeyCode->currentIndex()).toUInt(), modifiers,
		mViewOnly->isChecked() ? REMOTECONTROL_FLAG_DISABLE_INPUT : 0);
	connect(workerThread, &WorkerThread::resultReady, this, &MainWindow::finishedWorker);
	connect(workerThread, &WorkerThread::finished, workerThread, &QObject::deleteLater);

	QTimer::singleShot(500, this, SLOT(activatePleaseWaitDialog()));

	workerThread->start();
	mWorkerRunning = true;
}

void MainWindow::tableClicked(const QModelIndex &) {
	if (!mWorkerRunning) {
		mConnect->setEnabled(true);
	}
}

void MainWindow::finishedWorker(bool result, unsigned int lastError, unsigned long sessionId) {
	mWorkerRunning = false;
	mPleaseWaitDialog->deactivate();

	if (!result) {
		QMessageBox msgBox(this);
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setText(tr("Could not start shadowing of session %1").arg(sessionId));
		msgBox.setInformativeText(tr("Error code %1").arg(lastError));
		msgBox.exec();
	}

	mConnect->setEnabled(true);
}


void MainWindow::initKeyBox() {
	mKeyCode->addItem(QString("Space"), VK_SPACE);
	mKeyCode->addItem(QString("KP_Plus"), VK_ADD);
	mKeyCode->addItem(QString("KP_Minus"), VK_SUBTRACT);
	mKeyCode->addItem(QString("KP_Multiply"), VK_MULTIPLY);
	mKeyCode->addItem(QString("KP_Divide"), VK_DIVIDE);

	mKeyCode->addItem(QString("0"), VK_KEY_0);
	mKeyCode->addItem(QString("1"), VK_KEY_1);
	mKeyCode->addItem(QString("2"), VK_KEY_2);
	mKeyCode->addItem(QString("3"), VK_KEY_3);
	mKeyCode->addItem(QString("4"), VK_KEY_4);
	mKeyCode->addItem(QString("5"), VK_KEY_5);
	mKeyCode->addItem(QString("6"), VK_KEY_6);
	mKeyCode->addItem(QString("7"), VK_KEY_7);
	mKeyCode->addItem(QString("8"), VK_KEY_8);
	mKeyCode->addItem(QString("9"), VK_KEY_9);

	mKeyCode->addItem(QString("a"), VK_KEY_A);
	mKeyCode->addItem(QString("b"), VK_KEY_B);
	mKeyCode->addItem(QString("c"), VK_KEY_C);
	mKeyCode->addItem(QString("d"), VK_KEY_D);
	mKeyCode->addItem(QString("e"), VK_KEY_E);
	mKeyCode->addItem(QString("f"), VK_KEY_F);
	mKeyCode->addItem(QString("g"), VK_KEY_G);
	mKeyCode->addItem(QString("h"), VK_KEY_H);
	mKeyCode->addItem(QString("i"), VK_KEY_I);
	mKeyCode->addItem(QString("j"), VK_KEY_J);
	mKeyCode->addItem(QString("k"), VK_KEY_K);
	mKeyCode->addItem(QString("l"), VK_KEY_L);
	mKeyCode->addItem(QString("m"), VK_KEY_M);
	mKeyCode->addItem(QString("n"), VK_KEY_N);
	mKeyCode->addItem(QString("o"), VK_KEY_O);
	mKeyCode->addItem(QString("p"), VK_KEY_P);
	mKeyCode->addItem(QString("q"), VK_KEY_Q);
	mKeyCode->addItem(QString("r"), VK_KEY_R);
	mKeyCode->addItem(QString("s"), VK_KEY_S);
	mKeyCode->addItem(QString("t"), VK_KEY_T);
	mKeyCode->addItem(QString("u"), VK_KEY_U);
	mKeyCode->addItem(QString("v"), VK_KEY_V);
	mKeyCode->addItem(QString("w"), VK_KEY_W);
	mKeyCode->addItem(QString("x"), VK_KEY_X);
	mKeyCode->addItem(QString("y"), VK_KEY_Y);
	mKeyCode->addItem(QString("z"), VK_KEY_Z);

	mKeyCode->addItem(QString("F2"), VK_F2);
	mKeyCode->addItem(QString("F3"), VK_F3);
	mKeyCode->addItem(QString("F4"), VK_F4);
	mKeyCode->addItem(QString("F5"), VK_F5);
	mKeyCode->addItem(QString("F6"), VK_F6);
	mKeyCode->addItem(QString("F7"), VK_F7);
	mKeyCode->addItem(QString("F8"), VK_F8);
	mKeyCode->addItem(QString("F9"), VK_F9);
	mKeyCode->addItem(QString("F10"), VK_F10);
	mKeyCode->addItem(QString("F11"), VK_F11);
	mKeyCode->addItem(QString("F12"), VK_F12);

	mKeyCode->addItem(QString("Esc"), VK_ESCAPE);
	mKeyCode->addItem(QString("BackSpace"), VK_BACK);
	mKeyCode->addItem(QString("Tab"), VK_TAB);
	mKeyCode->addItem(QString("Enter"), VK_RETURN);
	mKeyCode->addItem(QString("PrtScrn"), VK_PRINT);
	mKeyCode->addItem(QString("Left"), VK_LEFT);
	mKeyCode->addItem(QString("Right"), VK_RIGHT);
	mKeyCode->addItem(QString("Up"), VK_UP);
	mKeyCode->addItem(QString("Down"), VK_DOWN);

	mKeyCode->setCurrentText("F10");
}
