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
    QImage *oryginal;

    float matrix[4][4];
    float tXMatrix[4][4];
    float tYMatrix[4][4];
    float tZMatrix[4][4];
    float sMatrix[4][4];
    float rXMatrix[4][4];
    float rYMatrix[4][4];
    float rZMatrix[4][4];
    float shXMatrix[4][4];
    float shYMatrix[4][4];

    int szer;
    int wys;
    int poczX;
    int poczY;
    int centerX;
    int centerY;
    int d;
    int xv[8];
    int yv[8];
    int zv[8];
    int tX;//translation X
    int tY;//translation Y
    int tZ;//translation Z
    float s;//scale
    float r;//rotation
    float shX;//pochylenieX
    float shY;//pochylenieY
    int ind;

    bool visible;

        void clean();
        void paint();
        void resetMatrix();
        void multiplyMatrixByV(float [4]);
        void multiplyMatrix(float [4][4]);
        void displayM(float [4][4]);
        void calculate(float [4][4]);
        void resetApp();
        void inicialize3D();
        void drawLine(int, int, int, int);
        void lightPixel(int, int);
        //void printWall(int, float, QVector<int>, QVector<int>);
        float lengthVector(float, float, float, float);
        bool checkAngles(QVector<float>, QVector<float>, int, int [8][3]);



private slots:
    void on_exitButton_clicked();
    void paintEvent(QPaintEvent*);
    void on_exitButton_2_clicked();
    void on_upDown_sliderMoved(int);
    void on_leftRigth_sliderMoved(int);
    void on_scale_sliderMoved(int);
    void on_rotation_sliderMoved(int);
    void on_shX_sliderMoved(int);
    void on_shY_sliderMoved(int);
    void on_Zslider_sliderMoved(int position);
    void on_rotationY_sliderMoved(int position);
    void on_rotationZ_sliderMoved(int position);
    void on_checkBox_stateChanged(int arg1);
};

#endif
