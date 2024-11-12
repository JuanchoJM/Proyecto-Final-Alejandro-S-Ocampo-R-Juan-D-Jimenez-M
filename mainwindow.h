#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startGame();  // Método para cambiar la escena cuando se presiona el botón "Start"

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;       // Escena inicial con imagen
    QGraphicsScene *emptyScene;  // Nueva escena vacía (con fondo blanco)
};

#endif // MAINWINDOW_H

