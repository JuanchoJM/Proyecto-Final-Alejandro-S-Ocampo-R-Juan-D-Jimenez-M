#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "jugador.h"
#include "Enemigo.h"
#include <QPixmap>
#include <QBrush>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(this)),
    emptyScene(new QGraphicsScene(this))
{
    ui->setupUi(this);

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
        QMessageBox::information(this, "Nivel 2", "Nivel 2 aún no implementado.");
        // Implementar nivel 2
    } else if (ui->radioButtonNivel3->isChecked()) {
        QMessageBox::information(this, "Nivel 3", "Nivel 3 aún no implementado.");
        // Implementar nivel 3
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
