#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QDir>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QTextStream>
#include <QUrl>
#include <QDebug>
#include <QMap>
#include <movieselector.h>

class QNetworkReply;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_generateNfoFiles_released();

private:
    Ui::MainWindow *ui;
    void writeNfo2File(QString fileName,QStringList);
    QVector<QMap<QString,QString> > stripData(QStringList data);
    MovieSelector movieSelector;
};

#endif // MAINWINDOW_H
