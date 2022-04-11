#ifndef SERVERMULTI_H
#define SERVERMULTI_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextBrowser>

class ServerMulti : public QWidget
{
    public:
        ServerMulti();

    private:
        QPushButton     *bouton_envoi;
        QPushButton *bouton_envoi_img;
        QLabel         *label_message;
        QLabel          *label_pseudo;
        QLineEdit       *message_edit;
        QLineEdit        *pseudo_edit;
        QTextBrowser     *message_box;
};

#endif // SERVERMULTI_H
