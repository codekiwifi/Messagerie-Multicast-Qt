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
    /* Slot réception message
     * Lecture, traitement et affichage des messages reçus.
     * On distingue plusieurs cas de figure en fonction du message codé reçu.
     * Le codage a pour but de différencier les messages spéciaux (messages d'arrivée/de départ, images...) avec les messages textuels normaux.
     * Les messages sont composés de 3 parties séparées par des ":" au format "pseudo:numero anonyme:message utile". */
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
    /* Si isJoined_ = false, l'utilisateur annonce à tout le monde son arrivée. Ce bool ne sera donc utilisé qu'une fois */
    bool isJoined_;

    /* Le numéro anonyme sert à différencier les anonymes.
     * Ce nombre est généré grâce à un nombre pseudo-aléatoire.
     * Il sera additionné au numéro de port avant d'être affiché sur la fenêtre de chat.
     * Il vaut 0 si l'utilisateur n'est pas anonyme.
     * Un utilisateur est anonyme s'il clique sur "cancel" lors du choix du pseudo */
    int anonymousNumber;

    /* count_image sert à numéroter les images.
     * Il est utilisé lors du nommage des images. */
    int count_image;


private:
    /* Convertir image en QByteArray */
    QByteArray byte_from_imagefile(const QImage &image);

    /* Convertir imgage en QImage */
    QImage image_to_byte(const QString &data);
};

#endif // MAINWINDOW_H
