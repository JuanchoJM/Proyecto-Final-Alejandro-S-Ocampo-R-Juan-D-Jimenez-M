#include "jugador.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QMessageBox>
#include "proyectil.h"
#include <QApplication>

Jugador::Jugador(QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent),
    indiceImagen(0),
    avanzando(true), direccionDerecha(true), descendiendo(false),
    velocidadX(0), velocidadY(0), gravedad(1.0f), enElAire(false), vidas(5)
{
    int anchoDeseado = 60;
    int altoDeseado = 60;

    for (int i = 1; i <= 8; ++i) {
        QPixmap imagen = QPixmap(QString("C:\\Users\\PC\\Documents\\ProyectoFinal\\BartD%1.png").arg(i));
        imagenesDerecha.append(imagen.scaled(anchoDeseado, altoDeseado, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    for (int i = 1; i <= 7; ++i) {
        QPixmap imagen = QPixmap(QString("C:\\Users\\PC\\Documents\\ProyectoFinal\\BartI%1.PNG").arg(i));
        imagenesIzquierda.append(imagen.scaled(anchoDeseado, altoDeseado, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    setPixmap(imagenesDerecha[indiceImagen]);

    // Temporizadores
    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, &Jugador::actualizarImagen);

    temporizadorFisica = new QTimer(this);
    connect(temporizadorFisica, &QTimer::timeout, this, &Jugador::actualizarFisica);
    temporizadorFisica->start(30); // Aproximadamente 60 FPS

    // Temporizador para detección de colisiones con proyectiles
    QTimer* temporizadorColision = new QTimer(this);
    connect(temporizadorColision, &QTimer::timeout, this, &Jugador::detectarColisionConProyectil);
    temporizadorColision->start(30); // Llamada periódica para verificar colisiones
}

void Jugador::keyPressEvent(QKeyEvent* event) {
    int step = 7;

    if (event->key() == Qt::Key_Right) {
        velocidadX = step;
        direccionDerecha = true;
    } else if (event->key() == Qt::Key_Left) {
        velocidadX = -step;
        direccionDerecha = false;
    } else if (event->key() == Qt::Key_Space && !enElAire) {
        velocidadY = -15;
        enElAire = true;
    } else if (event->key() == Qt::Key_Down) {
        descendiendo = true; // Activar modo de descenso
    }
    else if (event->key() == Qt::Key_W) {
        descendiendo = false; // Activar modo de descenso
    }

    actualizarImagen();
}

void Jugador::keyReleaseEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Right || event->key() == Qt::Key_Left) {
        velocidadX = 0;
    }
    actualizarImagen();
}

bool Jugador::detectarColisionConPlataformas() {
    for (QGraphicsItem* plataforma : plataformas) {
        if (collidesWithItem(plataforma)) {
            return true; // Colisión detectada
        }
    }
    return false;
}

void Jugador::actualizarFisica() {
    enColisionConPlataforma = false;

    for (QGraphicsItem* plataforma : plataformas) {
        if (collidesWithItem(plataforma)) {
            if (descendiendo) {

                //descendiendo = false;
                continue;
            }

            enColisionConPlataforma = true;

            if (velocidadY > 0) { // Si el jugador está cayendo
                velocidadY = 0;
            }
            break; // Salir del bucle, no necesitamos más verificaciones
        }
    }

    enElAire = !enColisionConPlataforma;

    if (enElAire) {
        velocidadY += gravedad; // Aplicar gravedad si está en el aire
    }

    float nuevoX = x() + velocidadX;
    float nuevoY = y() + velocidadY;

    if (scene()) { // Verificar que el jugador está en una escena
        // Obtener las dimensiones de la escena
        float anchoEscena = scene()->width();
        float altoEscena = scene()->height();

        // Verificar los límites horizontales (izquierda y derecha)
        if (nuevoX < 110) { // Borde izquierdo
            nuevoX = 110; // Restringir al borde izquierdo
            velocidadX = 0; // Detener movimiento hacia la izquierda
        } else if (nuevoX + pixmap().width() > anchoEscena) { // Borde derecho
            nuevoX = anchoEscena - pixmap().width(); // Restringir al borde derecho
            velocidadX = 0; // Detener movimiento hacia la derecha
        }

        // Verificar los límites verticales (arriba y abajo)
        if (nuevoY + pixmap().height() < 0) { // Sale por arriba
            nuevoY = altoEscena; // Aparece en la parte inferior
        } else if (nuevoY > altoEscena) { // Sale por abajo
            nuevoY = -pixmap().height(); // Aparece en la parte superior
        }
    }

    // Actualizar posición del jugador
    setPos(nuevoX, nuevoY);
}

void Jugador::actualizarImagen() {
    const QVector<QPixmap>& imagenesActuales = direccionDerecha ? imagenesDerecha : imagenesIzquierda;

    if (avanzando) {
        indiceImagen++;
        if (indiceImagen >= imagenesActuales.size() - 1) {
            avanzando = false;
        }
    } else {
        indiceImagen--;
        if (indiceImagen <= 0) {
            avanzando = true;
        }
    }

    setPixmap(imagenesActuales[indiceImagen]);
}

void Jugador::setPlataformas(const QList<QGraphicsItem*>& plataformas) {
    this->plataformas = plataformas;
}



void Jugador::detectarColisionConProyectil() {
    QList<QGraphicsItem*> itemsColisionados = collidingItems();

    for (QGraphicsItem* item : itemsColisionados) {
        Proyectil* proyectil = dynamic_cast<Proyectil*>(item);

        if (proyectil) {
            if (proyectil->obtenerPropietario() == this) {
                continue;
            }

            scene()->removeItem(proyectil);
            delete proyectil;

            vidas--;
            emit vidasCambiadas(vidas); // Emitir señal cuando las vidas cambien

            qDebug() << "Vidas restantes: " << vidas;

            if (vidas <= 0) {
                QMessageBox::information(nullptr, "Game Over", "¡Perdio mi papa!");
                QApplication::quit();
                return;
            }
        }
    }
}



