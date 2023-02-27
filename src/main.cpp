#include <QApplication>

#include "Method.h"
#include "mainwindow.h"
#include "myapp.h"
#include "fileoperation.h"

void loadLocal();
extern QVector<QString> filelist;
QWidgetList wdlist;
extern QString PlistFileName, CurVersion, ocVer;
extern bool zh_cn;
MainWindow *mw_one;

int main(int argc, char *argv[]) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
  {
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
  }
#endif

  MyApplication *a = new MyApplication(argc, argv);
#ifdef Q_OS_MAC
  Method::init_MacVerInfo(CurVersion);
#endif
  loadLocal();

  QDir dir;
  dir.mkpath(QDir::homePath() + "/.ocat/");
  QString strAppExePath = qApp->applicationDirPath();
  FileOperation::copyDirectoryFiles(strAppExePath + "/Database/",
                                      QDir::homePath() + "/.ocat/Database/", false);

  QString strDrivers0 = strAppExePath + "/Database/EFI/OC/Drivers/";
  QString strTools0 = strAppExePath + "/Database/EFI/OC/Tools/";
  QString strDrivers1 = QDir::homePath() + "/.ocat/Database/EFI/OC/Drivers/";
  QString strTools1 = QDir::homePath() + "/.ocat/Database/EFI/OC/Tools/";
  QString strDrivers2 = QDir::homePath() + "/.ocat/devDatabase/EFI/OC/Drivers/";
  QString strTools2 = QDir::homePath() + "/.ocat/devDatabase/EFI/OC/Tools/";
  QString strDrivers3 =
      QDir::homePath() + "/.ocat/Database/DEBUG/EFI/OC/Drivers/";
  QString strTools3 = QDir::homePath() + "/.ocat/Database/DEBUG/EFI/OC/Tools/";
  QString strDrivers4 =
      QDir::homePath() + "/.ocat/devDatabase/DEBUG/EFI/OC/Drivers/";
  QString strTools4 =
      QDir::homePath() + "/.ocat/devDatabase/DEBUG/EFI/OC/Tools/";

  dir.mkpath(strDrivers1);
  dir.mkpath(strDrivers2);
  dir.mkpath(strDrivers3);
  dir.mkpath(strDrivers4);
  dir.mkpath(strTools1);
  dir.mkpath(strTools2);
  dir.mkpath(strTools3);
  dir.mkpath(strTools4);

  QStringList listDrivers, listTools, listACPI;
  listDrivers = Method::DirToFileList(strDrivers0, "*.efi");
  listTools = Method::DirToFileList(strTools0, "*.efi");

  for (int i = 0; i < listDrivers.count(); i++) {
      FileOperation::copyFileToPath(strDrivers0 + listDrivers.at(i),
                                    strDrivers1 + listDrivers.at(i), false);
      FileOperation::copyFileToPath(strDrivers0 + listDrivers.at(i),
                                    strDrivers2 + listDrivers.at(i), false);
      FileOperation::copyFileToPath(strDrivers0 + listDrivers.at(i),
                                    strDrivers3 + listDrivers.at(i), false);
      FileOperation::copyFileToPath(strDrivers0 + listDrivers.at(i),
                                    strDrivers4 + listDrivers.at(i), false);
  }
  for (int i = 0; i < listTools.count(); i++) {
      FileOperation::copyFileToPath(strTools0 + listTools.at(i),
                                    strTools1 + listTools.at(i), false);
      FileOperation::copyFileToPath(strTools0 + listTools.at(i),
                                    strTools2 + listTools.at(i), false);
      FileOperation::copyFileToPath(strTools0 + listTools.at(i),
                                    strTools3 + listTools.at(i), false);
      FileOperation::copyFileToPath(strTools0 + listTools.at(i),
                                    strTools4 + listTools.at(i), false);
  }

  // ACPI Files
  QString strACPI0 = strAppExePath + "/Database/EFI/OC/ACPI/";
  QString strACPI1 = QDir::homePath() + "/.ocat/Database/EFI/OC/ACPI/";
  QString strACPI2 = QDir::homePath() + "/.ocat/devDatabase/EFI/OC/ACPI/";
  QString strACPI3 = QDir::homePath() + "/.ocat/Database/DEBUG/EFI/OC/ACPI/";
  QString strACPI4 = QDir::homePath() + "/.ocat/devDatabase/DEBUG/EFI/OC/ACPI/";
  dir.mkpath(strACPI1);
  dir.mkpath(strACPI2);
  dir.mkpath(strACPI3);
  dir.mkpath(strACPI4);
  listACPI = Method::DirToFileList(strACPI0, "*.aml");
  for (int i = 0; i < listACPI.count(); i++) {
      FileOperation::copyFileToPath(strACPI0 + listACPI.at(i), strACPI1 + listACPI.at(i),
                                    false);
      FileOperation::copyFileToPath(strACPI0 + listACPI.at(i), strACPI2 + listACPI.at(i),
                                    false);
      FileOperation::copyFileToPath(strACPI0 + listACPI.at(i), strACPI3 + listACPI.at(i),
                                    false);
      FileOperation::copyFileToPath(strACPI0 + listACPI.at(i), strACPI4 + listACPI.at(i),
                                    false);
  }

  QString fileSample =
      QDir::homePath() + "/.ocat/Database/BaseConfigs/SampleCustom.plist";
  if (!QFile(fileSample).exists()) {
    QMessageBox::critical(
        NULL, "",
        fileSample + "\n\n" +
            QObject::tr("The file does not exist, please check the "
                        "integrity of the app."));

    return 0;
  }

  QFont f;
#ifdef Q_OS_WIN32

  PlistFileName = QString::fromLocal8Bit(argv[1]);  //解决乱码

  f.setFamily("Microsoft YaHei UI");

#endif

#ifdef Q_OS_MAC

#endif

  if (!PlistFileName.isEmpty()) {
    a->new_win();
  } else {
    mw_one = new MainWindow();
    mw_one->show();
  }

  f.setPixelSize(12);
  a->setFont(f);
  return a->exec();
}

void loadLocal() {
    static QTranslator translator;
    static QTranslator translator1;

    QLocale locale;
    QLocale::Language type = locale.language();
    switch (type) {
    case QLocale::English:
        zh_cn = false;
        break;
    case QLocale::Chinese:
        if (translator.load(":/translations/cn.qm")) {
            qApp->installTranslator(&translator);
            zh_cn = true;
        }
        if (translator1.load(":/translations/qt_zh_CN.qm")) {
            qApp->installTranslator(&translator1);
            zh_cn = true;
        }
        break;
    default:
        break;
    }
}
