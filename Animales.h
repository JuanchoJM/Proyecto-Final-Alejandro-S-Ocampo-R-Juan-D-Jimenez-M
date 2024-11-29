#ifndef ANIMALES_H
#define ANIMALES_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QRandomGenerator>

class Animales : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Animales(QGraphicsItem *parent = nullptr);

    void cargarImagen(const QString &rutaImagen);
    void colocarAleatoriamente(int limiteIzquierdo, int limiteDerecho, int limiteSuperior, int limiteInferior);
    void moverAleatoriamente();


private:
    QString tipoAnimal;  // Tipo de animal (cocodrilo, pez, etc.)
    QTimer *temporizadorMovimiento;  // Temporizador para mover el animal aleatoriamente
};

#endif // ANIMALES_H
