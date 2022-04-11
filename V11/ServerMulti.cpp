#include "ServerMulti.hpp"

ServerMulti::ServerMulti() : QWidget()
{
    setFixedSize(1600,800);
    bouton_envoi = new QPushButton("Envoyer", this);
    bouton_envoi->move(1200, 700);
    bouton_envoi->setCursor(Qt::PointingHandCursor);
    bouton_envoi->setFont(QFont("Poppins", 14));

    bouton_envoi_img = new QPushButton("Envoyer l'image", this);
    bouton_envoi_img->move(1300, 700);
    bouton_envoi_img->setCursor(Qt::PointingHandCursor);
    bouton_envoi_img->setFont(QFont("Poppins", 14));

    label_message = new QLabel("Message : ", this);
    label_message->setFont(QFont("Poppins", 14));
    label_message->move(150, 705);

    label_pseudo = new QLabel("Pseudo : ", this);
    label_pseudo->setFont(QFont("Poppins", 14));
    label_pseudo->move(1200, 105);

    message_edit = new QLineEdit(this);
    message_edit->setFixedSize(900, 30);
    message_edit->move(250, 700);

    pseudo_edit = new QLineEdit("toto", this);
    pseudo_edit->setFixedSize(200, 30);
    pseudo_edit->move(1300, 100);

    message_box = new QTextBrowser(this);
    message_box->setFixedSize(1000, 550);
    message_box->move(150,100);
}

