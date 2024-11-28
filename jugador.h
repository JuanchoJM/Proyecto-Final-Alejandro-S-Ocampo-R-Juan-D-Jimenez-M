#ifndef JUGADOR_H
#define JUGADOR_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QTimer>
#include <QVector>
class Jugador : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    explicit Jugador(QGraphicsItem *parent = nullptr);
    void setPlataformas(const QList<QGraphicsItem*>& plataformas);
    bool detectarColisionConPlataformas();
    // Métodos de disparo
    void dispararPiedraIzquierda();
    void dispararPiedraDerecha();
private:
     int vidas;
    QTimer *temporizadorvidas;
signals:
    void vidasCambiadas(int vidas); // Señal para notificar cambio de vidas
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    QVector<QPixmap> imagenesDerecha;
    QVector<QPixmap> imagenesIzquierda;
    int indiceImagen;
    bool avanzando;
    bool direccionDerecha;
    bool descendiendo;

    // Física
    float velocidadX;
    float velocidadY;
    float gravedad;
    bool enElAire;
    bool enColisionConPlataforma;
    QList<QGraphicsItem*> plataformas;
    // Temporizadores
    QTimer *temporizador;
    QTimer *temporizadorFisica;
protected slots:
    void actualizarFisica();
    void actualizarImagen();
    virtual void detectarColisionConProyectil();
};

#endif // JUGADOR_H
