// Copyright (c) 2017-2018 The PIVX developers
// Copyright (c) 2018-2019 The Dilithium Core developers
// Copyright (c) 2019 The Kahsh Core developers

// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ZKSHCONTROLDIALOG_H
#define ZKSHCONTROLDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "primitives/zerocoin.h"
#include "privacydialog.h"

class CZerocoinMint;
class WalletModel;

namespace Ui {
class ZXdhControlDialog;
}

class CZXdhControlWidgetItem : public QTreeWidgetItem
{
public:
    explicit CZXdhControlWidgetItem(QTreeWidget *parent, int type = Type) : QTreeWidgetItem(parent, type) {}
    explicit CZXdhControlWidgetItem(int type = Type) : QTreeWidgetItem(type) {}
    explicit CZXdhControlWidgetItem(QTreeWidgetItem *parent, int type = Type) : QTreeWidgetItem(parent, type) {}

    bool operator<(const QTreeWidgetItem &other) const;
};

class ZXdhControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ZXdhControlDialog(QWidget *parent);
    ~ZXdhControlDialog();

    void setModel(WalletModel* model);

    static std::set<std::string> setSelectedMints;
    static std::set<CMintMeta> setMints;
    static std::vector<CMintMeta> GetSelectedMints();

private:
    Ui::ZXdhControlDialog *ui;
    WalletModel* model;
    PrivacyDialog* privacyDialog;

    void updateList();
    void updateLabels();

    enum {
        COLUMN_CHECKBOX,
        COLUMN_DENOMINATION,
        COLUMN_PUBCOIN,
        COLUMN_VERSION,
        COLUMN_CONFIRMATIONS,
        COLUMN_ISSPENDABLE
    };
    friend class CZXdhControlWidgetItem;

private slots:
    void updateSelection(QTreeWidgetItem* item, int column);
    void ButtonAllClicked();
};

#endif // ZKSHCONTROLDIALOG_H
