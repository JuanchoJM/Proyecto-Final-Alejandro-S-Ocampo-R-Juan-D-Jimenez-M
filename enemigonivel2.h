#ifndef ENEMIGONIVEL2_H
#define ENEMIGONIVEL2_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>


class EnemigoNivel2 : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit EnemigoNivel2(QGraphicsItem *parent = nullptr);

    void cargarImagen(const QString &rutaImagen);
    void cambiarPosicion();

private slots:

    void dispararProyectil();

private:
    QTimer *temporizadorPosicion; // Temporizador para cambiar posición
    QTimer *temporizadorDisparo;  // Temporizador para disparar
};

#endif // ENEMIGONIVEL2_H
