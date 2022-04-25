#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QImage>
#include <QBuffer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    void setisJoined(const bool& isJoined);

private slots:
    void processPendingDatagram();
    void on_pushButton_Send_clicked();
    void on_pushButton_Send_Image_clicked();
    void on_pushButton_Send_Image_2_clicked();

private:
    Ui::MainWindow *ui;
    QUdpSocket* socket;

private:
    bool isJoined_;
    int count_image;
    QString image_file_data;
    QImage image;
    QString picture_data;

private:
    QByteArray get_imagedata_from_imagefile(const QImage &image);
    QImage get_imagedata_from_byte(const QString &data);

};

#endif // MAINWINDOW_H
