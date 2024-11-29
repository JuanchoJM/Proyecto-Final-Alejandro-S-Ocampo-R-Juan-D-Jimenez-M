#ifndef PROYECTILNIVEL2_H
#define PROYECTILNIVEL2_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QRandomGenerator>

class ProyectilNivel2 : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit ProyectilNivel2(QGraphicsItem *parent = nullptr); // Constructor declarado aquí

signals:
    void fueraDeEscena(); // Señal para notificar que el proyectil salió de la escena

private slots:
    void mover(); // Slot para mover el proyectil

private:
    QTimer *temporizador; // Temporizador para mover el proyectil
    QString tipoProyectil; // Tipo de proyectil ("piedra" o "tronco")
};

#endif // PROYECTILNIVEL2_H
