#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textOutput->ensureCursorVisible();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_generateNfoFiles_released()
{

    QDir rootDir;
    QStringList filters;
    rootDir.setCurrent("/home/yazhuo/Videos/Movies/");
    rootDir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    rootDir.setSorting(QDir::Size | QDir::Reversed);
    filters<<"*.ignore";
    rootDir.setNameFilters(filters);
    QFileInfoList list = rootDir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo  = list.at(i);
        QString movieName   = fileInfo.fileName().replace(".ignore","").replace("."," ");
        QString nfoFileName = fileInfo.fileName().replace(".ignore",".nfo");
        ui->textOutput->insertPlainText(QString("%1\n").arg(movieName));

        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QNetworkRequest request;
        QString searchString ;
        if(ui->searchType->currentText().contains("Title"))
            searchString = QString("http://www.omdbapi.com/?t=%1").arg(movieName);
        else
            searchString = QString("http://www.omdbapi.com/?s=%1").arg(movieName);
        ui->searchString->setText(searchString);
        request.setUrl(QUrl(searchString));
        request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");

        QNetworkReply *reply = manager->get(request);
        QEventLoop loop;
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();

        QString result   = reply->readAll();
        QStringList data = result.split("},{");
        QVector<QMap<QString,QString> > map = stripData(data);
        if(data.size()>1)
        {
            movieSelector.setMovieData(movieName,map);
            QEventLoop waitForSelection;
            connect(&movieSelector, SIGNAL(finished(int)),&waitForSelection,SLOT(quit()));
            movieSelector.show();
            waitForSelection.exec();
            ui->textOutput->insertPlainText(QString("Waiting for user to Select Movie"));
            if(movieSelector.result()==QDialog::Rejected)
            {
                break;
            }
        }
        /*
        for(int i=0;i<data.size();i++)
        {
            data[i].replace("\"Search\":[","");
            data[i].replace("{","");
            data[i].replace("}","");
            QStringList enties = data[i].split("\",\"");
            bool movieInfoFound= false;
            for(int j=0;j<enties.size();j++)
            {
                QString attributes   = enties[j].replace("\"","");
                QStringList elements = attributes.split(":");
                if(elements.size()!=2)
                    continue;
                if(elements[0].contains("Response") && elements[1].contains("True"))
                {
                    qDebug()<<"Movie Info Found";
                    movieInfoFound = true;
                }
                ui->textOutput->insertPlainText(QString("Attribute:%1  Value:%2\n").arg(elements[0]).arg(elements[1]));
            }
            if(movieInfoFound)
            {
                writeNfo2File(nfoFileName,enties);
            }
        }
        */
        ui->textOutput->insertPlainText("\n\n");
        ui->textOutput->ensureCursorVisible();
    }
}

QVector<QMap<QString,QString> > MainWindow::stripData(QStringList data)
{
    QVector<QMap<QString,QString> > dataMap;
    for(int i=0;i<data.size();i++)
    {
        data[i].replace("\"Search\":[","");
        data[i].replace("{","");
        data[i].replace("}","");
        QStringList enties = data[i].split("\",\"");
        QMap<QString,QString> map;
        for(int j=0;j<enties.size();j++)
        {
            QString attributes   = enties[j].replace("\"","");
            QStringList elements = attributes.split(":");
            if(elements.size()!=2)
                continue;
            if(elements[0].contains("Response") && elements[1].contains("True"))
            {
                qDebug()<<"Movie Info Found";
            }
            ui->textOutput->insertPlainText(QString("Attribute:%1  Value:%2\n").arg(elements[0]).arg(elements[1]));
            map.insert(elements[0],elements[1]);
        }
        dataMap.push_back(map);
    }
    return dataMap;
}

void MainWindow::writeNfo2File(QString fileName,QStringList data)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"Can't Write to file:"<<fileName;
        return;
    }
    qDebug()<<"Writing Data to file:"<<fileName;
    QTextStream stream;
    stream.setDevice(&file);
    stream<<"<?xml version=\"1.0\" ?>\n";
    stream<<"   <movie>\n";
    for(int j=0;j<data.size();j++)
    {
        QString attributes   = data[j].replace("\"","");
        QStringList elements = attributes.split(":");
        if(elements.size()!=2)
            continue;
        if(elements[0].contains("Title"))
            stream<<"       <title>"<<elements[1]<<"</title>\n";
        else if(elements[0].contains("imdbRating"))
            stream<<"       <rating>"<<elements[1]<<"</rating>\n";
        else if(elements[0].contains("imdbID"))
            stream<<"       <id>"<<elements[1]<<"</id>\n";
        else if(elements[0].contains("Year"))
            stream<<"       <Year>"<<elements[1]<<"</year>\n";
        else if(elements[0].contains("Runtime"))
            stream<<"       <runtime>"<<elements[1]<<"</runtime>\n";
        else if(elements[0].contains("Plot"))
            stream<<"       <plot>"<<elements[1]<<"</plot>\n";
        else if(elements[0].contains("imdbVotes"))
            stream<<"       <votes>"<<elements[1]<<"</votes>\n";
        else if(elements[0].contains("Genre"))
        {
            QStringList genres = elements[1].split(",");
            qDebug()<<"Genres Are:"<<elements[1]<<" After Split:"<<genres;
            for(int i=0;i<genres.size();i++)
                stream<<"       <genre>"<<genres[i].trimmed()<<"</genre>\n";
        }
        else if(elements[0].contains("Actors"))
        {
            QStringList actors = elements[1].split(",");
            for(int i=0;i<actors.size();i++)
            {
                stream<<"       <actor>\n";
                stream<<"           <name>"<<actors[i].trimmed()<<"</name>\n";
                stream<<"       </actor>\n";
            }
        }
        else if(elements[0].contains("Writer"))
        {
            QStringList writers = elements[1].split(",");
            for(int i=0;i<writers.size();i++)
                stream<<"       <credits>"<<writers[i].trimmed()<<"</credits>\n";
        }
        else if(elements[0].contains("Director"))
            stream<<"       <director>"<<elements[1]<<"</director>\n";
    }
    stream<<"   </movie>\n";
}
