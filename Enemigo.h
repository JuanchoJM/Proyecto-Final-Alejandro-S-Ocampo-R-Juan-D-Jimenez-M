#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "jugador.h"
#include <QObject>
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsPixmapItem>


class Enemigo : public Jugador {
    Q_OBJECT

public:
    explicit Enemigo(QGraphicsItem* parent = nullptr);
    ~Enemigo();
    void cargarImagen(const QString& rutaImagen);
    void iniciarMovimiento();
    void detectarColisionConProyectil();

protected slots:
    void moverAleatorio();
    void disparar();

private:
    QTimer* temporizadorMovimiento;
    QTimer* temporizadorDisparo;
    int vidaEnemigo;
};

#endif // ENEMIGO_H
