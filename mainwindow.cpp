#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "jugador.h"
#include "Enemigo.h"
#include <QPixmap>
#include <QBrush>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "jugadornivel2.h"
#include "enemigonivel2.h"
#include "Animales.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(this)),
    emptyScene(new QGraphicsScene(this))
{
    ui->setupUi(this);
    player = new QMediaPlayer(this);
    videoWidget = new QVideoWidget(this);
    audioOutput=new QAudioOutput(this);
    player->setVideoOutput(videoWidget);
    player->setAudioOutput(audioOutput);


    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(videoWidget);
    setLayout(layout);
    videoWidget->setFixedSize(1280, 546);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::cancionTerminada);
    player->setSource(QUrl("qrc:/Imagenes1/badcops.mp4")); // Primera canción
    player->play();

    // Tamaño de la escena y ventana
    int sceneWidth = 1280;
    int sceneHeight = 546;
    this->resize(sceneWidth, sceneHeight); // Ajustar el tamaño de la ventana
    this->setFixedSize(sceneWidth, sceneHeight); // Fijar el tamaño para evitar cambios

    // Configuración del QGraphicsView
    ui->graphicsView->resize(sceneWidth, sceneHeight);
    scene->setSceneRect(0, 0, sceneWidth, sceneHeight);

    QPixmap background(":/Imagenes1/BobPatino_BartHalloween.jpg");
    scene->setBackgroundBrush(QBrush(background.scaled(sceneWidth, sceneHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Configuración de la escena vacía
    emptyScene->setSceneRect(0, 0, sceneWidth, sceneHeight);
    emptyScene->setBackgroundBrush(Qt::white);

    // Ocultar contraseñas en los campos
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_4->setEchoMode(QLineEdit::Password);

    // Conexión de botones
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::verificarInicio);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::registrarse);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    delete emptyScene;
}

void MainWindow::registrarse()
{
    QString usuario = ui->lineEdit->text();
    QString contrasena = ui->lineEdit_4->text();

    if (usuario.isEmpty() || contrasena.isEmpty()) {
        QMessageBox::warning(this, "Error", "Por favor, completa ambos campos.");
        return;
    }

    QFile archivo("Usuario.txt");
    if (archivo.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&archivo);
        out << usuario << " " << contrasena << "\n";
        archivo.close();

        QMessageBox::information(this, "Éxito", "Usuario registrado correctamente.");
        ui->lineEdit->clear();
        ui->lineEdit_4->clear();
    } else {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo.");
    }
}

bool MainWindow::verificarUsuario(const QString &usuario, const QString &contrasena)
{
    QFile archivo("Usuario.txt");
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo.");
        return false;
    }

    QTextStream in(&archivo);
    while (!in.atEnd()) {
        QString linea = in.readLine();
        QStringList partes = linea.split(" ");
        if (partes.size() == 2 && partes[0] == usuario && partes[1] == contrasena) {
            archivo.close();
            return true;
        }
    }

    archivo.close();
    return false;
}

void MainWindow::verificarInicio()
{
    QString usuario = ui->lineEdit_3->text();
    QString contrasena = ui->lineEdit_2->text();

    if (!verificarUsuario(usuario, contrasena)) {
        QMessageBox::warning(this, "Error", "Usuario o contraseña incorrectos.");
        return;
    }

    if (!ui->radioButtonNivel1->isChecked() && !ui->radioButtonNivel2->isChecked() && !ui->radioButtonNivel3->isChecked()) {
        QMessageBox::warning(this, "Error", "Por favor, selecciona un nivel para continuar.");
        return;
    }

    if (ui->radioButtonNivel1->isChecked()) {
        nivel1();
    } else if (ui->radioButtonNivel2->isChecked()) {
        nivel3();
    } else if (ui->radioButtonNivel3->isChecked()) {
        nivel2();
    }

}

void MainWindow::nivel1()
{

    // Eliminar los widgets de la interfaz original
    delete ui->pushButton;
    delete ui->pushButton_2;
    delete ui->lineEdit_2;
    delete ui->lineEdit_3;
    delete ui->label;
    delete ui->label_3;
    delete ui->label_5;
    delete ui->label_7;
    delete ui->label_2;
    delete ui->lineEdit;
    delete ui->label_6;
    delete ui->lineEdit_4;
    delete ui->label_4;
    delete ui->label_8;
    delete ui->radioButtonNivel1;
    delete ui->radioButtonNivel2;
    delete ui->radioButtonNivel3;


    player->setSource(QUrl("qrc:/Imagenes1/cinematica1.mp4"));
    reanudarVideo();
    player->play();
    connect(player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::cancionTerminada);

    ui->graphicsView->setScene(emptyScene);

    QPixmap nuevoFondo(":/Imagenes1/Fondo1");
    emptyScene->setBackgroundBrush(QBrush(nuevoFondo.scaled(emptyScene->width(), emptyScene->height(),
                                                            Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));

    // Crear y configurar el jugador
    Jugador *jugador = new Jugador();
    emptyScene->addItem(jugador);

    QList<QGraphicsItem*> plataformas = crearPlataformas(emptyScene);
    jugador->setPlataformas(plataformas);

    jugador->setPos(emptyScene->width() / 2 - jugador->pixmap().width() / 2,
                    emptyScene->height() / 2 - jugador->pixmap().height() / 2);
    jugador->setFlag(QGraphicsItem::ItemIsFocusable);
    jugador->setFocus();

    // Crear y añadir enemigos
    Enemigo *enemigo1 = new Enemigo();
    enemigo1->cargarImagen(":/Imagenes1/Bobpatinox");
    enemigo1->setPos(100, 100); // Posición inicial del enemigo
    emptyScene->addItem(enemigo1);
    enemigo1->setPlataformas(plataformas);

    // Añadir imágenes en la esquina superior izquierda
    QGraphicsPixmapItem *vidasIcono = new QGraphicsPixmapItem(QPixmap(":/Imagenes1/5 vidasBart.PNG"));
    vidasIcono->setPos(160, 30); // Posición cerca de la esquina superior izquierda
    emptyScene->addItem(vidasIcono);

    QGraphicsPixmapItem *bartIcono = new QGraphicsPixmapItem(QPixmap(":/Imagenes1/VidaBa.png"));
    bartIcono->setPixmap(bartIcono->pixmap().scaled(180, 170, Qt::KeepAspectRatio, Qt::SmoothTransformation)); // Cambiar tamaño
    bartIcono->setPos(103, 0); // A la derecha del icono de vidas
    emptyScene->addItem(bartIcono);

    QGraphicsPixmapItem *vidasIconoB = new QGraphicsPixmapItem(QPixmap(":/Imagenes1/20 vidas.png"));
    vidasIconoB->setPos(900, 10);
    emptyScene->addItem(vidasIconoB);

    QGraphicsPixmapItem *BobIcono = new QGraphicsPixmapItem(QPixmap(":/Imagenes1/Bobpatinovida.png"));
    BobIcono->setPixmap(BobIcono->pixmap().scaled(110, 170, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    BobIcono->setPos(1140, 0);
    emptyScene->addItem(BobIcono);

    connect(jugador, &Jugador::vidasCambiadas, [vidasIcono](int vidas) {
        QString ruta = QString(":/Imagenes1/%1 vidasBart.PNG").arg(vidas);
        vidasIcono->setPixmap(QPixmap(ruta).scaled(vidasIcono->pixmap().size(),
                                                   Qt::KeepAspectRatio, Qt::SmoothTransformation));
    });
    connect(enemigo1, &Enemigo::vidasCambiadas, [vidasIconoB](int vidaEnemigo) {
        if (vidaEnemigo % 5 == 0) { // Verificar si el número de vidas es múltiplo de 5
            QString ruta = QString(":/Imagenes1/%1 vidas.png").arg(vidaEnemigo);
            vidasIconoB->setPixmap(QPixmap(ruta).scaled(vidasIconoB->pixmap().size(),
                                                        Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }});
}
QList<QGraphicsItem*>
MainWindow::crearPlataformas(QGraphicsScene *scene)
{
    QList<QGraphicsItem*> plataformas;

    QGraphicsRectItem *plataforma1 = new QGraphicsRectItem(100, 407, 500, 0);
    plataforma1->setPen(QPen(Qt::NoPen));
    scene->addItem(plataforma1);
    plataformas.append(plataforma1);

    QGraphicsRectItem *plataforma2 = new QGraphicsRectItem(315, 445, 572, 0);
    plataforma2->setPen(QPen(Qt::NoPen));
    scene->addItem(plataforma2);
    plataformas.append(plataforma2);

    QGraphicsRectItem *plataforma3 = new QGraphicsRectItem(420, 214, 70, 0);//techo
    plataforma3->setPen(QPen(Qt::NoPen));
    scene->addItem(plataforma3);
    plataformas.append(plataforma3);

    QGraphicsRectItem *plataforma4 = new QGraphicsRectItem(100, 312, 380, 0);
    plataforma4->setPen(QPen(Qt::NoPen));
    scene->addItem(plataforma4);
    plataformas.append(plataforma4);
    QGraphicsRectItem *plataforma5 = new QGraphicsRectItem(792, 312, 485, 0);
    plataforma5->setPen(QPen(Qt::NoPen));
    scene->addItem(plataforma5);
    plataformas.append(plataforma5);

    QGraphicsRectItem *plataforma6 = new QGraphicsRectItem(970, 407, 320, 0);
    plataforma6->setPen(QPen(Qt::NoPen));
    scene->addItem(plataforma6);
    plataformas.append(plataforma6);

    QGraphicsRectItem *plataforma7 = new QGraphicsRectItem(185, 184, 144, 0);//techo
    plataforma7->setPen(QPen(Qt::NoPen));
    scene->addItem(plataforma7);
    plataformas.append(plataforma7);

    QGraphicsRectItem *plataforma8 = new QGraphicsRectItem(875, 135, 245, 0);//techo
    plataforma8->setPen(QPen(Qt::NoPen));
    scene->addItem(plataforma8);
    plataformas.append(plataforma8);

    QGraphicsRectItem *plataforma9 = new QGraphicsRectItem(1150, 195, 110, 0);//techo
    plataforma9->setPen(QPen(Qt::NoPen));
    scene->addItem(plataforma9);
    plataformas.append(plataforma9);
    return plataformas;
}
void MainWindow::mousePressEvent(QMouseEvent */*event*/)
{
    if (player) {
        // Detener la reproducción
        player->stop();

        // Liberar recursos
        //delete player;
        //player = nullptr;
        videoWidget->hide();
        emit player->mediaStatusChanged(QMediaPlayer::EndOfMedia);
        //delete audioOutput;
        //audioOutput = nullptr;
    }
}
void MainWindow::reanudarVideo()
{
    if (player) {
        // Mostrar el widget del video
        videoWidget->show();
        // Reanudar la reproducción
        player->play();

        qDebug() << "Video reanudado.";
    }
}
void MainWindow::nivel2()
{
    // Eliminar los widgets de la interfaz original
    delete ui->pushButton;
    delete ui->pushButton_2;
    delete ui->lineEdit_2;
    delete ui->lineEdit_3;
    delete ui->label;
    delete ui->label_3;
    delete ui->label_5;
    delete ui->label_7;
    delete ui->label_2;
    delete ui->lineEdit;
    delete ui->label_6;
    delete ui->lineEdit_4;
    delete ui->label_4;
    delete ui->label_8;
    delete ui->radioButtonNivel1;
    delete ui->radioButtonNivel2;
    delete ui->radioButtonNivel3;

    player->setSource(QUrl("qrc:/Imagenes1/cinematica3.mp4"));
    reanudarVideo();
    player->play();
    connect(player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::cancionTerminada);

    // Configurar la escena vacía para el nivel 2
    ui->graphicsView->setScene(emptyScene);

    // Ajustar las dimensiones de la escena
    emptyScene->setSceneRect(0, 0, 1280, 546);

    // Configurar el fondo de la escena
    QPixmap nuevoFondo(":/Imagenes1/Fondo 2.png");
    emptyScene->setBackgroundBrush(QBrush(nuevoFondo.scaled(emptyScene->sceneRect().width(),
                                                            emptyScene->sceneRect().height(),
                                                            Qt::IgnoreAspectRatio,
                                                            Qt::SmoothTransformation)));

    // Crear y añadir al jugador a la escena
    JugadorNivel2 *jugador = new JugadorNivel2();
    jugador->setLimites(200, 1050, 0,530); // Establecer límites del mapa
    jugador->setPos(640 - jugador->pixmap().width() / 2, // Centrar horizontalmente
                    546 - jugador->pixmap().height() - 50); // Posicionar cerca de la parte inferior
    emptyScene->addItem(jugador);

    // Configurar el jugador como el objeto enfocado para recibir eventos de teclado
    jugador->setFlag(QGraphicsItem::ItemIsFocusable);
    jugador->setFocus();

    // Crear y añadir al enemigo a la escena
    EnemigoNivel2 *enemigo = new EnemigoNivel2();

    // Configurar el enemigo con una imagen
    enemigo->cargarImagen(":/Imagenes1/Bobtirando.png"); // Cambia esta ruta por la ruta real de tu imagen de enemigo
    emptyScene->addItem(enemigo);

    // Inicializar la posición del enemigo después de agregarlo a la escena
    enemigo->cambiarPosicion();



    // Crear y agregar múltiples animales a la escena
    for (int i = 0; i < 5; ++i) {  // Cambiar 5 por la cantidad de animales que quieras en total
        Animales *animal = new Animales();  // Crea un nuevo animal (cocodrilo o pez aleatorio)
        animal->colocarAleatoriamente(200, 1050, 0, 530);  // Colocarlos aleatoriamente dentro de los límites
        emptyScene->addItem(animal);  // Añadir el an   imal a la escena

    }

    QTimer *victoryTimer = new QTimer(this);
    victoryTimer->setSingleShot(true); // Solo se dispara una vez
    connect(victoryTimer, &QTimer::timeout, [this, jugador]() {
        if (jugador->obtenerVidas() > 0) {
            QMessageBox::information(this, "Victoria", "¡Has ganado el nivel 2!");
        } else {
            QMessageBox::information(this, "Derrota", "No tienes vidas suficientes para continuar.");
        }
    });

    // Inicia el temporizador (60 segundos)
    victoryTimer->start(5000);

}


void MainWindow::reproducirSiguienteCancion() {
    static int currentIndex = 0;
    QList<QString> listaCanciones = {
         ":/Imagenes1/shideshow.mp3",
        ":/Imagenes1/cancionbarco.mp3",
        ":/Imagenes1/capefear.mp3"

    };

    currentIndex = (currentIndex + 1) % listaCanciones.size(); // Avanzar al siguiente índice

    player->setSource(QUrl("qrc" + listaCanciones[currentIndex])); // Cambiar la canción
    player->play(); // Reproducir la siguiente canción
}


void MainWindow::cancionTerminada(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::EndOfMedia) {
        reproducirSiguienteCancion();
    }
}
void MainWindow::nivel3() {
    // Eliminar los widgets de la interfaz original
    delete ui->pushButton;
    delete ui->pushButton_2;
    delete ui->lineEdit_2;
    delete ui->lineEdit_3;
    delete ui->label;
    delete ui->label_3;
    delete ui->label_5;
    delete ui->label_7;
    delete ui->label_2;
    delete ui->lineEdit;
    delete ui->label_6;
    delete ui->lineEdit_4;
    delete ui->label_4;
    delete ui->label_8;
    delete ui->radioButtonNivel1;
    delete ui->radioButtonNivel2;
    delete ui->radioButtonNivel3;

    // Inicia el reproductor de video
    player->setSource(QUrl("qrc:/Imagenes1/cinematica2.mp4"));
    reanudarVideo();
    player->play();
    connect(player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::cancionTerminada);

    ui->graphicsView->setScene(emptyScene);

    // Cargar un fondo específico para el nivel 3
    QPixmap nuevoFondo(":/Imagenes1/fondoNivel3.jpg");
    emptyScene->setBackgroundBrush(QBrush(nuevoFondo.scaled(emptyScene->width(), emptyScene->height(),
                                                            Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));

}


