#ifndef ENEMIGONIVEL3_H
#define ENEMIGONIVEL3_H

#include "jugadornivel3.h"
#include <QGraphicsItem>
#include <QObject>
#include <QTimer>
#include <QPointF>

class EnemigoNivel3 : public JugadorNivel3 {
    Q_OBJECT
public:
    EnemigoNivel3(QGraphicsItem* parent = nullptr);

    void setJugador(JugadorNivel3* jugador); // Asigna el jugador a perseguir
    void actualizarFisica();
    void recibirDano(int dano);  // Método para recibir daño
    int obtenervidaE();

private slots:
    void perseguirJugador();
    void atacarJugador();

private:
    bool puedeMoverse(qreal nuevoX, qreal nuevoY);

    JugadorNivel3* jugador; // Referencia al jugador a perseguir
    QTimer* timerPersecucion; // Temporizador para actualizar el movimiento del enemigo
    QPixmap imagenAbajo;
    QPixmap imagenArriba;
    QPixmap imagenIzquierda;
    QPixmap imagenDerecha;
    QTimer* timerAtaque; // Temporizador para controlar los ataques

    const qreal rangoAtaque = 50; // Rango dentro del cual puede atacar al jugador

    int vida; // Vida del enemigo
};

#endif // ENEMIGONIVEL3_H
