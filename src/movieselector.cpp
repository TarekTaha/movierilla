#include "movieselector.h"
#include "ui_movieselector.h"

MovieSelector::MovieSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MovieSelector)
{
    ui->setupUi(this);
    selectedIndex = 0;
}

MovieSelector::~MovieSelector()
{
    delete ui;
}

void MovieSelector::setMovieData(QString searchString, QVector<QMap<QString,QString> >  _titles)
{
    ui->movieTitle->setText(searchString);
    ui->movieOptionsCombo->clear();
    for(int i=0;i<_titles.size();i++)
    {
        ui->movieOptionsCombo->addItem(_titles[i]["Title"]);
    }
    ui->movieOptionsCombo->setCurrentIndex(0);
    titles = _titles;
    updateDateRelated2Movie();
}

void MovieSelector::on_movieOptionsCombo_currentIndexChanged(int index)
{
    selectedIndex = index;
    updateDateRelated2Movie();
}

void MovieSelector::updateDateRelated2Movie()
{
    if(titles.size()>0)
    {
        ui->movieData->clear();
        QMap<QString, QString>::const_iterator i = titles[selectedIndex].constBegin();
        while (i != titles[selectedIndex].constEnd())
        {
            ui->movieData->insertPlainText(QString("%1:%2\n").arg(i.key()).arg(i.value()));
            ++i;
        }
    }
}
