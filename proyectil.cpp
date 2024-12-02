#include "proyectil.h"
#include <QGraphicsScene>

Proyectil::Proyectil(QGraphicsItem* propietario, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent), propietario(propietario) {
    // Cargar imagen del proyectil
    QPixmap imagen(":/Imagenes/hacha.png");
    setPixmap(imagen.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Determinar dirección aleatoria (-1 para izquierda, 1 para derecha)
    direccion = (rand() % 2 == 0) ? -1 : 1;

    // Configurar temporizador para movimiento
    temporizadorMovimiento = new QTimer(this);
    connect(temporizadorMovimiento, &QTimer::timeout, this, &Proyectil::mover);
    temporizadorMovimiento->start(30); // Mover cada 30 ms
}

void Proyectil::mover() {
    // Movimiento hacia arriba y lateral
    int velocidadVertical = 1;
    int velocidadLateral = 7 * direccion;

    setPos(x() + velocidadLateral, y() + velocidadVertical);

    // Eliminar el proyectil si sale de la escena
    if (y() + pixmap().height() < 0 || x() + pixmap().width() < 0 || x() > scene()->width()) {
        delete this;
    }
}
void Proyectil::establecerDireccion(int nuevaDireccion) {
    direccion = nuevaDireccion; // Asignar dirección: -1 para izquierda, 1 para derecha
}
