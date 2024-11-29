#ifndef JUGADORNIVEL2_H
#define JUGADORNIVEL2_H

#include <QGraphicsPixmapItem>
#include <QKeyEvent>

class JugadorNivel2 : public QGraphicsPixmapItem
{
public:
    // Constructor
    JugadorNivel2(QGraphicsItem *parent = nullptr);

    // Configurar límites del mapa
    void setLimites(int limiteIzquierdo, int limiteDerecho, int limiteSuperior, int limiteInferior);

    // Reducir la vida del jugador
    void reducirVida();

    // Obtener las vidas actuales
    int obtenerVidas() const;
    void verificarColisiones();
    void detectarColisiones();
protected:
    // Evento de teclado para movimiento
    void keyPressEvent(QKeyEvent *event) override;

private:
    int velocidad;         // Velocidad de movimiento del jugador
    int limiteIzquierdo;   // Límite izquierdo del mapa
    int limiteDerecho;     // Límite derecho del mapa
    int limiteSuperior;    // Límite superior del mapa
    int limiteInferior;    // Límite inferior del mapa
    int vidas;             // Cantidad de vidas del jugador
};

#endif // JUGADORNIVEL2_H
