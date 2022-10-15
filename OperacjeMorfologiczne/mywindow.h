#ifndef MYWINDOW_H
#define MYWINDOW_H
#include <QMainWindow>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>

namespace Ui {
    class MyWindow;
}

class MyWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyWindow(QWidget *parent = 0);
    ~MyWindow();

private:
    Ui::MyWindow *ui;
    QImage *displayPicture;
    QImage *copyImage;
    int szer;
    int wys;
    int poczX;
    int poczY;
    int mask;
    QString fileName;

    void clean();
    void conversionWhiteBlack(QImage*, int);
    void addImage(QString);
    void dilatation();
    void erosion();


private slots:
    void on_cleanButton_clicked();
    void on_exitButton_clicked();
    void paintEvent(QPaintEvent*);
    void on_AddButton_clicked();
    void on_ResetButton_clicked();
    void on_spinBox_valueChanged(int);
    void on_openButton_clicked();
    void on_closeButton_clicked();
};

#endif
