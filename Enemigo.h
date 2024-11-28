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

protected slots:
    void detectarColisionConProyectil() override;
    void moverHaciaJugador();
    void disparar();
private:
    int vidaEnemigo;
    QTimer* temporizadorMovimiento;
    QTimer* temporizadorDisparo;
    QTimer* temporizadorvidaEnemigo;
};

#endif // ENEMIGO_H
