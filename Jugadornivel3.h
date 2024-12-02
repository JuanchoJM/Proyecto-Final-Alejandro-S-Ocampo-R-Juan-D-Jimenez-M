#ifndef JUGADORNIVEL3_H
#define JUGADORNIVEL3_H

#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QTimer>
#include <QObject>
#include <QList>
#include <QGraphicsItem>

// Declaración adelantada de la clase EnemigoNivel3
class EnemigoNivel3;

class JugadorNivel3 : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    JugadorNivel3(QGraphicsItem* parent = nullptr);

    void setPlataformas(const QList<QGraphicsItem*>& plataformas);
    void setObjetosInteractivos(const QList<QGraphicsItem*>& objetos);
    void agregarObjetoInteractivo(QGraphicsItem* objeto);

    // Métodos para manejar el movimiento
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void actualizarFisica();
    void reducirVidas(int cantidad);
    int obtenerVidas() ;
    bool tieneObjeto() const;
    bool objetocoger;
    void interactuarConEnemigo(EnemigoNivel3* enemigo);  // Usando puntero a EnemigoNivel3
    void setEnemigo(EnemigoNivel3* enemigo);

private slots:
    void actualizarAnimacion();
    void interactuarConObjeto();

protected:
    QTimer* temporizadorFisica;
    QList<QGraphicsItem*> plataformas;
    QList<QGraphicsItem*> objetosInteractivos;
    int velocidad;

private:
    void moverJugador();
    void cambiarImagen();
    bool estaCercaDe(QGraphicsItem* objeto, int distancia);

    int dx, dy;
    QVector<QPixmap> animacionDerecha;
    QVector<QPixmap> animacionIzquierda;
    QVector<QPixmap> animacionArriba;
    QVector<QPixmap> animacionAbajo;
    int indiceAnimacion;
    QTimer* timerAnimacion;
    QString ultimaDireccion;
    QTimer* timerMovimiento;
    int vidas;
    EnemigoNivel3* enemigo;
};

#endif // JUGADORNIVEL3_H
