#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QImage>
#include <QBuffer>
#include <QtGlobal>
#include <QTime>
#include <QFileDialog>
#include <QInputDialog>


/* Adresse D et port qu'on va utiliser */
#define MULTICAST_ADDRESS "239.255.255.252"
#define MULTICAST_PORT    12345


namespace Ui{
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /* Constructeur fenêtre principale */
    explicit MainWindow(QWidget *parent = 0);

    /* Destructeur fenêtre principale */
    ~MainWindow();


public:
    /* Constructeur bool isJoined_ */
    void setisJoined(const bool& isJoined);


private slots:
    /* Slot réception message */
    void processDatagram();

    /* Slot bouton "Envoyer" */
    void on_pushButton_Send_clicked();

    /* Slot bouton "Envoyer une image" */
    void on_pushButton_Send_Image_clicked();

    /* Slot bouton "Enregistrer l'image" */
    void on_pushButton_Save_Image_clicked();


private:
    Ui::MainWindow *ui;

    /* Socket UDP */
    QUdpSocket* socket;


private:
    /* isJoined_ */
    bool isJoined_;

    /* Le numéro anonyme */
    int anonymousNumber;

    /* count_image */
    int count_image;


private:
    /* Convertir image en QByteArray */
    QByteArray byte_from_imagefile(const QImage &image);

    /* Convertir imgage en QImage */
    QImage image_to_byte(const QString &data);
};

#endif // MAINWINDOW_H
