#include "mainwindow.h"
#include "ui_mainwindow.h"


/* Constructeur fenêtre principale */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isJoined_(false),
    count_image(0)
{
    /* Setup fenêtre principale */
    ui->setupUi(this);
    this->setWindowTitle("Messagerie Multicast");
    ui->lineEdit_pseudo->setText("Anonyme");


    /* Setup socket UDP */
    socket = new QUdpSocket(this);
    socket->setSocketOption(QAbstractSocket::MulticastTtlOption, 1);
    if (!socket->bind(QHostAddress(MULTICAST_ADDRESS), MULTICAST_PORT, QUdpSocket::ReuseAddressHint|QUdpSocket::ShareAddress))
        exit(EXIT_FAILURE);
    socket->joinMulticastGroup((QHostAddress(MULTICAST_ADDRESS)));
    connect(socket, &QUdpSocket::readyRead, this, &MainWindow::processDatagram);


    /* Création d'un dossier temporaire pour stockage des images */
    QDir dir_tmp;
    dir_tmp.cd("./tmp");
    if (!dir_tmp.exists())
    {
        if (!dir_tmp.mkdir("./tmp"))
            qDebug() << "dossier tmp non cree" << endl;
    }


    /* Pop-up pseudo du démarrage */
    bool ok;
    QString text = QInputDialog::getText(this, tr("Bienvenue"), tr("Choisissez votre pseudo :"), QLineEdit::Normal, QDir::home().dirName(), &ok);
    if (ok and !text.isEmpty())
        ui->lineEdit_pseudo->setText(text);
    else
        ui->lineEdit_pseudo->setText("Anonyme");
    ui->lineEdit_pseudo->setDisabled(true);


    /* Message d'accueil */
    ui->textBrowser->clear();
    ui->textBrowser->append("Bienvenue " + ui->lineEdit_pseudo->text().toUtf8() + " :) Ici on ne chat pas, on multicast !");


    /* Prévenir l'arrivée aux autres utilisateurs */
    on_pushButton_Send_clicked();
}


/* Destructeur fenêtre principale */
MainWindow::~MainWindow()
{
    /* Destruction du dossier tmp */
    // à faire

    /* Prevenir les autres utilisateurs le depart */
    QByteArray datagram = ui->lineEdit_pseudo->text().toUtf8();
    datagram = datagram +  ": :déconnecté#147258!@.";
    socket->writeDatagram(datagram, QHostAddress(MULTICAST_ADDRESS), MULTICAST_PORT);

    /* Destruction UI */
    delete ui;
}


/* Constructeur bool isJoined_ */
void MainWindow::setisJoined(const bool& isJoined)
{
    this->isJoined_ = isJoined;
}


/* Slot bouton "Envoyer" */
void MainWindow::on_pushButton_Send_clicked()
{
    /* Envoi de message */
    if (isJoined_ == true)
    {
        QByteArray message = ui->textEdit->toPlainText().toUtf8();
        QByteArray datagram = ui->lineEdit_pseudo->text().toUtf8() + " :" + QByteArray::number(anonymousNumber);
        datagram = datagram + " : " + message;
        socket->writeDatagram(datagram, QHostAddress(MULTICAST_ADDRESS), MULTICAST_PORT);
        ui->textEdit->clear();
        // qDebug() << datagram << endl;
    }

    /* Envoi du tout premier message & calcul du numéro anonyme */
    else
    {
        if (ui->lineEdit_pseudo->text().toUtf8() == "Anonyme")
        {
            qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
            anonymousNumber = qrand()%100;
        } else
            anonymousNumber = 0;
        QByteArray datagram = ui->lineEdit_pseudo->text().toUtf8() + ":" + QByteArray::number(anonymousNumber);
        datagram = datagram + ":arejoint#147258!@.";    // Code du premier message
        socket->writeDatagram(datagram, QHostAddress(MULTICAST_ADDRESS), MULTICAST_PORT);
        ui->textEdit->clear();
        setisJoined(true);
    }
}


/* Slot réception message */
void MainWindow::processDatagram()
{
    QByteArray long_image_datagram;
    while (socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress host;
        quint16 port;
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(), datagram.size(), &host, &port);

        /* Réception grande image */
        if (QString(datagram.data()).split(":")[0] == "#147258!@.img")
        {
            // Rellocage des paquets
            long_image_datagram.append(QString(datagram.data()).split(":")[1]);
            qDebug() << "test" << endl;
            qDebug() << long_image_datagram.size() << endl;
        } else if (QString(datagram.data()).split(":")[0] == "#147258!@.endlongimg")
        {
            // Reception du dernier paquet et affichage de l'image
            qDebug() << "test2" << endl;
            long_image_datagram.append(QString(datagram.data()).split(":")[1]);
            QImage image_temp = image_to_byte(QString(long_image_datagram.data()));
            image_temp.save(QString("./tmp/image%1.png").arg(count_image), "PNG");
            QImage* imgScaled = new QImage;
            *imgScaled = image_temp.scaled(400, 400, Qt::KeepAspectRatio);
            ui->label_image->setPixmap(QPixmap::fromImage(*imgScaled));
            count_image++;
            break;
        }

        /* Réception et affichage petite image */
        if (QString(datagram.data()).split(":")[0] == "#147258!@.endimg")
        {
            QImage image_temp = image_to_byte(QString(datagram.data()).split(":")[1]);
            image_temp.save(QString("./tmp/image%1.png").arg(count_image), "PNG");
            QImage* imgScaled = new QImage;
            *imgScaled = image_temp.scaled(400, 400, Qt::KeepAspectRatio);
            ui->label_image->setPixmap(QPixmap::fromImage(*imgScaled));
            count_image++;
            break;
        }

        /* Réception et affichage de message textuel */
        else if (QString(datagram.data()).split(":")[0] != "#147258!@.endimg" and QString(datagram.data()).split(":")[0] != "#147258!@.endlongimg" and QString(datagram.data()).split(":")[0] != "#147258!@.img")
        {
            QStringList msg_split = QString(datagram.data()).split(":");
            QTime time = QTime::currentTime();
            if (msg_split[0] == "Anonyme " and msg_split[2] != "/image#147258!@.")
            {
                ui->textBrowser->append(time.toString() + " | " + "Anonyme " + QString::number(port+msg_split[1].toInt()) + " :" + msg_split[2]);
            } else if (msg_split[0] == "Anonyme" and msg_split[2] == "/image#147258!@.")
            {
                ui->textBrowser->append(time.toString() + " | " + "Anonyme " + QString::number(port+msg_split[1].toInt()) + " a envoyé une image.");
                ui->label_envoyeurImage->setText("Image envoyée par Anonyme " + QString::number(port+msg_split[1].toInt()));
            } else if (msg_split[0] != "Anonyme" and msg_split[2] == "/image#147258!@.")
            {
                if (msg_split[0] == ui->lineEdit_pseudo->text().toUtf8())
                {
                    msg_split[0] = "Vous";
                    ui->textBrowser->append(time.toString() + " | " + "Vous avez envoyé une image.");
                    ui->label_envoyeurImage->setText("Image envoyée par vous");
                } else
                {
                    ui->textBrowser->append(time.toString() + " | " + msg_split[0] + " a envoyé une image.");
                    ui->label_envoyeurImage->setText("Image envoyée par " + msg_split[0]);
                }
            } else if (msg_split[0] != "Anonyme" and msg_split[2] == "arejoint#147258!@.")
            {
                if (!(msg_split[0] == ui->lineEdit_pseudo->text().toUtf8()))
                    ui->textBrowser->append(time.toString() + " | " + msg_split[0] + " a rejoint la discussion !");
            } else if (msg_split[0] == "Anonyme" and msg_split[2] == "arejoint#147258!@.")
            {
                ui->textBrowser->append(time.toString() + " | " + "Anonyme " + QString::number(port+msg_split[1].toInt()) + " a rejoint la discussion !");
            } else if (msg_split[2] == "déconnecté#147258!@.")
            {
                ui->textBrowser->append(time.toString() + " | " + msg_split[0] + " s'est déconnecté :(");
            } else
            {
                if (msg_split[0] == ui->lineEdit_pseudo->text().toUtf8() + " ")
                    msg_split[0] = "Vous ";
                ui->textBrowser->append(time.toString() + " | " + msg_split[0]  + ":" + msg_split[2]);
            }
        }
    }
}


/* Slot bouton "Envoyer une image" */
void MainWindow::on_pushButton_Send_Image_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Image à envoyer", QDir::currentPath(), "Image Files(*.jpg *.png)");
    if (file_name == "")
        return;

    QImage image;
    image.load(file_name);
    QString image_file_data = byte_from_imagefile(image);

    /* Envoi d'image de taille inférieure à 65000o */
    if (image_file_data.size() < 65000)
    {
        socket->writeDatagram("#147258!@.endimg:" + image_file_data.toLatin1(), QHostAddress(MULTICAST_ADDRESS), MULTICAST_PORT);
    }

    /* Envoi d'image de taille importante en plusieurs paquets */
    else
    {
        int i = 0;
        QString package;
        QString long_image_file_data = byte_from_imagefile(image);
        long_image_file_data = image_file_data.mid(1 + i * 64500);
        while (long_image_file_data.size() > 64500)
        {
            // Decoupage en paquets
            package = "#147258!@.img:" + image_file_data.mid(1 + i * 64500, 64500);
            socket->writeDatagram(package.toLatin1(), QHostAddress(MULTICAST_ADDRESS), MULTICAST_PORT);
            i++;
            long_image_file_data = image_file_data.mid(1 + i * 64500);
            qDebug() << long_image_file_data.size() << endl;
        }
        package = "#147258!@.endlongimg:" + image_file_data.mid(1 + i * 64500);  // Dernier paquet
        socket->writeDatagram(package.toLatin1(), QHostAddress(MULTICAST_ADDRESS), MULTICAST_PORT);
    }
    qDebug() << "go" << endl;
    QByteArray datagram = ui->lineEdit_pseudo->text().toUtf8() + ":" + QByteArray::number(anonymousNumber);
    datagram += ":/image#147258!@.";  // Code pour envoi d'image
    socket->writeDatagram(datagram, QHostAddress(MULTICAST_ADDRESS), MULTICAST_PORT);
}


/* Slot bouton "Enregistrer l'image" */
void MainWindow::on_pushButton_Save_Image_clicked()
{
    QString file_path = QFileDialog::getExistingDirectory(this, "Enregistrer sous...", "./");
    QImage image_temp = QImage(QString("./tmp/image%1.png").arg(count_image-1));
    if (!image_temp.save(file_path + QString("/image%1.png").arg(count_image-1), "PNG"))
    {
        ui->textBrowser->append(QString("image%1.png n'a pas pu être enregistré.").arg(count_image-1));
        return;
    }
    ui->textBrowser->append("L'image a été enregistrée sous " + file_path + QString("/image%1.png").arg(count_image-1));
}


/* Convertir image en QByteArray */
QByteArray MainWindow::byte_from_imagefile(const QImage &image)
{
    QByteArray imageData;
    QBuffer buffer(&imageData);
    image.save(&buffer, "JPG");
    imageData = imageData.toBase64();
    return imageData;
}


/* Convertir imgage en QImage */
QImage MainWindow::image_to_byte(const QString &data)
{
    QByteArray imageData = QByteArray::fromBase64(data.toLatin1());
    QImage image;
    image.loadFromData(imageData);
    return image;
}
