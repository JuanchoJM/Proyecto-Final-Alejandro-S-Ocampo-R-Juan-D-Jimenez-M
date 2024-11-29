#include "enemigonivel2.h"
#include <QGraphicsScene>
#include "proyectilnivel2.h"
EnemigoNivel2::EnemigoNivel2(QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent)
{
    // Inicializar temporizador para cambiar posición
    temporizadorPosicion = new QTimer(this);
    connect(temporizadorPosicion, &QTimer::timeout, this, &EnemigoNivel2::cambiarPosicion);
    temporizadorPosicion->start(1000); // Cambia la posición cada 5 segundos

    // Inicializar temporizador para disparar proyectiles
    temporizadorDisparo = new QTimer(this);
    connect(temporizadorDisparo, &QTimer::timeout, this, &EnemigoNivel2::dispararProyectil);
    temporizadorDisparo->start(1000); // Dispara cada 2 segundos

    // Configurar posición inicial
    cambiarPosicion();
}

void EnemigoNivel2::cargarImagen(const QString &rutaImagen)
{
    QPixmap pixmap(rutaImagen);
    setPixmap(pixmap.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void EnemigoNivel2::cambiarPosicion()
{

    int xAleatorio = QRandomGenerator::global()->bounded(1100, 1101);
    int yAleatorio = QRandomGenerator::global()->bounded(0, 400);

    setPos(xAleatorio, yAleatorio);
}


void EnemigoNivel2::dispararProyectil()
{
    if (!scene()) return;

    // Crear un proyectil y añadirlo a la escena
    ProyectilNivel2 *proyectil = new ProyectilNivel2();
    proyectil->setPos(x(), y() + pixmap().height() / 2); // Ajustar posición inicial
    scene()->addItem(proyectil);
}

