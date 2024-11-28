#ifndef PROYECTIL_H
#define PROYECTIL_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QGraphicsItem>

class Proyectil : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    explicit Proyectil(QGraphicsItem* propietario, QGraphicsItem* parent = nullptr);

    // Método para obtener el propietario del proyectil
    QGraphicsItem* obtenerPropietario() const { return propietario; }
    void establecerDireccion(int nuevaDireccion);
private slots:
    void mover();

private:
    QTimer* temporizadorMovimiento;
    int direccion;
    QGraphicsItem* propietario; // Quién disparó el proyectil
};

#endif // PROYECTIL_H


