#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QList>
//#include <QMediaPlayer>
//#include <QAudioOutput>
#include <QGraphicsRectItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startGame();
    void registrarse();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;        // Escena inicial
    QGraphicsScene *emptyScene;  // Escena de juego
    //QMediaPlayer *player;
   // QAudioOutput *audioOutput;

    // Métodos auxiliares
    bool verificarUsuario(const QString &usuario, const QString &contrasena);
    QList<QGraphicsItem*> crearPlataformas(QGraphicsScene *scene);
};

#endif // MAINWINDOW_H
