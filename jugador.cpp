#include "jugador.h"
#include <QGraphicsScene>
#include <QDebug>
#include "proyectil.h"
#include <QMessageBox>
#include <QApplication>
Jugador::Jugador(QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent),
    vidas(5), indiceImagen(0), avanzando(true),direccionDerecha(true),
    descendiendo(false), velocidadX(0), velocidadY(0), gravedad(1.0f),enElAire(false)
{
    int anchoDeseado = 60;
    int altoDeseado = 60;

    for (int i = 1; i <= 8; ++i) {
        QPixmap imagen = QPixmap(QString(":Imagenes1/BartD%1.png").arg(i));
        imagenesDerecha.append(imagen.scaled(anchoDeseado, altoDeseado, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    for (int i = 1; i <= 7; ++i) {
        QPixmap imagen = QPixmap(QString(":Imagenes1/Barti%1.PNG").arg(i));
        imagenesIzquierda.append(imagen.scaled(anchoDeseado, altoDeseado, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    imagenDisparoDerecha = QPixmap(":Imagenes1/BartDisparoDerecha.png").scaled(anchoDeseado, altoDeseado, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imagenDisparoIzquierda = QPixmap(":Imagenes1/BartDisparoIzquierda.png").scaled(anchoDeseado, altoDeseado, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(imagenesDerecha[indiceImagen]);

    // Temporizadores
    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, &Jugador::actualizarImagen);

    temporizadorFisica = new QTimer(this);
    connect(temporizadorFisica, &QTimer::timeout, this, &Jugador::actualizarFisica);
    temporizadorFisica->start(30); // Aproximadamente 60 FPS
    temporizadorvidas= new QTimer(this);
    connect(temporizadorvidas, &QTimer::timeout, this, &Jugador::detectarColisionConProyectil);
      temporizadorvidas->start(5);
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
    } else if (event->key() == Qt::Key_Q) {
        dispararPiedraIzquierda();
        enEstadoDeDisparo = true;
    } else if (event->key() == Qt::Key_E) {
        dispararPiedraDerecha();
        enEstadoDeDisparo = true;
    }

    actualizarImagen();
}
void Jugador::keyReleaseEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Right || event->key() == Qt::Key_Left) {
        velocidadX = 0;
    }
    actualizarImagen();
}
bool Jugador::detectarColisionConPlataformas()
{
    for (QGraphicsItem* plataforma : plataformas) {
        if (collidesWithItem(plataforma)) {
            // Si detecta una colisión con una plataforma, devuelve true
            return true;
        }
    }
    // Si no hay colisión con ninguna plataforma, devuelve false
    return false;
}
void Jugador::actualizarFisica()
{
    enColisionConPlataforma = false;

    // Verificar la colisión con las plataformas
    for (QGraphicsItem* plataforma : plataformas) {
        if (collidesWithItem(plataforma)) {
            if (descendiendo) {
                descendiendo = false;
                continue;
            }

            enColisionConPlataforma = true;

            // Si el jugador está cayendo, ajustar su posición
            if (velocidadY > 0) {
                velocidadY = 0;
            }

            break; // No necesitamos verificar más plataformas
        }
    }

    // Actualizar el estado de "en el aire"
    enElAire = !enColisionConPlataforma;

    // Aplicar gravedad si está en el aire
    if (enElAire) {
        velocidadY += gravedad;
    }

    // Calcular nueva posición
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



void Jugador::actualizarImagen()
{
    if (enEstadoDeDisparo) return; // Salir si el jugador está disparando

    // Selecciona el conjunto de imágenes según la última dirección horizontal
    const QVector<QPixmap>& imagenesActuales = direccionDerecha ? imagenesDerecha : imagenesIzquierda;

    // Cambia el índice de la imagen en la dirección adecuada

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

    // Actualizar la imagen mostrada
    setPixmap(imagenesActuales[indiceImagen]);
}

void Jugador::setPlataformas(const QList<QGraphicsItem*>& plataformas) {
    this->plataformas = plataformas;
}

void Jugador::detectarColisionConProyectil() {
    QList<QGraphicsItem*> itemsColisionados = collidingItems();

    for (QGraphicsItem* item : itemsColisionados) {
        // Verifica si el objeto es un proyectil
        Proyectil* proyectil = dynamic_cast<Proyectil*>(item);

        if (proyectil) {
            // Si el proyectil fue disparado por el enemigo, reduce vidas del jugador
            if (proyectil->obtenerPropietario() != this) {  // El proyectil no fue disparado por el jugador
                vidas--;
                emit vidasCambiadas(vidas); // Emitir señal cuando las vidas cambien
                 qDebug() << "Vidas restantes: " << vidas;
                // Eliminar el proyectil
                scene()->removeItem(proyectil);
                delete proyectil;

                if (vidas <= 0) {
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("Game Over");
                    msgBox.setText("¡Perdio mi papa!");
                    msgBox.setIconPixmap(QPixmap(":/Imagenes1/perdio.png")); // Imagen grande
                    msgBox.exec();
                    QApplication::quit();
                    return;
                }
            }
        }
    }
}

void Jugador::dispararPiedraIzquierda() {
    if (!scene()) return;

    // Cambiar temporalmente a la imagen de disparo
    setPixmap(imagenDisparoIzquierda);

    // Crear un proyectil con este jugador como propietario
    Proyectil* piedra = new Proyectil(this);
    piedra->setPixmap(QPixmap(":/Imagenes1/PIEDRA.png").scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    piedra->setPos(x() - piedra->pixmap().width(), y() + pixmap().height() / 2 - piedra->pixmap().height() / 2);
    piedra->establecerDireccion(-1); // Dirección hacia la izquierda

    scene()->addItem(piedra);

    // Restaurar la actualización de la imagen después de un pequeño retraso
    QTimer::singleShot(500, this, [this]() {
        enEstadoDeDisparo = false; // Permitir que se reanude la animación
        actualizarImagen(); // Actualizar la imagen manualmente después del disparo
    });
}

void Jugador::dispararPiedraDerecha() {
    if (!scene()) return;

    // Cambiar temporalmente a la imagen de disparo
    setPixmap(imagenDisparoDerecha);

    // Crear un proyectil con este jugador como propietario
    Proyectil* piedra = new Proyectil(this);
    piedra->setPixmap(QPixmap(":/Imagenes1/PIEDRA.png").scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    piedra->setPos(x() + pixmap().width(), y() + pixmap().height() / 2 - piedra->pixmap().height() / 2);
    piedra->establecerDireccion(1); // Dirección hacia la derecha

    scene()->addItem(piedra);

    // Restaurar la actualización de la imagen después de un pequeño retraso
    QTimer::singleShot(500, this, [this]() {
        enEstadoDeDisparo = false; // Permitir que se reanude la animación
        actualizarImagen(); // Actualizar la imagen manualmente después del disparo
    });
}



