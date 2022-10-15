#ifndef MYWINDOW_H
#define MYWINDOW_H
#include <QMainWindow>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>
#include <QVector>

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

    QImage *pictures[5];
    QImage *displayPicture;
    int szer;
    int wys;
    int poczX;
    int poczY;
    QVector<int> img;
    float a[5];//alpha
    int b[5];//type blend

    //void clear();
    //void showPicture(int);
    //void choose();
    void blendPicture();

private slots:
    void on_cleanButton_clicked();
    void on_exitButton_clicked();
    void paintEvent(QPaintEvent*);

    void on_Rysunek1CheckBox_clicked(bool checked);
    void on_Rysunek2CheckBox_clicked(bool checked);
    void on_Rysunek3CheckBox_clicked(bool checked);
    void on_Rysunek4CheckBox_clicked(bool checked);
    void on_Rysunek5CheckBox_clicked(bool checked);

    void on_Rysunek1WyborBlendu_activated(int index);
    void on_Rysunek2WyborBlendu_activated(int index);
    void on_Rysunek3WyborBlendu_activated(int index);
    void on_Rysunek4WyborBlendu_activated(int index);
    void on_Rysunek5WyborBlendu_activated(int index);

    void on_Rysunek1Slider_sliderMoved(int position);
    void on_Rysunek2Slider_sliderMoved(int position);
    void on_Rysunek3Slider_sliderMoved(int position);
    void on_Rysunek4Slider_sliderMoved(int position);
    void on_Rysunek5Slider_sliderMoved(int position);
};

#endif
