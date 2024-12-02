#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QList>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QGraphicsRectItem>
#include <QAudioOutput>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
QList<QGraphicsItem*> crearPlataformasNivel3(QGraphicsScene *scene);
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void reproducirSiguienteCancion();
    void reanudarVideo();
private slots:
    void cancionTerminada(QMediaPlayer::MediaStatus status);
    void registrarse();
    void verificarInicio();
    void nivel1();
    void nivel2();
    void nivel3();
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;        // Escena inicial
    QGraphicsScene *emptyScene;  // Escena de juego
    void mousePressEvent(QMouseEvent *event);

    // Métodos auxiliares
    bool verificarUsuario(const QString &usuario, const QString &contrasena);
    QList<QGraphicsItem*> crearPlataformas(QGraphicsScene *scene);

    QMediaPlayer *player;
    QVideoWidget *videoWidget;
    QAudioOutput *audioOutput;
};

#endif // MAINWINDOW_H
