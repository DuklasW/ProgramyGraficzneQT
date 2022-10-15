#ifndef MYWINDOW_H
#define MYWINDOW_H
#include <QMainWindow>
#include <QPainter>
#include <QImage>
#include <QStack>
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
    QImage *photo1;
    QImage *photo2;
    QImage *morph;//texturing
    QImage *displayPicture;

    int szer;
    int wys;
    int poczX0;
    int poczY0;
    int poczX1;
    int poczY1;
    int x0, y0;
    int x1, y1;
    int choose;
    int upgradeMask;//upgdateMask
    int N;
    QVector<int> xv[2];
    QVector<int> yv[2];

    void clear(int);
    void drawLine(int, int, int, int);
    void zapal_pixel(int, int);
    void swapXY(int, int, int, int);
    void mark(int, int);
    //void paint(int, bool);
    void texturing(int [], int[], int, int, int, int, int);
    void minMax(int&, int&, int[]);
    bool checkInside(int, int, int, int, int, int, float, float);
    float triangle(int, int, int, int, int, int);
    void movePoint(int);
    void mesh();
    void drawMesh();
    void morphing();
    void delay(int);

private slots:
    void on_exitButton_clicked();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent*);
    void on_imageButton1_clicked();
    void on_imageButton2_clicked();
    void on_pushButton_clicked();
    void on_checkBox_stateChanged(int arg1);
};

#endif
