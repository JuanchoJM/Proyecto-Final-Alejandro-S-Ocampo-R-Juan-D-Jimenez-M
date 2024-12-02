#ifndef OBJETO_H
#define OBJETO_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QPixmap>

class Objeto : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    // Constructor que carga la imagen inicial
    explicit Objeto(const QString& rutaImagen, QGraphicsItem* parent = nullptr);

    // Método para establecer la imagen del objeto
    void setRutaImagen(const QString& ruta);

    // Método para verificar si el jugador está cerca del objeto
    bool estaCerca(const QGraphicsPixmapItem* jugador, int distancia) const;

    // Método para cambiar la imagen del objeto cuando el jugador esté cerca
    void cambiarImagenSiCerca(const QGraphicsPixmapItem* jugador, int distancia, const QString& nuevaRuta);

private:
    QPixmap imagenObjeto; // Imagen del objeto
};

#endif // OBJETO_H
