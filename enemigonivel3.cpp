#include "enemigonivel3.h"
#include <QGraphicsScene>
#include <QtMath> // Para qSqrt
#include <QDebug>

EnemigoNivel3::EnemigoNivel3(QGraphicsItem* parent)
    : JugadorNivel3(parent),vida(10), jugador(nullptr) {
    // Cargar imágenes para cada dirección
    imagenAbajo = QPixmap(":/Imagenes/BobFabajo.PNG").scaled(50, 50);
    imagenArriba = QPixmap(":/Imagenes/BobFariiba.PNG").scaled(50, 50);
    imagenIzquierda = QPixmap(":/Imagenes/BobFi.PNG").scaled(50, 50);
    imagenDerecha = QPixmap(":/Imagenes/BobFd.PNG").scaled(50, 50);

    // Establecer imagen inicial
    setPixmap(imagenAbajo);

    // Temporizador para actualizar la persecución
    timerPersecucion = new QTimer(this);
    connect(timerPersecucion, &QTimer::timeout, this, &EnemigoNivel3::perseguirJugador);
    timerPersecucion->start(50); // Actualizar cada 50 ms

    // Temporizador para atacar al jugador
    timerAtaque = new QTimer(this);
    connect(timerAtaque, &QTimer::timeout, this, &EnemigoNivel3::atacarJugador);
    timerAtaque->start(500); // Intentar atacar cada 500 ms

    velocidad = 10;
}

void EnemigoNivel3::setJugador(JugadorNivel3* jugador) {
    this->jugador = jugador;
}

bool EnemigoNivel3::puedeMoverse(qreal nuevoX, qreal nuevoY) {
    // Guardar la posición original del enemigo
    QPointF posicionOriginal = pos();

    // Mover temporalmente al enemigo a la nueva posición
    setPos(nuevoX, nuevoY);

    // Verificar colisiones con las plataformas
    for (QGraphicsItem* plataforma : plataformas) {
        if (collidesWithItem(plataforma)) {
            // Si hay colisión, restaurar la posición original y retornar false
            setPos(posicionOriginal);
            return false;
        }
    }

    // Restaurar la posición original si no hay colisiones
    setPos(posicionOriginal);
    return true; // Puede moverse
}


void EnemigoNivel3::perseguirJugador() {
    if (!jugador) return; // Asegurarse de que hay un jugador asignado

    // Obtener la posición actual del jugador
    QPointF posicionJugador = jugador->pos();

    // Calcular la dirección hacia el jugador
    qreal deltaX = posicionJugador.x() - x();
    qreal deltaY = posicionJugador.y() - y();

    // Normalizar el movimiento para que sea constante
    qreal magnitud = qSqrt(deltaX * deltaX + deltaY * deltaY);
    if (magnitud > 0) {
        deltaX /= magnitud;
        deltaY /= magnitud;
    }

    // Cambiar la imagen según la dirección
    if (qAbs(deltaX) > qAbs(deltaY)) {
        if (deltaX > 0) {
            setPixmap(imagenDerecha); // Movimiento hacia la derecha
        } else {
            setPixmap(imagenIzquierda); // Movimiento hacia la izquierda
        }
    } else {
        if (deltaY > 0) {
            setPixmap(imagenAbajo); // Movimiento hacia abajo
        } else {
            setPixmap(imagenArriba); // Movimiento hacia arriba
        }
    }

    // Calcular la nueva posición
    qreal nuevoX = x() + deltaX * velocidad;
    qreal nuevoY = y() + deltaY * velocidad;

    // Verificar colisiones con plataformas
    if (puedeMoverse(nuevoX, y())) {
        setX(nuevoX);
    }
    if (puedeMoverse(x(), nuevoY)) {
        setY(nuevoY);
    }
}

void EnemigoNivel3::actualizarFisica() {
    // Este método puede usarse para gestionar comportamientos adicionales de movimiento
    perseguirJugador(); // Llamar a la lógica de persecución en cada actualización
}
void EnemigoNivel3::atacarJugador() {
    if (!jugador) return; // Asegurarse de que hay un jugador asignado

    // Calcular la distancia al jugador
    qreal distancia = qSqrt(qPow(jugador->x() - x(), 2) + qPow(jugador->y() - y(), 2));

    // Verificar si el jugador está dentro del rango de ataque
    if (distancia <= rangoAtaque) {
        // Reducir las vidas del jugador
        jugador->reducirVidas(1);
        qDebug() << "¡Ataque realizado! Vidas restantes del jugador:" << jugador->obtenerVidas();
    }
}
void EnemigoNivel3::recibirDano(int dano) {
    vida -= dano;  // Reducir la vida por el daño recibido

    if (vida <= 0) {
        // El enemigo muere cuando su vida llega a cero o menos
        scene()->removeItem(this);  // Eliminar el enemigo de la escena
        delete this;  // Eliminar el objeto de memoria
    }
}
int EnemigoNivel3::obtenervidaE(){return vida;}
