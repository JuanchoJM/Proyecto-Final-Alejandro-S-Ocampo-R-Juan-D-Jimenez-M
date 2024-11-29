#include "jugadornivel2.h"
#include "proyectilnivel2.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QMessageBox>
ProyectilNivel2::ProyectilNivel2(QGraphicsItem *parent) : QGraphicsPixmapItem(parent)  // El parent se pasa solo a QGraphicsPixmapItem
{
    // Selección aleatoria entre piedra y tronco
    QPixmap pixmap = (QRandomGenerator::global()->bounded(2) == 0) ?
                         QPixmap(":/Imagenes1/piedra2.png") : QPixmap(":/Imagenes1/Tronco.png");

    setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, &ProyectilNivel2::mover);
    temporizador->start(50);  // Mover cada 50 ms
}

void ProyectilNivel2::mover() {
    // Mover el proyectil hacia la izquierda
    setPos(x() - 10, y());

    // Verifica si el proyectil salió de la escena
    if (x() + pixmap().width() < 0) {
        scene()->removeItem(this);  // Eliminar proyectil de la escena
        delete this;  // Eliminar objeto de la memoria
        return;
    }

    // Detectar colisión con el jugador
    QList<QGraphicsItem *> colisiones = collidingItems();
    for (QGraphicsItem *item : colisiones) {
        JugadorNivel2 *jugador = dynamic_cast<JugadorNivel2 *>(item);
        if (jugador) {
            jugador->reducirVida();  // Llamar a reducirVida() cuando hay colisión con el jugador
            scene()->removeItem(this);  // Eliminar proyectil
            delete this;  // Eliminar objeto de la memoria
            return;
        }
    }
}


