#include "mywindow.h"
#include "ui_mywindow.h"
#include <QStack>
#include <QImage>
#include <QVector>
#include <QTimer>


MyWindow::MyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow)
{
    ui->setupUi(this);
    szer = ui->picturesFrame->width();
    wys = ui->picturesFrame->height();

    poczX0 = ui->picturesFrame->x();
    poczY0 = ui->picturesFrame->y();

    poczX1 = ui->morphFrame->x();
    poczY1 = ui->morphFrame->y();

    photo1 = new QImage(":/Images/picture1.jpg");
    photo2 = new QImage(":/Images/picture2.jpg");

    displayPicture = new QImage(szer,wys,QImage::Format_RGB32);
    *displayPicture = photo1 -> copy(0, 0, szer, wys);

    morph = new QImage(szer,wys,QImage::Format_RGB32);

    choose = 0;
    upgradeMask = 2;
    N = 20;
    mesh();
    drawMesh();
}

//zrobic kropki po przycisnieciu, 10px, przesuwanie ich(z baizera)

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
    p.drawImage(poczX0, poczY0, *displayPicture);
    p.drawImage(poczX1, poczY1, *morph);
}


void MyWindow::clear(int p)
{
    if(p ==0) *displayPicture = photo1 -> copy(0, 0, szer, wys);
    else if(p == 1) *displayPicture = photo2 -> copy(0, 0, szer, wys);
    else {
        unsigned char *ptr;
        ptr = morph->bits();
        int i,j;
        for(i=0; i<wys; i++)
        {
            for(j=0; j<szer; j++)
            {
              ptr[szer*4*i + 4*j]=0; // Skladowa BLUE
               ptr[szer*4*i + 4*j + 1] = 0; // Skladowa GREEN
              ptr[szer*4*i + 4*j + 2] = 0; // Skladowa RED
            }
        }
    }
}

void MyWindow::zapal_pixel(int x, int y){
    unsigned char *prt;
    prt = displayPicture ->bits();

   if ((x>=0)&&(y>=0)&&(x<szer)&&(y<wys)){
       prt[szer*4*y + 4*x] = 0;//68+color;
       prt[szer*4*y + 4*x + 1] = 0;
       prt[szer*4*y + 4*x + 2] = 255;//color;
   }
   update();
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
            zapal_pixel(x, (int)round(y+0.5));
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
           zapal_pixel((int)round(y+0.5), x);
           y = y + a;
       }
    }
    update();
}

void MyWindow::mousePressEvent(QMouseEvent *event)
{
    x0 = event->position().x();
    y0 = event->position().y();

    x0 -= poczX0;
    y0 -= poczY0;
    qDebug()<<x0<<" "<<y0;
}

void MyWindow::mouseReleaseEvent(QMouseEvent *event){
    x1 = event->position().x();
    y1 = event->position().y();

    x1 -= poczX0;
    y1 -= poczY0;

    movePoint(choose);
}

void MyWindow::movePoint(int p){
    for(int i=0; i < xv[p].size(); i++){
        if(x0-10 <= xv[p][i] && x0+10 >= xv[p][i] &&  y0-10 <= yv[p][i] && y0+10 >= yv[p][i]){

            xv[p][i] = x1;
            yv[p][i] = y1;
            clear(p);
            drawMesh();
        }
    }
}

bool MyWindow::checkInside(int x0, int y0, int x1, int y1, int x2, int y2, float xt, float yt){

    float T;
    T = triangle(x0, y0, x1, y1, x2, y2);
    float T1 = triangle(xt, yt, x1, y1, x2, y2);
    float T2 = triangle(x0, y0, xt, yt, x2, y2);
    float T3 = triangle(x0, y0, x1, y1, xt, yt);
    return (T == T1+T2+T3);
}

float MyWindow::triangle(int x0, int y0, int x1, int y1, int x2, int y2){
    float pom = 0.5*qAbs(x0*(y1 - y2)*1.0 + x1*(y2 - y0)*1.0 + x2*(y0 - y1)*1.0);
    return pom;
}

void MyWindow::minMax(int& min, int& max, int v[]){
    min = v[0];
    max = v[2];
    for(int i=0; i < 3; i++){
        if(min > v[i]) min = v[i];
        if(max < v[i]) max = v[i];
    }
}

void MyWindow::mark(int x0, int y0){
    for(int i= y0 - 4; i<y0 + 4; i++){
        for(int j = x0 - 4; j< x0 + 4; j++){
        zapal_pixel(j, i);
        }
   }

}

void MyWindow::on_imageButton1_clicked()
{
    choose = 0;
    //photo1 = new QImage(":/Images/picture1.jpg");
    *displayPicture = photo1 -> copy(0, 0, szer, wys);
    drawMesh();
    update();
}


void MyWindow::on_imageButton2_clicked()
{
    choose = 1;
    //photo2 = new QImage(":/Images/picture2.jpg");
    *displayPicture = photo2 -> copy(0, 0, szer, wys);
    drawMesh();
    update();
}


void MyWindow::on_pushButton_clicked()
{
   qDebug() << xv[0].size();
   morphing();
}

void MyWindow::morphing(){
    int abcX[3];//nowe wierzcholki X trojkata przechodniego
    int abcY[3];//nowe wierzcholki Y trojkata przechodniego

    int d = 0;
    if(upgradeMask == 0) d -= 4;
    for(int n = 0; n <= N; n++){
        clear(2);
        for(int z = 0; z < 25; z += 5){//25
            for(int x = 0; x < 4; x++){//4

                //gorny trojkat
                abcX[0] = (1 - n*1.0/N) * 1.0 * xv[0][z+x] + (n*1.0/N) * 1.0 * xv[1][z+x];//Ax
                abcY[0] = (1 - n*1.0/N) * 1.0 * yv[0][z+x] + (n*1.0/N) * 1.0 * yv[1][z+x];//Ay
                abcX[1] = (1 - n*1.0/N) * 1.0 * xv[0][5+z+x+d] + (n*1.0/N)  * 1.0 * xv[1][5+z+x+d];//Bx
                abcY[1] = (1 - n*1.0/N) * 1.0 * yv[0][5+z+x+d] + (n*1.0/N)  * 1.0 * yv[1][5+z+x+d];//By
                abcX[2] = (1 - n*1.0/N) * 1.0 * xv[0][6+z+x+d] + (n*1.0/N)  * 1.0 * xv[1][6+z+x+d];//Cx
                abcY[2] = (1 - n*1.0/N) * 1.0 * yv[0][6+z+x+d] + (n*1.0/N)  * 1.0 * yv[1][6+z+x+d];//Cy
                texturing(abcX, abcY, n, z, x, d, 0);
                if(upgradeMask == 0){z =100; x=100;}
                else{
                    abcX[0] = (1 - n*1.0/N) * 1.0 * xv[0][z+x] + (n*1.0/N) * 1.0 * xv[1][z+x];//Ax
                    abcY[0] = (1 - n*1.0/N) * 1.0 * yv[0][z+x] + (n*1.0/N) * 1.0 * yv[1][z+x];//Ay
                    abcX[1] = (1 - n*1.0/N) * 1.0 * xv[0][1+z+x] + (n*1.0/N)  * 1.0 * xv[1][1+z+x];//Bx
                    abcY[1] = (1 - n*1.0/N) * 1.0 * yv[0][1+z+x] + (n*1.0/N)  * 1.0 * yv[1][1+z+x];//By
                    abcX[2] = (1 - n*1.0/N) * 1.0 * xv[0][6+z+x+d] + (n*1.0/N)  * 1.0 * xv[1][6+z+x+d];//Cx
                    abcY[2] = (1 - n*1.0/N) * 1.0 * yv[0][6+z+x+d] + (n*1.0/N)  * 1.0 * yv[1][6+z+x+d];//Cy
                    texturing(abcX, abcY, n, z, x, d, -4);
                }
                update();
            }
        }
        delay(1);
    }

}
void MyWindow::texturing(int abcX[3], int abcY[3], int n, int z, int x, int d, int s){
    unsigned char *img1, *img2, *tx;
    img1 = photo1 -> bits();
    img2 = photo2 -> bits();

    tx = morph ->bits();
    int minX=0, maxX=0, minY=0, maxY=0;
    minMax(minX, maxX, abcX);
    minMax(minY, maxY, abcY);
    int xt[2], yt[2];
    float w0, wV,wW;
    float v, w, u;
    float red=0,green=0,blue=0;
    for(int i = minX; i <= maxX; i++){
            for(int j = minY; j <= maxY; j++){
                 if(checkInside(abcX[0], abcY[0], abcX[1], abcY[1], abcX[2], abcY[2], i, j)){
                     w0 = (abcX[1]  - abcX[0])  * (abcY[2]  - abcY[0])  - (abcY[1]  - abcY[0])  * (abcX[2]  - abcX[0]);
                     wV = (i        - abcX[0])  * (abcY[2]  - abcY[0])  - (j        - abcY[0])  * (abcX[2]  - abcX[0]);
                     wW = (abcX[1]  - abcX[0])  * (j        - abcY[0])  - (abcY[1]  - abcY[0])  * (i        - abcX[0]);
                     v = wV*1.0/w0;
                     w = wW*1.0/w0;
                     u = 1 - v - w;


                     xt[0]=u*xv[0][z+x]*1.0 + v*xv[0][5+z+x+d+s]*1.0 + w*xv[0][6+z+x+d]*1.0;
                     yt[0]=u*yv[0][z+x]*1.0 + v*yv[0][5+z+x+d+s]*1.0 + w*yv[0][6+z+x+d]*1.0;
                     xt[1]=u*xv[1][z+x]*1.0 + v*xv[1][5+z+x+d+s]*1.0 + w*xv[1][6+z+x+d]*1.0;
                     yt[1]=u*yv[1][z+x]*1.0 + v*yv[1][5+z+x+d+s]*1.0 + w*yv[1][6+z+x+d]*1.0;


                     blue = (1 - n*1.0/N) * img1[szer*4*(int)yt[0] + 4*(int)xt[0]] + (n*1.0/N) * img2[szer*4*(int)yt[1] + 4*(int)xt[1]];
                     green = (1 - n*1.0/N) * img1[szer*4*(int)yt[0] + 4*(int)xt[0] + 1] + (n*1.0/N) * img2[szer*4*(int)yt[1] + 4*(int)xt[1] + 1];
                     red = (1 - n*1.0/N) * img1[szer*4*(int)yt[0] + 4*(int)xt[0] + 2]  + (n*1.0/N) * img2[szer*4*(int)yt[1] + 4*(int)xt[1] + 2];

                     tx[szer*4*j + 4*i]     = round(blue);
                     tx[szer*4*j + 4*i + 1] = round(green);
                     tx[szer*4*j + 4*i + 2] = round(red);
                 }
            }
    }
}

inline void MyWindow::delay(int millisecondsWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
}

void MyWindow::mesh(){
    if(upgradeMask != 0){
        for(int i=0; i<wys; i++){
            for(int j=0; j< szer; j++){
               //if(j%120 == 0 || i%120 == 0) zapal_pixel(i, j);//linie do usuniecia
             if((i%120 == 0 && j%150 == 0) || (i % 120 ==0 && j ==599) || (i == 599 && j%150==0) || (i == 599 && j == 599))
             {
                xv[1].push_back(j);
                yv[1].push_back(i);
                xv[0].push_back(j);
                yv[0].push_back(i);
            }
        }
      }

        xv[0][6] = 150;
        xv[0][7] = 300;
        xv[0][8] = 450;
        xv[0][12] = 275;
        xv[0][13] = 430;
        xv[0][16] = 130;
        xv[0][17] = 260;
        xv[0][18] = 525;
        xv[0][21] = 150;
        xv[0][22] = 280;
        xv[0][23] = 500;
        //xv[0][26] = 200;
        //xv[0][28] = 435;
        yv[0][6] = 60;
        yv[0][7] = 50;
        yv[0][8] = 60;
        yv[0][12] = 250;
        yv[0][13] = 250;
        yv[0][16] = 355;
        yv[0][17] = 360;
        yv[0][18] = 360;
        yv[0][21] = 455;
        yv[0][22] = 435;
        yv[0][23] = 460;
        //yv[0][26] = 545;
        //yv[0][28] = 545;

        xv[1][6] = 114;
        xv[1][7] = 218;
        xv[1][8] = 340;
        xv[1][11] = 150;
        xv[1][12] = 240;
        xv[1][13] = 350;
        xv[1][16] = 130;
        xv[1][17] = 230;
        xv[1][22] = 250;
        xv[1][23] = 425;
        //xv[1][26] = 190;
        //xv[1][27] = 270;
        //xv[1][28] = 370;

        yv[1][6] = 175;
        yv[1][7] = 130;
        yv[1][8] = 130;
        yv[1][11] = 320;
        yv[1][12] = 310;
        yv[1][13] = 290;
        yv[1][16] = 400;
        yv[1][17] = 380;
        yv[1][22] = 450;
        yv[1][23] = 465;
        //yv[1][26] = 545;
        //yv[1][27] = 575;
        //yv[1][28] = 535;

    }
    else{
        xv[0].push_back(200);//Ax
        yv[0].push_back(200);//Ay
        xv[0].push_back(400);//Bx
        yv[0].push_back(400);//By
        xv[0].push_back(300);//Cx
        yv[0].push_back(200);//Cy

        xv[1].push_back(200);//Ax
        yv[1].push_back(400);//Ay
        xv[1].push_back(400);//Bx
        yv[1].push_back(200);//By
        xv[1].push_back(300);//Cx
        yv[1].push_back(400);//Cy
    }
}
void MyWindow::drawMesh(){
    int k = choose;
    if(upgradeMask != 0){
        int pom = 4;
        int i = 0;
        for(i = 0; i < xv[k].size(); i++){
            mark(xv[k][i], yv[k][i]);//zaznaczenie puktow
            if(i != pom) drawLine(xv[k][i], xv[k][i+1], yv[k][i], yv[k][i+1]);//z lewej do prawej odcinek
            if(i != pom && i < 25) {
             drawLine(xv[k][i], xv[k][i+6], yv[k][i], yv[k][i+6]); //skosny odcinek
             drawLine(xv[k][i], xv[k][i+5], yv[k][i], yv[k][i+5]);  // z gory na dol odcinek
            }
            if(i == pom && i < 25) {drawLine(xv[k][i]-1, xv[k][i+5]-1, yv[k][i], yv[k][i+5]); pom += 5;}//prawy brzeg
        }
    }
    else{
        for(int i=0; i<3; i++){
            mark(xv[k][i], yv[k][i]);
        }
        for(int i=0; i<2; i++){
            drawLine(xv[k][i], xv[k][i+1], yv[k][i], yv[k][i+1]);
        }
        drawLine(xv[k][0], xv[k][2], yv[k][0], yv[k][2]);
    }
}


void MyWindow::on_checkBox_stateChanged(int arg1)
{
    qDebug() << xv[0].size()<< " "<<xv[1].size();
    upgradeMask = arg1;
    xv[0].clear();
    yv[0].clear();
    xv[1].clear();
    yv[1].clear();
    clear(choose);
    mesh();
    drawMesh();
    update();
    qDebug() << xv[0].size()<< " "<<xv[1].size();
}

