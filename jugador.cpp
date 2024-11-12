#include "jugador.h"
#include <QBrush>

Jugador::Jugador(QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent), indiceImagen(0), avanzando(true), direccionDerecha(true)
{
    // Dimensiones deseadas para las imágenes redimensionadas
    int anchoDeseado = 200; // Ajusta este valor según tus necesidades
    int altoDeseado = 200;  // Ajusta este valor según tus necesidades

    // Cargar y redimensionar las ocho imágenes para la animación de caminar hacia la derecha
    for (int i = 1; i <= 8; ++i) {
        QPixmap imagen = QPixmap(QString("C:/Users/PC/Documents/ProyectoFinal/BartD%1.png").arg(i));
        imagenesDerecha.append(imagen.scaled(anchoDeseado, altoDeseado, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    // Cargar y redimensionar las siete imágenes para la animación de caminar hacia la izquierda
    for (int i = 1; i <= 7; ++i) {
        QPixmap imagen = QPixmap(QString("C:/Users/PC/Documents/ProyectoFinal/BartI%1.png").arg(i));
        imagenesIzquierda.append(imagen.scaled(anchoDeseado, altoDeseado, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    // Configura la primera imagen (dirección derecha por defecto)
    setPixmap(imagenesDerecha[indiceImagen]);

    // Iniciar el temporizador para la animación
    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, &Jugador::actualizarImagen);
}
void Jugador::keyPressEvent(QKeyEvent* event)
{
    int step = 10;
    bool movimientoHorizontal = false;

    if (event->key() == Qt::Key_Right) {
        setX(x() + step);
        direccionDerecha = true;


    } else if (event->key() == Qt::Key_Left) {
        setX(x() - step);
        direccionDerecha = false;

    } else if (event->key() == Qt::Key_Up) {
        setY(y() - step);

    } else if (event->key() == Qt::Key_Down) {
        setY(y() + step);
    }

    // Si no está activo el temporizador, iniciar la animación
    if (!temporizador->isActive()) {
        temporizador->start(100); // Intervalo de cambio de imagen en milisegundos
    }

    // Si hubo un cambio en la dirección horizontal, reiniciar el índice de la imagen
    if (movimientoHorizontal) {
        indiceImagen = 0;
        avanzando = true;
    }
}

void Jugador::actualizarImagen()
{
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

