#ifndef MovieSelector_H
#define MovieSelector_H

#include <QDialog>
#include <QMap>
#include <QMapIterator>

namespace Ui {
class MovieSelector;
}

class MovieSelector : public QDialog
{
    Q_OBJECT
    
public:
    explicit MovieSelector(QWidget *parent = 0);
    ~MovieSelector();
    void setMovieData(QString searchString,QVector<QMap<QString,QString> > );
private slots:
    void on_movieOptionsCombo_currentIndexChanged(int index);
    void updateDateRelated2Movie();
private:
    Ui::MovieSelector *ui;
    int selectedIndex;
    QVector<QMap<QString,QString> > titles;
};

#endif // MovieSelector_H
