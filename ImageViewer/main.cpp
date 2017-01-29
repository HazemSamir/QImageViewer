#include "mainwindow.h"
#include <QApplication>

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QtAndroid>
#endif

QString loadAndroidFile();

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
#ifdef Q_OS_ANDROID
    w.display(loadAndroidFile());
#else
    if (argc > 1)
        w.display(argv[1]);
#endif
    return a.exec();
}

QString loadAndroidFile()
{
#ifdef Q_OS_ANDROID
    QAndroidJniObject activity = QtAndroid::androidActivity();
    if (activity.isValid()) {
        QAndroidJniObject intent = activity.callObjectMethod("getIntent", "()Landroid/content/Intent;");
        if (intent.isValid()) {
            QAndroidJniObject data = intent.callObjectMethod("getData", "()Landroid/net/Uri;");
            if (data.isValid()) {
                QAndroidJniObject path = data.callObjectMethod("getPath", "()Ljava/lang/String;");
                if (path.isValid())
                    return path.toString();
            }
        }
    }
#endif
    return "";
}
