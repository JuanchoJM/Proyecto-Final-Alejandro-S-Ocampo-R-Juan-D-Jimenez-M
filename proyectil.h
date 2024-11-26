#ifndef PROYECTIL_H
#define PROYECTIL_H
#include <QGraphicsItem>
#include <QTimer>
#include <QObject>
#include <QGraphicsPixmapItem>

class Proyectil : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    explicit Proyectil(QGraphicsItem* propietario, QGraphicsItem* parent = nullptr);

    // Método para obtener el propietario del proyectil
    QGraphicsItem* obtenerPropietario() const { return propietario; }

private slots:
    void mover();

private:
    QTimer* temporizadorMovimiento;
    int direccion;
    QGraphicsItem* propietario; // Quién disparó el proyectil
};

#endif // PROYECTIL_H
