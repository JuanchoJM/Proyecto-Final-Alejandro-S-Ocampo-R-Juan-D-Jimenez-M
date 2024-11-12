#ifndef JUGADOR_H
#define JUGADOR_H

#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QTimer>
#include <QPixmap>
#include <QObject>

class Jugador : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT  // Necesario para señales y ranuras

public:
    // Constructor del jugador
    Jugador(QGraphicsItem* parent = nullptr);

    // Método para manejar eventos de teclado
    void keyPressEvent(QKeyEvent* event);

private slots:
    void actualizarImagen();

private:
    // Imágenes para la animación hacia la derecha e izquierda
    QVector<QPixmap> imagenesDerecha;
    QVector<QPixmap> imagenesIzquierda;

    int indiceImagen;           // Índice de la imagen actual
    bool avanzando;             // Dirección de la animación (true = avanzar, false = retroceder)
    bool direccionDerecha;      // Última dirección horizontal (true = derecha, false = izquierda)
    QTimer* temporizador;       // Temporizador para cambiar las imágenes
};

#endif // JUGADOR_H
