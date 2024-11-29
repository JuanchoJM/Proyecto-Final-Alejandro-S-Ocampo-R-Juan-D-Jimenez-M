#include "jugadornivel2.h"
#include <QMessageBox>
#include "Animales.h"
// Constructor
JugadorNivel2::JugadorNivel2(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent),
    velocidad(10),
    limiteIzquierdo(500),
    limiteDerecho(700),
    limiteSuperior(0),
    limiteInferior(546), // Configuración inicial de los límites del mapa
    vidas(5)
{
    // Escalar la imagen en el constructor
    QPixmap pixmap(":/Imagenes1/Bartremando.png"); // Ruta de la imagen
    pixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation); // Ajusta el tamaño de la imagen
    setPixmap(pixmap); // Establece la imagen escalada en el jugador
}

// Configurar límites de movimiento
void JugadorNivel2::setLimites(int limiteIzquierdo, int limiteDerecho, int limiteSuperior, int limiteInferior)
{
    this->limiteIzquierdo = limiteIzquierdo;
    this->limiteDerecho = limiteDerecho;
    this->limiteSuperior = limiteSuperior;
    this->limiteInferior = limiteInferior;
}

// Manejo de teclas para el movimiento
void JugadorNivel2::keyPressEvent(QKeyEvent *event)
{
    // Verificar si se presiona la tecla de flecha derecha
    if (event->key() == Qt::Key_Right) {
        int nuevaPosicion = x() + velocidad; // Calcular nueva posición
        if (nuevaPosicion + pixmap().width() <= limiteDerecho) { // Verificar el límite derecho
            setX(nuevaPosicion); // Mover a la derecha
        }
    }

    // Verificar si se presiona la tecla de flecha izquierda
    else if (event->key() == Qt::Key_Left) {
        int nuevaPosicion = x() - velocidad; // Calcular nueva posición
        if (nuevaPosicion >= limiteIzquierdo) { // Verificar el límite izquierdo
            setX(nuevaPosicion); // Mover a la izquierda
        }
    }

    // Verificar si se presiona la tecla de flecha arriba
    else if (event->key() == Qt::Key_Up) {
        int nuevaPosicion = y() - velocidad; // Calcular nueva posición
        if (nuevaPosicion >= limiteSuperior) { // Verificar el límite superior
            setY(nuevaPosicion); // Mover hacia arriba
        }
    }

    // Verificar si se presiona la tecla de flecha abajo
    else if (event->key() == Qt::Key_Down) {
        int nuevaPosicion = y() + velocidad; // Calcular nueva posición
        if (nuevaPosicion + pixmap().height() <= limiteInferior) { // Verificar el límite inferior
            setY(nuevaPosicion); // Mover hacia abajo
        }
    }
    detectarColisiones();
}
void JugadorNivel2::reducirVida() {
    // Evitar que las vidas sean negativas
    if (vidas <= 0) {
        return; // Si ya no hay vidas, no hacer nada
    }

    vidas--;  // Reducir una vida
    //emit vidasCambiadas(vidas); // Emitir señal cuando las vidas cambien
    qDebug() << "Vidas restantes:" << vidas;

    // Verificar si ya hemos mostrado el mensaje de "Has perdido"
    static bool mensajeMostrado = false;  // Bandera estática para asegurar que el mensaje solo se muestre una vez

    if (vidas <= 0 && !mensajeMostrado) {
        // Mostrar un mensaje de pérdida solo la primera vez que se pierden todas las vidas
        QMessageBox::information(nullptr, "Juego Terminado", "¡Has perdido!");

        mensajeMostrado = true;  // Marcar que el mensaje ya ha sido mostrado

        // Aquí puedes terminar el juego, por ejemplo, cerrando la ventana
        // QApplication::quit();
    }
}
void JugadorNivel2::detectarColisiones() {
    // Obtener los elementos con los que colisiona el jugador
    QList<QGraphicsItem *> elementosColisionados = collidingItems();

    // Recorrer los elementos colisionados
    for (QGraphicsItem *elemento : elementosColisionados) {
        // Intentar convertir el elemento en un animal
        Animales *animal = dynamic_cast<Animales *>(elemento);

        if (animal) {  // Si es un animal
            reducirVida();  // Reducir una vida al jugador


            // Opcional: Eliminar al animal después de la colisión

            delete animal;

            break;  // Salir del bucle después de manejar una colisión
        }
    }
}
int JugadorNivel2::obtenerVidas() const
{
    return vidas;
}
