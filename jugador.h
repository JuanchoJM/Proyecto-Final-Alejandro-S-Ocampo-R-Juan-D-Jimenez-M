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

    // Nuevo método para detectar colisiones con plataformas
    bool detectarColisionConPlataformas();
    void detectarColisionConProyectil();


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
    int vidas;

protected slots:
    void actualizarFisica();
    void actualizarImagen();






};


#endif // JUGADOR_H
