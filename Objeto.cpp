#include "Objeto.h"
#include <QPixmap>
#include <cmath>

// Constructor que carga la imagen inicial
Objeto::Objeto(const QString& rutaImagen, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent) {
    setRutaImagen(rutaImagen); // Cargar la imagen inicial
}

// Método para establecer la imagen del objeto
void Objeto::setRutaImagen(const QString& ruta) {
    imagenObjeto = QPixmap(ruta);
    setPixmap(imagenObjeto);  // Establecer la imagen en el objeto
}

// Método para verificar si el jugador está cerca del objeto
bool Objeto::estaCerca(const QGraphicsPixmapItem* jugador, int distancia) const {
    // Calcular la distancia entre el objeto y el jugador usando la distancia euclidiana
    int dx = x() - jugador->x();
    int dy = y() - jugador->y();
    int distanciaCalculada = std::sqrt(dx * dx + dy * dy);

    // Compara la distancia calculada con el umbral
    return distanciaCalculada <= distancia;
}

// Método para cambiar la imagen del objeto si el jugador está cerca
void Objeto::cambiarImagenSiCerca(const QGraphicsPixmapItem* jugador, int distancia, const QString& nuevaRuta) {
    if (estaCerca(jugador, distancia)) {
        setRutaImagen(nuevaRuta); // Cambiar la imagen si el jugador está cerca
    }
}
