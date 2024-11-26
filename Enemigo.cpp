#include "Enemigo.h"
#include <QRandomGenerator>
#include <QDebug>
#include "proyectil.h"
#include <QMessageBox>
Enemigo::Enemigo(QGraphicsItem* parent)
    : Jugador(parent) {
    // Temporizadores
    temporizadorMovimiento = new QTimer(this);
    connect(temporizadorMovimiento, &QTimer::timeout, this, &Enemigo::moverAleatorio);
    vidaEnemigo=5;
    temporizadorDisparo = new QTimer(this);
    connect(temporizadorDisparo, &QTimer::timeout, this, &Enemigo::disparar);

    iniciarMovimiento();
}

Enemigo::~Enemigo() {
    delete temporizadorMovimiento;
    delete temporizadorDisparo;
}

void Enemigo::cargarImagen(const QString& rutaImagen) {
    QPixmap imagen(rutaImagen);
    if (!imagen.isNull()) {
        setPixmap(imagen.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        qWarning("No se pudo cargar la imagen del enemigo.");
    }
}

void Enemigo::iniciarMovimiento() {
    temporizadorMovimiento->start(500); // Mover cada 500 ms
    temporizadorDisparo->start(500);   // Disparar cada 1 segundo
}

void Enemigo::moverAleatorio() {
    // Movimiento aleatorio
    float desplazamientoX = QRandomGenerator::global()->bounded(-100, 100); // Aleatorio entre -10 y 10

    float nuevoX = x() + desplazamientoX;
    float nuevoY = y();

    // Verificar límites de la escena
    if (scene()) {
        if (nuevoX < 0) nuevoX = 0;
        if (nuevoX + pixmap().width() > scene()->width())
            nuevoX = scene()->width() - pixmap().width();
        if (nuevoY < 0) nuevoY = 0;
        if (nuevoY + pixmap().height() > scene()->height())
            nuevoY = scene()->height() - pixmap().height();
    }

    setPos(nuevoX, nuevoY);

    // Saltar ocasionalmente
    if (QRandomGenerator::global()->bounded(100) < 20) { // Probabilidad del 20%
        velocidadY = -15;
    }
}

void Enemigo::disparar() {
    // Crear un proyectil
    Proyectil* proyectil = new Proyectil(this);
    proyectil->setPos(x() + pixmap().width() / 2 - proyectil->pixmap().width() / 2, y());
    if (scene()) {
        scene()->addItem(proyectil);
    }
}
void Enemigo::detectarColisionConProyectil() {
    QList<QGraphicsItem*> itemsColisionados = collidingItems();

    for (QGraphicsItem* item : itemsColisionados) {
        // Verifica si el objeto es un proyectil
        Proyectil* proyectil = dynamic_cast<Proyectil*>(item);

        if (proyectil) {
            // Si el proyectil fue disparado por el jugador, reduce vidas del enemigo
            if (proyectil->obtenerPropietario() != this) {
                // El proyectil fue disparado por el enemigo, por lo tanto es del jugador
                vidaEnemigo--;
                qDebug() << "Vida del enemigo: " << vidaEnemigo;

                // Eliminar el proyectil
                scene()->removeItem(proyectil);
                delete proyectil;

                if (vidaEnemigo <= 0) {
                    QMessageBox::information(nullptr, "Enemigo derrotado", "¡Has derrotado al enemigo!");
                    delete this;
                    return;
                }
            }
        }
    }
}
