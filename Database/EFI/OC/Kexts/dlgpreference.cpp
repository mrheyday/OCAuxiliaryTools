#include "dlgpreference.h"

#include <QDir>
#include <QKeyEvent>
#include <QSettings>

#include "Method.h"
#include "mainwindow.h"
#include "ui_dlgpreference.h"
#include "ui_mainwindow.h"

extern MainWindow *mw_one;
extern Method *mymethod;
extern QString SaveFileName, strIniFile, strAppName, ocFromDev, strOCFromDev,
    strAppExePath;
extern bool blDEV;
extern QProgressBar *progBar;

dlgPreference::dlgPreference(QWidget *parent)
    : QDialog(parent), ui(new Ui::dlgPreference) {
  ui->setupUi(this);

  for (int i = 0; i < ui->tableKextUrl->columnCount(); i++) {
    ui->tableKextUrl->horizontalHeader()->setSectionResizeMode(
        i, QHeaderView::ResizeToContents);
  }

  // Kexts Urls
  ui->tableKextUrl->horizontalHeader()->setStretchLastSection(true);
  ui->tableKextUrl->setColumnWidth(0, 200);
  ui->tableKextUrl->setColumnWidth(1, 400);
  ui->textEdit->setHidden(true);

  strIniFile =
      QDir::homePath() + "/.config/" + strAppName + "/" + strAppName + ".ini";

  QSettings Reg(strIniFile, QSettings::IniFormat);

  QFileInfo fi(strIniFile);
  QString strDef = "https://ghproxy.com/https://github.com/";
  QLocale locale;
  if (fi.exists()) {
    if (locale.language() == QLocale::Chinese) {
      ui->comboBoxNet->setCurrentText(Reg.value("Net", strDef).toString());
    } else {
      ui->comboBoxNet->setCurrentText(
          Reg.value("Net", "https://github.com/").toString());
    }

    ui->comboBoxWeb->setCurrentText(
        Reg.value("Web", "https://github.com/").toString());
    ui->rbtnAPI->setChecked(Reg.value("rbtnAPI").toBool());
    ui->rbtnWeb->setChecked(Reg.value("rbtnWeb").toBool());
    ui->chkBoxLastFile->setChecked(Reg.value("LastFile").toBool());

  } else {
    if (locale.language() == QLocale::Chinese) {
      ui->comboBoxNet->setCurrentText(strDef);

    } else {
      ui->comboBoxNet->setCurrentText("https://github.com/");
    }
  }
  ui->chkShowVolName->setChecked(Reg.value("ShowVolName", 0).toBool());

  ui->chkRecentOpen->setChecked(Reg.value("chkRecentOpen", 1).toBool());
  ui->chkOpenDir->setChecked(Reg.value("chkOpenDir", 1).toBool());
  ui->chkMountESP->setChecked(Reg.value("chkMountESP", 1).toBool());
  ui->chkBackupEFI->setChecked(Reg.value("chkBackupEFI", 1).toBool());
  ui->chkDatabase->setChecked(Reg.value("chkDatabase", 1).toBool());
  ui->chkHideToolbar->setChecked(Reg.value("chkHideToolbar", 0).toBool());
  ui->chkSmartKey->setChecked(Reg.value("SmartKey", 1).toBool());
  ui->chkSmartKey->setHidden(true);
}

dlgPreference::~dlgPreference() { delete ui; }

void dlgPreference::closeEvent(QCloseEvent *event) {
  if (!ui->btnDownloadKexts->isEnabled()) event->ignore();
  saveKextUrl();
}

void dlgPreference::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Escape:
      close();
      break;

    case Qt::Key_Return:

      break;

    case Qt::Key_Backspace:

      break;

    case Qt::Key_Space:

      break;

    case Qt::Key_F1:

      break;
  }

  if (event->modifiers() == Qt::ControlModifier) {
    if (event->key() == Qt::Key_M) {
      this->setWindowState(Qt::WindowMaximized);
    }
  }
}

void dlgPreference::saveKextUrl() {
  ui->textEdit->clear();
  QString str0, str1;
  for (int i = 0; i < ui->tableKextUrl->rowCount(); i++) {
    str0 = ui->tableKextUrl->item(i, 0)->text().trimmed();
    str1 = ui->tableKextUrl->item(i, 1)->text().trimmed();
    if (str0 != "" || str1 != "") ui->textEdit->append(str0 + " | " + str1);
  }
  mymethod->TextEditToFile(ui->textEdit, mw_one->strConfigPath + "KextUrl.txt");
}

void dlgPreference::refreshKextUrl() {
  QString file = strAppExePath + "/Database/preset/KextUrl.txt";
  ui->textEdit->clear();
  ui->textEdit->setPlainText(mymethod->loadText(file));
  QTextEdit *txtEdit = new QTextEdit;
  QString txt = mymethod->loadText(mw_one->strConfigPath + "KextUrl.txt");
  txtEdit->setPlainText(txt);
  for (int i = 0; i < txtEdit->document()->lineCount(); i++) {
    QString line = mymethod->getTextEditLineText(txtEdit, i).trimmed();
    bool re = false;
    for (int j = 0; j < ui->textEdit->document()->lineCount(); j++) {
      QString line2 = mymethod->getTextEditLineText(ui->textEdit, j).trimmed();
      if (line == line2) {
        re = true;
      }
    }
    if (!re) ui->textEdit->append(line);
  }

  QStringList listKexts;
  for (int i = 0; i < ui->textEdit->document()->lineCount(); i++) {
    QString line = mymethod->getTextEditLineText(ui->textEdit, i).trimmed();
    if (line != "") listKexts.append(line);
  }

  std::sort(listKexts.begin(), listKexts.end(),
            [](const QString &s1, const QString &s2) { return s1 < s2; });

  ui->tableKextUrl->setRowCount(0);
  ui->tableKextUrl->setRowCount(listKexts.count());
  for (int i = 0; i < listKexts.count(); i++) {
    QString line = listKexts.at(i);
    QStringList list = line.split("|");
    QString str0, str1;
    if (list.count() == 2) {
      str0 = list.at(0);
      str1 = list.at(1);
      ui->tableKextUrl->setCurrentCell(i, 0);
      ui->tableKextUrl->setItem(i, 0, new QTableWidgetItem(str0.trimmed()));
      ui->tableKextUrl->setItem(i, 1, new QTableWidgetItem(str1.trimmed()));
    }
  }

  ui->tableKextUrl->setFocus();
}

void dlgPreference::on_btnAdd_clicked() {
  int n = ui->tableKextUrl->rowCount();
  ui->tableKextUrl->setRowCount(n + 1);
  ui->tableKextUrl->setCurrentCell(n, 0);
  ui->tableKextUrl->setItem(n, 0, new QTableWidgetItem(""));
  ui->tableKextUrl->setItem(n, 1, new QTableWidgetItem(""));
}

void dlgPreference::on_btnDel_clicked() {
  if (ui->tableKextUrl->rowCount() == 0) return;
  int n = ui->tableKextUrl->currentRow();
  ui->tableKextUrl->removeRow(n);
  if (n - 1 == -1) n = 1;
  ui->tableKextUrl->setCurrentCell(n - 1, 0);
  ui->tableKextUrl->setFocus();
}

void dlgPreference::on_btnOpenUrl_clicked() {
  if (!ui->tableKextUrl->currentIndex().isValid()) return;
  int n = ui->tableKextUrl->currentRow();
  QString strurl = ui->tableKextUrl->item(n, 1)->text().trimmed();
  QUrl url(strurl);
  QDesktopServices::openUrl(url);
}

void dlgPreference::on_rbtnAPI_clicked() {
  QSettings Reg(strIniFile, QSettings::IniFormat);
  Reg.setValue("rbtnAPI", ui->rbtnAPI->isChecked());
  Reg.setValue("rbtnWeb", ui->rbtnWeb->isChecked());
}

void dlgPreference::on_rbtnWeb_clicked() { on_rbtnAPI_clicked(); }

void dlgPreference::on_btnTest_clicked() {
  mw_one->on_actionOnline_Download_Updates_triggered();
  mw_one->dlgAutoUpdate->ui->btnStartUpdate->setHidden(true);
}

void dlgPreference::on_comboBoxNet_currentTextChanged(const QString &arg1) {
  QSettings Reg(strIniFile, QSettings::IniFormat);
  Reg.setValue("Net", arg1);
}

void dlgPreference::on_comboBoxWeb_currentTextChanged(const QString &arg1) {
  QSettings Reg(strIniFile, QSettings::IniFormat);
  Reg.setValue("Web", arg1);
}

void dlgPreference::writeIni(QString key, int arg1) {
  QSettings Reg(strIniFile, QSettings::IniFormat);
  Reg.setValue(key, arg1);
}

void dlgPreference::on_chkBoxLastFile_clicked(bool checked) {
  QSettings Reg(strIniFile, QSettings::IniFormat);
  Reg.setValue("LastFile", checked);
}

void dlgPreference::on_chkOpenDir_stateChanged(int arg1) {
  writeIni("chkOpenDir", arg1);
}

void dlgPreference::on_chkRecentOpen_stateChanged(int arg1) {
  writeIni("chkRecentOpen", arg1);
}

void dlgPreference::on_chkMountESP_stateChanged(int arg1) {
  writeIni("chkMountESP", arg1);
}

void dlgPreference::on_chkBackupEFI_stateChanged(int arg1) {
  writeIni("chkBackupEFI", arg1);
}

void dlgPreference::on_chkDatabase_stateChanged(int arg1) {
  writeIni("chkDatabase", arg1);
}

void dlgPreference::on_chkHideToolbar_stateChanged(int arg1) {
  writeIni("chkHideToolbar", arg1);
}

void dlgPreference::on_chkShowVolName_clicked(bool checked) {
  QSettings Reg(strIniFile, QSettings::IniFormat);
  Reg.setValue("ShowVolName", checked);
}

void dlgPreference::on_btnPing_clicked() {
  QString strurl = ui->comboBoxWeb->currentText();
  QUrl url(strurl);
  QDesktopServices::openUrl(url);
}

void dlgPreference::on_chkSmartKey_clicked(bool checked) {
  QSettings Reg(strIniFile, QSettings::IniFormat);
  Reg.setValue("SmartKey", checked);
}

void dlgPreference::on_btnDownloadKexts_clicked() {
  if (!mw_one->dlgSyncOC->ui->btnCheckUpdate->isEnabled()) return;
  if (!mw_one->dlgSyncOC->ui->btnUpdateOC->isEnabled()) return;
  ui->btnDownloadKexts->setEnabled(false);
  repaint();
  // mw_one->dlgSyncOC->isCheckOC = true;
  progBar = new QProgressBar(this);
  progBar->setTextVisible(false);
  progBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  progBar->setStyleSheet(
      "QProgressBar{border:0px solid #FFFFFF;"
      "height:30;"
      "background:rgba(25,255,25,0);"
      "text-align:right;"
      "color:rgb(255,255,255);"
      "border-radius:0px;}"

      "QProgressBar:chunk{"
      "border-radius:0px;"
      "background-color:rgba(25,255,0,100);"
      "}");

  mymethod->downloadAllKexts();

  for (int i = 0; i < ui->tableKextUrl->rowCount(); i++)
    ui->tableKextUrl->removeCellWidget(i, 0);
  ui->btnDownloadKexts->setEnabled(true);
  repaint();
}

void dlgPreference::on_btnStop_clicked() {
  if (ui->btnDownloadKexts->isEnabled()) return;

  mymethod->cancelKextUpdate();
}