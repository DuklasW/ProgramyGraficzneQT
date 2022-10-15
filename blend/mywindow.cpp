#include "mywindow.h"
#include "ui_mywindow.h"
#include <QImage>
#include <QDebug>
MyWindow::MyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow)
{
    ui->setupUi(this);

    szer = ui->rysujFrame->width();
    wys = ui->rysujFrame->height();
    poczX = ui->rysujFrame->x();
    poczY = ui->rysujFrame->y();


    displayPicture = new QImage(szer,wys,QImage::Format_RGB32);

    //pictures[0] = new QImage(":/new/prefix1/Image/black.jpg");
    pictures[0] = new QImage(":/new/prefix1/Image/image1.jpg");
    pictures[1] = new QImage(":/new/prefix1/Image/image2.jpg");
    pictures[2] = new QImage(":/new/prefix1/Image/image3.jpg");
    pictures[3] = new QImage(":/new/prefix1/Image/image4.jpg");
    pictures[4] = new QImage(":/new/prefix1/Image/imgae5.jpg");
    img.push_back(0);
    //img.push_back(1);

    //qDebug()<<a[0] << " " << b[0];

    //choose();

    for(int i=0; i<5;i++){a[i]=1;b[i]=0;}
    blendPicture();
    update();
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

void MyWindow::on_cleanButton_clicked()
{
    clear();
    update();
}


void MyWindow::clear()
{
    unsigned char *ptr;
    ptr = displayPicture->bits();
    int i,j;

    qDebug() <<img;
    for(i=0; i<wys; i++)
    {
        for(j=0; j<szer; j++)
        {
            ptr[szer*4*i + 4*j] = 0; // Skladowa BLUE
            ptr[szer*4*i + 4*j + 1] = 0; // Skladowa GREEN
            ptr[szer*4*i + 4*j + 2] = 0; // Skladowa RED
        }
    }
}

/*

void MyWindow::showPicture(int index){
    unsigned char *tlo;
    unsigned char *ptr;
    ptr = pictures[index]->bits();
    tlo = displayPicture->bits();
    int i,j;
    qDebug()<<"Dziala";
    for(i=0; i<wys; i++)
    {
     for(j=0; j<szer; j++)
     {
        tlo[szer*4*i + 4*j] = ptr[szer*4*i + 4*j];
        tlo[szer*4*i + 4*j + 1] = ptr[szer*4*i + 4*j + 1];
        tlo[szer*4*i + 4*j + 2] = ptr[szer*4*i + 4*j + 2];
     }
    }
}

void MyWindow::choose(){
    if(img.isEmpty()){ clear(1);}
    else showPicture(img.last());
    update();
}
*/
void MyWindow::blendPicture(){
    int i,j, alpha;
    std::sort(img.begin(), img.end());


    QImage *background;
    background = new QImage(szer,wys,QImage::Format_RGB32);//background

    unsigned char *dp, *pt, *bg; //dp co to widzimy, pt - obrazek, bg - tlo

    dp = displayPicture->bits();
    bg = background->bits();

    for(int k=0; k<img.size(); k++){
        alpha = img[k];
        if(k != 0) bg = displayPicture ->bits();
        pt = pictures[alpha]->bits();
        switch(b[alpha]){
            case 0:
                for(i = 0; i <wys; i++)
                for(j = 0; j <szer; j++)
                {
                    for(int p = 0; p < 3; p++)
                        dp[szer*4*i + 4*j + p] = a[alpha]*pt[szer*4*i + 4*j + p] + (1.0 - a[alpha])*bg[szer*4*i + 4*j + p];
                }
                break;

            case 1:
                for(i = 0; i <wys; i++)
                for(j = 0; j <szer; j++)
                {
                    for(int p = 0; p < 3; p++)
                        dp[szer*4*i + 4*j + p] = (int)(a[alpha]*pt[szer*4*i + 4*j + p]*bg[szer*4*i + 4*j + p]) >> 8;

                }
                break;
            case 2:
                for(i = 0; i <wys; i++)
                for(j = 0; j <szer; j++)
                {
                    for(int p = 0; p < 3; p++){
                        int color;
                        if(bg[szer*4*i + 4*j + p] < pt[szer*4*i + 4*j + p]) color = bg[szer*4*i + 4*j + p];
                        else color = pt[szer*4*i + 4*j +p];
                        dp[szer*4*i + 4*j + p] = a[alpha]*color + (1 - a[alpha])*bg[szer*4*i+ 4*j + p];
                        }
                }
                break;
            case 3:
                for(i = 0; i <wys; i++)
                for(j = 0; j <szer; j++)
                {
                    for(int p = 0; p < 3; p++){
                        int color;
                        if(bg[szer*4*i + 4*j + p] > pt[szer*4*i + 4*j + p]) color = bg[szer*4*i + 4*j + p];
                        else color = pt[szer*4*i + 4*j +p];
                        dp[szer*4*i + 4*j + p] = a[alpha]*color + (1 - a[alpha])*bg[szer*4*i+ 4*j + p];
                    }
                }
                break;
        }
    }
    update();
}

void MyWindow::on_Rysunek1WyborBlendu_activated(int index)
{
    b[0] = index;
    blendPicture();
}
void MyWindow::on_Rysunek2WyborBlendu_activated(int index)
{
    b[1] = index;
    blendPicture();
}
void MyWindow::on_Rysunek3WyborBlendu_activated(int index)
{
    b[2] = index;
    blendPicture();
}
void MyWindow::on_Rysunek4WyborBlendu_activated(int index)
{
    b[3] = index;
    blendPicture();
}
void MyWindow::on_Rysunek5WyborBlendu_activated(int index)
{
    b[4] = index;
    blendPicture();
}

void MyWindow::on_Rysunek1Slider_sliderMoved(int position)
{
    a[0] = (position*1.0)/(100*1.0);
   blendPicture();
}
void MyWindow::on_Rysunek2Slider_sliderMoved(int position)
{
    a[1] = (position*1.0)/(100*1.0);
    blendPicture();
}
void MyWindow::on_Rysunek3Slider_sliderMoved(int position)
{
    a[2] = (position*1.0)/(100*1.0);
    blendPicture();
}
void MyWindow::on_Rysunek4Slider_sliderMoved(int position)
{
    a[3] = (position*1.0)/(100*1.0);
    blendPicture();
}
void MyWindow::on_Rysunek5Slider_sliderMoved(int position)
{
    a[4] = (position*1.0)/(100*1.0);
    blendPicture();
}

void MyWindow::on_Rysunek1CheckBox_clicked(bool checked)
{
    if(checked == true){img.push_back(0);}
    else img.removeOne(0);
    blendPicture();
}
void MyWindow::on_Rysunek2CheckBox_clicked(bool checked)
{
    if(checked == true){img.push_back(1);}
    else img.removeOne(1);
    blendPicture();
}
void MyWindow::on_Rysunek3CheckBox_clicked(bool checked)
{
    if(checked == true){img.push_back(2);}
    else img.removeOne(2);
    blendPicture();
}
void MyWindow::on_Rysunek4CheckBox_clicked(bool checked)
{
    if(checked == true){img.push_back(3);}
    else img.removeOne(3);
    blendPicture();

}
void MyWindow::on_Rysunek5CheckBox_clicked(bool checked)
{
    if(checked == true){img.push_back(4);}
    else img.removeOne(4);
    blendPicture();

}

