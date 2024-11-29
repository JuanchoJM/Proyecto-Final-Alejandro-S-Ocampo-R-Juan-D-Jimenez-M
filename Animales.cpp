#include "Animales.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QRandomGenerator>
#include <QTimer>

Animales::Animales(QGraphicsItem *parent) : QObject(), QGraphicsPixmapItem(parent)
{
    // Inicialización del tipo de animal aleatorio entre 2 opciones
    int aleatorio = QRandomGenerator::global()->bounded(2);  // Aleatorio entre 0, 1
    if (aleatorio == 0)
        tipoAnimal = "cocodrilo";
    else
        tipoAnimal = "pez";  // Puedes agregar más animales si lo deseas

    // Configurar el temporizador de movimiento aleatorio
    temporizadorMovimiento = new QTimer(this);
    connect(temporizadorMovimiento, &QTimer::timeout, this, &Animales::moverAleatoriamente);
    temporizadorMovimiento->start(5000);  // Mueve cada 1 segundo (ajustable)
}

void Animales::cargarImagen(const QString &rutaImagen)
{
    QPixmap pixmap(rutaImagen);
    setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));  // Ajustar tamaño de la imagen
}

void Animales::colocarAleatoriamente(int limiteIzquierdo, int limiteDerecho, int limiteSuperior, int limiteInferior)
{
    // Generar posiciones aleatorias dentro de los límites
    int xAleatorio = QRandomGenerator::global()->bounded(limiteIzquierdo, limiteDerecho + 1);
    int yAleatorio = QRandomGenerator::global()->bounded(limiteSuperior, limiteInferior + 1);

    setPos(xAleatorio, yAleatorio);  // Colocar al animal en la posición aleatoria

    // Dependiendo del tipo de animal, cargar una imagen diferente
    if (tipoAnimal == "cocodrilo") {
        cargarImagen(":/Imagenes1/Cocodrilo.png");  // Ruta de imagen para el cocodrilo
    } else if (tipoAnimal == "pez") {
        cargarImagen(":/Imagenes1/pez.png");   // Ruta de imagen para el pez
    }
}

void Animales::moverAleatoriamente()
{
    // Generar una nueva posición aleatoria dentro de los límites
    int xAleatorio = QRandomGenerator::global()->bounded(200, 800);  // Limites del jugador
    int yAleatorio = QRandomGenerator::global()->bounded(0, 530);     // Limites del jugador

    setPos(xAleatorio, yAleatorio);  // Actualizar la posición aleatoria
}
