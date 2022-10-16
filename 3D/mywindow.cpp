#include "mywindow.h"
#include "ui_mywindow.h"
#include <QImage>
#include <QtAlgorithms>

MyWindow::MyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow)
{
    ui->setupUi(this);
    szer = ui->rysujFrame->width();
    wys = ui->rysujFrame->height();
    poczX = ui->rysujFrame->x();
    poczY = ui->rysujFrame->y();
    centerX=300;
    centerY=300;
    oryginal = new QImage(szer,wys,QImage::Format_RGB32);
    displayPicture = new QImage(szer,wys,QImage::Format_RGB32);
    *displayPicture = oryginal -> copy(0, 0, szer, wys);

    inicialize3D();
    resetApp();
}

MyWindow::~MyWindow()
{
    delete ui;
}

void MyWindow::on_exitButton_clicked()
{
    qApp->quit();
}

void MyWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.drawImage(poczX,poczY,*displayPicture);
}


void MyWindow::clean()
{
    unsigned char *ptr;
    ptr = displayPicture->bits();
    int i,j;
    for(i=0; i<wys; i++)
    {
        for(j=0; j<szer; j++)
        {
            ptr[szer*4*i + 4*j] = 255; // Skladowa BLUE
            ptr[szer*4*i + 4*j + 1] = 255; // Skladowa GREEN
            ptr[szer*4*i + 4*j + 2] = 255; // Skladowa RED
        }
    }
}

void MyWindow::paint(){
        float displayMatrix[4][4];
        clean();
        QVector<int> pointsX, pointsY, pointsZ;
        for(int i=0; i< 8; i++){

            for(int j = 0; j< 4; j++){
                for(int k =0; k < 4; k++){
                    if(k!=j) displayMatrix[j][k] =  0;
                    else displayMatrix[j][k] =  1;
                }
            }
            displayMatrix[0][0] = xv[i] - centerX;
            displayMatrix[1][1] = yv[i] - centerY;
            displayMatrix[2][2] = zv[i] - 50;
            displayMatrix[3][3] = 1;


            calculate(displayMatrix); //->liczy wszystkie macierze przeksztalcen

            //(matrix[0][0] + matrix[0][1] + matrix[0][2] + matrix[0][3] + centerX); -> x
            //(matrix[1][0] + matrix[1][1] + matrix[1][2] + matrix[1][3] + centerY); -> y
            //(matrix[2][0] + matrix[2][1] + matrix[2][2] + matrix[2][3] + 50);      -> z

            pointsX.push_back((matrix[0][0] + matrix[0][1] + matrix[0][2] + matrix[0][3] + centerX) - sqrt(2)*(matrix[2][0] + matrix[2][1] + matrix[2][2] + matrix[2][3] + 50)/2);
            pointsY.push_back((matrix[1][0] + matrix[1][1] + matrix[1][2] + matrix[1][3] + centerY) - sqrt(2)*(matrix[2][0] + matrix[2][1] + matrix[2][2] + matrix[2][3] + 50)/2);
            pointsZ.push_back((matrix[2][0] + matrix[2][1] + matrix[2][2] + matrix[2][3] + 50));
        }
        drawLine(pointsX[0], pointsX[1], pointsY[0], pointsY[1]);
        drawLine(pointsX[0], pointsX[3], pointsY[0], pointsY[3]);
        drawLine(pointsX[0], pointsX[4], pointsY[0], pointsY[4]);

        drawLine(pointsX[2], pointsX[1], pointsY[2], pointsY[1]);
        drawLine(pointsX[2], pointsX[3], pointsY[2], pointsY[3]);
        drawLine(pointsX[2], pointsX[6], pointsY[2], pointsY[6]);

        drawLine(pointsX[5], pointsX[1], pointsY[5], pointsY[1]);
        drawLine(pointsX[5], pointsX[4], pointsY[5], pointsY[4]);
        drawLine(pointsX[5], pointsX[6], pointsY[5], pointsY[6]);


        drawLine(pointsX[7], pointsX[3], pointsY[7], pointsY[3]);
        drawLine(pointsX[7], pointsX[4], pointsY[7], pointsY[4]);
        drawLine(pointsX[7], pointsX[6], pointsY[7], pointsY[6]);
    }

void MyWindow::displayM(float m[4][4]){
    for(int as = 0; as< 4; as++) qDebug() << m[as][0]<< m[as][1]<< m[as][2]<< m[as][3];
}

void MyWindow::multiplyMatrixByV(float v[]){
    float tempV[4];
    float temp;
    float temp2;
    float temp3;


    for(int i=0; i < 4; i++){
            tempV[i] = 0;
            for(int j=0; j<4; j++){
                temp2 = (float)matrix[i][j];
                temp3 = (float)v[j];
                temp = temp2 * temp3 * 1.0;
                tempV[i] += temp;
            }
    }

    for(int i=0; i<4; i++){
            v[i] = tempV[i];
    }
}
void MyWindow::multiplyMatrix(float additionalMatrix[4][4]){
  float tempMatrix[4][4];

  for(int i=0; i < 4; i++){
      for(int j=0; j < 4; j++){
          tempMatrix[i][j] = 0;
          for(int k=0; k<4; k++) tempMatrix[i][j] += (additionalMatrix[i][k]*matrix[k][j]);
      }
  }

  for(int i=0; i<4; i++){
      for(int j=0; j<4; j++){
          matrix[i][j] = tempMatrix[i][j];
      }
  }
}
void MyWindow::resetMatrix(){
    for(int i=0; i<4;i++){
        for(int j=0; j<4; j++){
            if(i==j){
                matrix[i][j] = 1.0;
            }
            else{
                matrix[i][j] = 0.0;
            }
        }
    }
}

void MyWindow::resetApp(){
    *displayPicture = oryginal -> copy(0, 0, szer, wys);

    ui->leftRigth->setValue(50);
    ui->upDown->setValue(50);
    ui->Zslider->setValue(50);
    ui->scale->setValue(25);
    ui->rotation->setValue(628);
    ui->rotationY->setValue(628);
    ui->rotationZ->setValue(628);
    ui->shX->setValue(100);
    ui->shY->setValue(100);

    tX = 0;
    tY = 0;
    tZ = 0;
    s = 0;
    r = 0;
    shX = 0;
    shY = 0;

    for(int i=0; i<4;i++){
        for(int j=0; j<4; j++){
            if(i==j){
                     matrix[i][j] = 1.0;
                     tXMatrix[i][j] = 1.0;
                     tYMatrix[i][j] = 1.0;
                     tZMatrix[i][j] = 1.0;
                     sMatrix[i][j] = 1.0;
                     rXMatrix[i][j] = 1.0;
                     rYMatrix[i][j] = 1.0;
                     rZMatrix[i][j] = 1.0;
                     shXMatrix[i][j] = 1.0;
                     shYMatrix[i][j] = 1.0;

            }
            else{
                     matrix[i][j] = 0.0;
                     tXMatrix[i][j] = 0.0;
                     tYMatrix[i][j] = 0.0;
                     tZMatrix[i][j] = 0.0;
                     sMatrix[i][j] = 0.0;
                     rXMatrix[i][j] = 0.0;
                     rYMatrix[i][j] = 0.0;
                     rZMatrix[i][j] = 0.0;
                     shXMatrix[i][j] = 0.0;
                     shYMatrix[i][j] = 0.0;
            }
        }
    }
    paint();
}
void MyWindow::inicialize3D(){
    //          0    1    2    3    4    5    6    7
    //trapezX 250, 350, 400, 200, 250, 350, 400, 200,
    //trapezY 250, 250, 350, 350, 250, 250, 350, 350,
    //trapezZ 0, 0, 0, 0, 100, 100, 100, 100;

    for(int i = 0; i < 8; i++){
        if(i == 0 || i == 4) xv[i] = 250;
        else if(i == 1 || i == 5) xv[i] = 350;
        else if(i == 2 || i == 6) xv[i] = 400;
        else xv[i] = 200;
        if(i == 0 || i == 1 || i == 4 || i == 5) yv[i] = 250;
        else yv[i] = 350;

        if(i < 4) zv[i] = 0;
        else zv[i] = 100;

    }
}

void MyWindow::calculate(float m[4][4]){
    resetMatrix();

    multiplyMatrix(m);
    multiplyMatrix(rXMatrix);
    multiplyMatrix(shXMatrix);
    multiplyMatrix(rYMatrix);
    multiplyMatrix(shYMatrix);
    multiplyMatrix(rZMatrix);
    multiplyMatrix(sMatrix);

    multiplyMatrix(tXMatrix);
    multiplyMatrix(tYMatrix);
    multiplyMatrix(tZMatrix);

}

void MyWindow::drawLine(int x0, int x1, int y0, int y1)
{
    int x, pom;
    float a, y;
    a = (float)(y1 - y0)/(float)(x1 - x0);

    if(a < 1 && a > -1){
        if(x1 < x0){
            pom = x0;
            x0 = x1;
            x1 = pom;
            y0 = y1;
        }
        y = y0;
        for(x = x0; x <= x1; x++){
            lightPixel(x, (int)round(y+0.5));
            y = y + a;
        }
    }else{

       pom = x0;
       x0 = y0;
       y0 = pom;
       pom = x1;
       x1 = y1;
       y1 = pom;

       if(x1 < x0){
           pom = x0;
           x0 = x1;
           x1 = pom;

           pom = y0;
           y0 = y1;
           y1 = pom;
       }

       a = (float)(y1 - y0)/(float)(x1 - x0);
       y = y0;
       for(x = x0; x <= x1; x++){
           lightPixel((int)round(y+0.5), x);
           y = y + a;
       }
    }
}

void MyWindow::lightPixel(int x, int y){
    unsigned char *prt;
    prt = displayPicture ->bits();

   if ((x>=0)&&(y>=0)&&(x<szer)&&(y<wys)){
       prt[szer*4*y + 4*x] = 0;
       prt[szer*4*y + 4*x + 1] = 0;
       prt[szer*4*y + 4*x + 2] = 0;
   }
   update();
}

void MyWindow::on_exitButton_2_clicked()
{
    resetApp();
    update();
}

void MyWindow::on_leftRigth_sliderMoved(int value)
{
    tX = (value-50)*-4;
    tXMatrix[0][3] = -tX;
    paint();
    update();
}
void MyWindow::on_upDown_sliderMoved(int value)
{
    tY = (value-50)*-4;
    tYMatrix[1][3] = -tY;
    paint();
    update();
}
void MyWindow::on_Zslider_sliderMoved(int value)
{
    tZ = (value-50)*-6;
    tZMatrix[2][3] = -tZ;

    paint();
    update();
}

void MyWindow::on_scale_sliderMoved(int value)
{

    s=(value-25)*(-15);
    s = 1 + 0.1*s*(-1.0)/65;
    sMatrix[0][0] = s;
    sMatrix[1][1] = s;
    sMatrix[2][2] = s;
    paint();
    update();
}



void MyWindow::on_rotation_sliderMoved(int value)
{
    //Pi = 3.14, jeden cały obrót to 2*PI czyli w przyblizeniu 6.28
    r = float((value-628)/100.0);

    rXMatrix[1][1] = cos(r);
    rXMatrix[2][2] = cos(r);
    rXMatrix[1][2] = sin(r);
    rXMatrix[2][1] = -sin(r);
    paint();
    update();
}


void MyWindow::on_shX_sliderMoved(int value)
{
    shX = (value-100)/80.0;
    shXMatrix[0][2] = shX;
    paint();
    update();
}


void MyWindow::on_shY_sliderMoved(int value)
{
    shY = (value-100)/80.0;
    shXMatrix[1][2] = shY;
    paint();
    update();
}


void MyWindow::on_rotationY_sliderMoved(int value)
{
    float ry;
    ry = float((value-628)/100.0);
    rYMatrix[0][0] = cos(ry);
    rYMatrix[2][2] = cos(ry);
    rYMatrix[2][0] = sin(ry);
    rYMatrix[0][2] = -sin(ry);

    paint();
    update();
}


void MyWindow::on_rotationZ_sliderMoved(int value)
{
    float rz;
    rz = float((value-628)/100.0);

    rZMatrix[0][0] = cos(rz);
    rZMatrix[1][1] = cos(rz);
    rZMatrix[0][1] = sin(rz);
    rZMatrix[1][0] = -sin(rz);

    paint();
    update();
}

