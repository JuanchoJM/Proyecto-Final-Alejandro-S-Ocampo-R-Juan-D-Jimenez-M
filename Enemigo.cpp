#include "Enemigo.h"
#include <QRandomGenerator>
#include <QDebug>
#include <QMessageBox>
#include <QApplication>
#include "proyectil.h"
#include "jugador.h"
Enemigo::Enemigo(QGraphicsItem* parent)
    : Jugador(parent) {
    // Temporizadores
    temporizadorMovimiento = new QTimer(this);
    connect(temporizadorMovimiento, &QTimer::timeout, this, &Enemigo::moverHaciaJugador);
    vidaEnemigo=20;
    temporizadorDisparo = new QTimer(this);
    connect(temporizadorDisparo, &QTimer::timeout, this, &Enemigo::disparar);
    temporizadorvidaEnemigo= new QTimer(this);
    connect(temporizadorvidaEnemigo, &QTimer::timeout, this, &Enemigo::detectarColisionConProyectil);
    iniciarMovimiento();
    gravedad=1;

}

Enemigo::~Enemigo() {
    delete temporizadorMovimiento;
    delete temporizadorDisparo;
    delete temporizadorvidaEnemigo;
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
    temporizadorMovimiento->start(30); // Mover cada 30 ms
    temporizadorDisparo->start(1000);   // Disparar cada 1 segundo
    temporizadorvidaEnemigo->start(5);
}

void Enemigo::moverHaciaJugador() {

    if (!scene()) return;

    // Buscar al jugador en la escena
    Jugador* jugador = nullptr;
    QList<QGraphicsItem*> itemsEnEscena = scene()->items();

    for (QGraphicsItem* item : itemsEnEscena) {
        jugador = dynamic_cast<Jugador*>(item);
        if (jugador && jugador != this) { // Asegurarse de que no sea el propio enemigo
            break;
        }
    }

    if (!jugador) {
        qDebug() << "No se encontró al jugador";
        return;
    }

    // Calcular distancia al jugador
    qreal dx = jugador->x() - x();
    qreal dy = jugador->y() - y();
    qreal distancia = std::sqrt(dx * dx + dy * dy);


    // Si la distancia es mayor a un umbral, moverse
    if (distancia > 5.0) { // Cambia este valor según la precisión que desees
        qreal velocidadX = 5;   // Velocidad del enemigo en X
        qreal velocidadY = 0;   // Velocidad inicial en Y

        if (distancia < 800.0 && distancia >200.0) { // Si la distancia es considerable, realizar un salto
            velocidadY = -15;   // Valor de la velocidad vertical
        }

        // Actualizar posición con velocidad en X e Y
        setPos(x() + velocidadX * (dx / distancia),
               y() + velocidadY);

    } else {
        qDebug() << "Enemigo ya está cerca del jugador";
    }
}

void Enemigo::disparar() {
    // Crear un temporizador
    QTimer* temporizadorDisparo = new QTimer(this);
    temporizadorDisparo->setSingleShot(true); // El temporizador se dispara solo una vez

    // Conectar el temporizador a la función que va a disparar el proyectil
    connect(temporizadorDisparo, &QTimer::timeout, this, [this]() {
        // Crear un proyectil
        Proyectil* proyectil = new Proyectil(this);
        proyectil->setPos(x() + pixmap().width() / 2 - proyectil->pixmap().width() / 2, y());
        if (scene()) {
            scene()->addItem(proyectil);
        }
    });

    // Iniciar el temporizador con el tiempo especificado (en milisegundos)
    temporizadorDisparo->start(35000);  // 10000 ms = 10 segundos
}

void Enemigo::detectarColisionConProyectil() {

    QList<QGraphicsItem*> itemsColisionados = collidingItems();

    for (QGraphicsItem* item : itemsColisionados) {
        // Verifica si el objeto es un proyectil
        Proyectil* proyectil = dynamic_cast<Proyectil*>(item);

        if (proyectil) {
            // Si el proyectil fue disparado por el jugador, reduce vidas del enemigo
            if (proyectil->obtenerPropietario() != this) {
                // El proyectil no fue disparado por el enemigo, por lo tanto es del jugador
                vidaEnemigo--;
                  emit vidasCambiadas(vidaEnemigo); // Emitir señal cuando las vidas cambien
                 qDebug() << "Vida del enemigo: " << vidaEnemigo;
                // Eliminar el proyectil
                scene()->removeItem(proyectil);
                delete proyectil;

                if (vidaEnemigo <= 0) {
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("Enemigo derrotado");
                    msgBox.setText("¡Has derrotado al enemigo!");
                    msgBox.setIconPixmap(QPixmap(":/Imagenes/corono.png")); // Imagen grande
                    msgBox.exec();
                    delete this;
                    return;
                }
            }
        }
    }
}
