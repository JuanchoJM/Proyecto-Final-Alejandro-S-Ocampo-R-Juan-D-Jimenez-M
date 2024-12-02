#include "JugadorNivel3.h"
#include "Objeto.h"
#include <QKeyEvent>
#include <QTimer>
#include <QGraphicsScene>
#include <QMessageBox>
#include "enemigonivel3.h"
JugadorNivel3::JugadorNivel3(QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent), velocidad(3), vidas(5), dx(0), dy(0), indiceAnimacion(0),ultimaDireccion("derecha") {
    // Cargar imágenes de animación
    for (int i = 1; i <= 8; i++) {
        animacionDerecha.append(QPixmap(":/Imagenes/Bartderecha.png"));
        animacionIzquierda.append(QPixmap(":/Imagenes/BartIzquierda.png"));
        animacionArriba.append(QPixmap(":/Imagenes/BartArriba.png"));
        animacionAbajo.append(QPixmap(":/Imagenes/BartAbajo.png" ));
    }

    setPixmap(animacionDerecha[0]); // Imagen inicial

    // Configurar temporizador para la animación
    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &JugadorNivel3::actualizarAnimacion);

    // Configurar temporizador para el movimiento
    timerMovimiento = new QTimer(this);
    connect(timerMovimiento, &QTimer::timeout, this, &JugadorNivel3::moverJugador);
    timerMovimiento->start(16); // Aproximadamente 60 FPS
    temporizadorFisica = new QTimer(this);
    connect(temporizadorFisica, &QTimer::timeout, this, &JugadorNivel3::actualizarFisica);
    temporizadorFisica->start(30); // Aproximadamente 60 FPS
}

void JugadorNivel3::setPlataformas(const QList<QGraphicsItem*>& plataformas) {
    this->plataformas = plataformas;
}

void JugadorNivel3::setObjetosInteractivos(const QList<QGraphicsItem*>& objetos) {
    this->objetosInteractivos = objetos;
}

void JugadorNivel3::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_Right:
        dx = velocidad;
        dy = 0;
        ultimaDireccion = "derecha";
        timerAnimacion->start(100);
        break;
    case Qt::Key_Left:
        dx = -velocidad;
        dy = 0;
        ultimaDireccion = "izquierda";
        timerAnimacion->start(100);
        break;
    case Qt::Key_Up:
        dx = 0;
        dy = -velocidad;
        if (ultimaDireccion.isEmpty()) ultimaDireccion = "derecha";
        timerAnimacion->start(100);
        break;
    case Qt::Key_Down:
        dx = 0;
        dy = velocidad;
        if (ultimaDireccion.isEmpty()) ultimaDireccion = "derecha";
        timerAnimacion->start(100);
        break;
    case Qt::Key_F:
        // Interactuar con objetos
        interactuarConObjeto();
        break;
    case Qt::Key_Space:
        // Interactuar con el enemigo si está cerca y si tiene un objeto
        interactuarConEnemigo(enemigo);  // Asumiendo que tienes una referencia al enemigo
        break;
    }
}


void JugadorNivel3::keyReleaseEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_Right:
    case Qt::Key_Left:
    case Qt::Key_Up:
    case Qt::Key_Down:
        dx = 0;
        dy = 0;
        timerAnimacion->stop();
        indiceAnimacion = 0;
        cambiarImagen(); // Detener animación en la imagen inicial
        break;
    }
}

void JugadorNivel3::actualizarAnimacion() {
    // Actualizar índice de animación
    indiceAnimacion = (indiceAnimacion + 1) % animacionDerecha.size();
    cambiarImagen();
}

void JugadorNivel3::cambiarImagen() {
    if (dx > 0) {
        setPixmap(animacionDerecha[indiceAnimacion]);
    } else if (dx < 0) {
        setPixmap(animacionIzquierda[indiceAnimacion]);
    } else if (dy > 0) { // Movimiento hacia abajo
        setPixmap(animacionAbajo[indiceAnimacion]);
    } else if (dy < 0) { // Movimiento hacia arriba
        setPixmap(animacionArriba[indiceAnimacion]);
    }
}

void JugadorNivel3::moverJugador() {
    // Calcular nueva posición
    qreal nuevoX = x() + dx;
    qreal nuevoY = y() + dy;

    // Limitar al rango de la pantalla (1280x546)
    if (nuevoX < 100) nuevoX = 100; // Límite izquierdo
    if (nuevoX + pixmap().width() > 1280) nuevoX = 1280 - pixmap().width(); // Límite derecho
    if (nuevoY < 0) nuevoY = 0; // Límite superior
    if (nuevoY + pixmap().height() > 600) nuevoY = 600 - pixmap().height(); // Límite inferior

    // Actualizar la posición del jugador
    setPos(nuevoX, nuevoY);
}

void JugadorNivel3::actualizarFisica() {
    // Definir las dimensiones del jugador (esto es fijo)
    const int jugadorWidth = 50;  // Ancho del jugador
    const int jugadorHeight = 50; // Altura del jugador

    // Verificar colisión con las plataformas
    for (QGraphicsItem* plataforma : plataformas) {
        if (collidesWithItem(plataforma)) {
            // Obtener el rectángulo de la plataforma
            QRectF plataformaRect = plataforma->boundingRect().translated(plataforma->pos());

            // Detectar y manejar el lado de colisión
            if (y() + jugadorHeight > plataformaRect.top() && dy > 0) {
                // Colisión por abajo
                setPos(x(), plataformaRect.top() - jugadorHeight );
                dy = 0;
            } else if (y() < plataformaRect.bottom() && dy < 0) {
                // Colisión por arriba
                setPos(x(), plataformaRect.bottom());
                dy = 0;
            } else if (x() + jugadorWidth > plataformaRect.left() && dx > 0) {
                // Colisión por la derecha
                setPos(plataformaRect.left() - jugadorWidth , y());
                dx = 0;
            } else if (x() < plataformaRect.right() && dx < 0) {
                // Colisión por la izquierda
                setPos(plataformaRect.right(), y());
                dx = 0;
            }
            return; // Salir del bucle tras manejar la colisión
        }
    }

    // Actualizar la posición del jugador si no hay colisiones
    setPos(x() + dx, y() + dy);
}
bool JugadorNivel3::estaCercaDe(QGraphicsItem* objeto, int distancia) {
    // Obtener las posiciones del jugador y del objeto
    QPointF posJugador = pos();
    QPointF posObjeto = objeto->pos();

    // Calcular la distancia entre los centros de los objetos
    int dx = posJugador.x() - posObjeto.x();
    int dy = posJugador.y() - posObjeto.y();
    int distanciaCalculada = std::sqrt(dx * dx + dy * dy);

    // Comparar la distancia calculada con el umbral
    return distanciaCalculada <= distancia;
}
void JugadorNivel3::reducirVidas(int cantidad) {
    vidas -= cantidad;
    if (vidas <= 0) {
        vidas = 0;
        QMessageBox::information(nullptr, "Derrota", "¡Perdiste el juego!");
        // Opcional: Finalizar el juego o reiniciar
        exit(0); // Salir del juego
    }
}
int JugadorNivel3::obtenerVidas()  {
    return vidas;
}


void JugadorNivel3::interactuarConObjeto() {
    for (QGraphicsItem* objeto : objetosInteractivos) {
        if (estaCercaDe(objeto, 100)) {
            // Desactivar la interacción con el objeto (ya no será parte de los objetos interactivos)
            objetosInteractivos.removeOne(objeto);
            QGraphicsPixmapItem* nuevaImagen = new QGraphicsPixmapItem(QPixmap(":/Imagenes/Tienesobjeto.png"));
            nuevaImagen->setPos(500, 0);  // Ubicación arbitraria
            scene()->addItem(nuevaImagen);
            objetocoger=true;

            // Eliminar el objeto de la escena y de la lista de objetos interactivos
            scene()->removeItem(objeto);
            delete objeto;  // Liberar memoria
            qDebug() << "Interacción exitosa con el objeto.";
            break;  // Salir del bucle una vez que se haya agarrado un objeto
        }
    }
}
void JugadorNivel3::interactuarConEnemigo(EnemigoNivel3* enemigo) {
    if (enemigo == nullptr) {
        qDebug() << "El enemigo no está inicializado.";
        return; // Evitar interacción si el enemigo no está definido
    }

    // Comprobar si el jugador tiene un objeto
    if (tieneObjeto()) {
        // Verificar si el jugador está cerca del enemigo (dentro del rango de ataque)
        if (estaCercaDe(enemigo, 100)) {  // 50 es el rango de ataque
            enemigo->recibirDano(1);  // Resta 10 de vida al enemigo
            qDebug() << "Ataque exitoso al enemigo.";
            if (enemigo->obtenervidaE() <= 0) {
                QMessageBox::information(nullptr, "Victoria", "¡Ganaste el juego!");
                qDebug() << "WIN ";

            }

        } else {
            qDebug() << "El enemigo está fuera de rango.";
        }
    } else {
        qDebug() << "El jugador no tiene un objeto para atacar.";
    }
}

bool JugadorNivel3::tieneObjeto() const {
    return objetocoger;  // Retorna verdadero si el jugador tiene un objeto
}
void JugadorNivel3::setEnemigo(EnemigoNivel3* enemigo) {
    this->enemigo = enemigo;
}
