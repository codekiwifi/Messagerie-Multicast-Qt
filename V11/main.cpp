#include <QApplication>
#include "ServerMulti.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ServerMulti fenetre;
    fenetre.show();

    return app.exec();
}
