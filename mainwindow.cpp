#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QBrush>
#include "jugador.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new QGraphicsScene(this))       // Primera escena
    , emptyScene(new QGraphicsScene(this))  // Nueva escena vacía
{
    ui->setupUi(this);

    // Configuración de la primera escena con imagen de fondo
    int sceneWidth = 1280;
    int sceneHeight = 546;
    scene->setSceneRect(0, 0, sceneWidth, sceneHeight);

    QPixmap background("C:/Users/PC/Documents/ProyectoFinal/BobPatino_BartHalloween.jpg");
    QPixmap scaledBackground = background.scaled(sceneWidth, sceneHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    scene->setBackgroundBrush(QBrush(scaledBackground));

    // Usamos el QGraphicsView de la interfaz .ui
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Configuración de la nueva escena vacía (blanca)
    emptyScene->setSceneRect(0, 0, sceneWidth, sceneHeight);
    emptyScene->setBackgroundBrush(Qt::white);  // Fondo blanco

    // Conectar el botón de "Start" para cambiar de escena
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::startGame);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;  // Eliminar la escena original
    delete emptyScene;  // Eliminar la nueva escena vacía
}
void MainWindow::startGame()
{
    // Eliminar los widgets de la interfaz original
    delete ui->pushButton;
    delete ui->pushButton_2;
    delete ui->lineEdit_2;
    delete ui->lineEdit_3;
    delete ui->label;
    delete ui->label_3;
    delete ui->label_5;

    // Cambiar la escena del QGraphicsView a la nueva escena
    ui->graphicsView->setScene(emptyScene);

    // Configurar el nuevo fondo para la escena después de presionar "Start"
    QPixmap nuevoFondo("C:/Users/PC/Documents/ProyectoFinal/Fondo1.jpeg"); // Cambia esta ruta por la de tu imagen
    QPixmap scaledNuevoFondo = nuevoFondo.scaled(emptyScene->width(), emptyScene->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    emptyScene->setBackgroundBrush(QBrush(scaledNuevoFondo));

    // Crear y agregar el jugador a la escena
    Jugador* jugador = new Jugador();
    emptyScene->addItem(jugador);

    // Colocar el jugador en el centro de la escena usando pixmap().width() y pixmap().height()
    jugador->setPos(emptyScene->width() / 2 - jugador->pixmap().width() / 2,
                    emptyScene->height() / 2 - jugador->pixmap().height() / 2);

    // Permitir que el jugador reciba eventos de teclado
    jugador->setFlag(QGraphicsItem::ItemIsFocusable);
    jugador->setFocus();
}
