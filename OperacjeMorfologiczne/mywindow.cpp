#include "mywindow.h"
#include "ui_mywindow.h"
#include <QFileDialog>
MyWindow::MyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow)
{
    ui->setupUi(this);
    szer = ui->rysujFrame->width();
    wys = ui->rysujFrame->height();
    poczX = ui->rysujFrame->x();
    poczY = ui->rysujFrame->y();
    fileName = "";
    displayPicture = new QImage(szer,wys,QImage::Format_RGB32);
    copyImage = new QImage(szer,wys,QImage::Format_RGB32);


    conversionWhiteBlack(displayPicture, 140);
    mask = 3;
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

void MyWindow::on_cleanButton_clicked() //dodac przycisk do czyszczenia obrazka, ten resetuje go do stanu poczatkowego
{
    clean();
    update();
}

void MyWindow::clean()
{
    unsigned char *ptr;
        ptr = displayPicture->bits();

        for(int i=0; i<wys; i++)
        {
            for(int j=0; j<szer; j++)
            {
                ptr[szer*4*i + 4*j]= 0;
                ptr[szer*4*i + 4*j + 1] = 0;
                ptr[szer*4*i + 4*j + 2] = 0;
            }
        }
     update();
}


void MyWindow::conversionWhiteBlack(QImage *displayPicture, int value){
    unsigned char *dp;

    dp = displayPicture->bits();

    int B=0,G=0,R=0, grey, color;
    for(int i = 0; i<wys; i++){
        for(int j = 0; j < szer; j++){
            B = dp[szer*4*i + 4*j];
            G = dp[szer*4*i + 4*j + 1];
            R = dp[szer*4*i + 4*j + 2];
            grey = (int)round(0.21 * R + 0.72 * G + 0.07 * B);
            if(grey < value){
                color = 0;
            }
            else color = 255;

            dp[szer*4*i + 4*j] = color;
            dp[szer*4*i + 4*j + 1] = color;
            dp[szer*4*i + 4*j + 2] = color;

        }
    }
}

void MyWindow::addImage(QString obraz){

    copyImage = new QImage(obraz);
    *displayPicture = copyImage -> copy(0, 0, szer, wys);
    conversionWhiteBlack(displayPicture, 140);
}

void MyWindow::on_AddButton_clicked()
{
   fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "..//..//OperacjeMorfologiczne//Image",tr("Pliki graficzne (*.png *.jpg *bmp)"));
   if(!(fileName.isNull())){
       addImage(fileName);
       update();
   }
}

void MyWindow::dilatation(){
    QImage *oryginal = new QImage(szer,wys,QImage::Format_RGB32);
    *oryginal = displayPicture -> copy(0, 0, szer, wys);;
    unsigned char *old, *dp; //old - obrazek nie edytowany, dp - edytuje to co widac
    bool d = false; //d - decyduje czy dany pixel zmieniamy
    dp = displayPicture -> bits();
    old = oryginal -> bits();
    for(int i=0; i<wys; i++){
        for(int j=0; j<szer; j++){
            for(int k = -mask/2; k <= mask/2; k++){
                for(int l=-mask/2; l <= mask/2; l++){
                    if((i + k) >= 0 && (i + k) < wys && (j + l) >= 0 && (j + l) < szer && (old[szer*4*(i+k) + 4*(j+l)] == 0)) d = true;
                    if(d){
                        for(int x=0; x<3; x++){
                            dp[szer*4*i + 4*j + x] = 0;
                            d = false;
                        }
                    }
                }
            }
        }
    }
}
void MyWindow::erosion(){
    QImage *oryginal = new QImage(szer,wys,QImage::Format_RGB32);
    *oryginal = displayPicture -> copy(0, 0, szer, wys);;
    unsigned char *old, *dp; //old - obrazek nie edytowany, dp - edytuje to co widac
    bool d = false; //d - decyduje czy dany pixel zmieniamy
    dp = displayPicture -> bits();
    old = oryginal -> bits();
    for(int i=0; i<wys; i++){
        for(int j=0; j<szer; j++){
            for(int k = -mask/2; k <= mask/2; k++){
                for(int l=-mask/2; l <= mask/2; l++){
                    if((i + k) >= 0 && (i + k) < wys && (j + l) >= 0 && (j + l) < szer && (old[szer*4*(i+k) + 4*(j+l)] == 255)) d = true;
                    if(d){
                        for(int x=0; x<3; x++){
                            dp[szer*4*i + 4*j + x] = 255;
                            d = false;
                        }
                    }
                }
            }
        }
    }
}



void MyWindow::on_ResetButton_clicked()
{
    *displayPicture = copyImage -> copy(0, 0, szer, wys);
    conversionWhiteBlack(displayPicture, 140);
    update();
}


void MyWindow::on_spinBox_valueChanged(int arg1)
{
    mask = arg1;
}


void MyWindow::on_openButton_clicked()
{
    erosion();
    dilatation();
    update();
}


void MyWindow::on_closeButton_clicked()
{
    dilatation();
    erosion();
    update();
}

