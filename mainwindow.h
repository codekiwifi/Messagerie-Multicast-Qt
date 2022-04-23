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

    QString image_file_data;
    QImage image;
    QString picture_data;
    QByteArray get_imagedata_from_imagefile(const QImage &image);
    QImage get_imagedata_from_byte(const QString &data);
    // int isAnonymous = true;
    int count_image = 0;

private slots:
    void processPendingDatagram();
    void on_pushButton_Send_clicked();
    void on_pushButton_Send_Image_clicked();

    void on_pushButton_Send_Image_2_clicked();

private:
    Ui::MainWindow *ui;
    QUdpSocket* sender, * receiver;
    // QUdpSocket* socket;
};

#endif // MAINWINDOW_H
