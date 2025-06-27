#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QFont>
#include <QPen>
#include <QBrush>
#include "constants.h"
#include "game.h"
#include "startscene.h"

QGraphicsView* view = nullptr;
Game* game = nullptr;
void startGame() {
    QGraphicsScene* scene = new QGraphicsScene(0, 0, fieldWidth + panelWidth, fieldHeight);
    scene->setBackgroundBrush(QColor(150, 200, 255));
    QColor fieldColor(12, 20, 40);
    QGraphicsRectItem* field = scene->addRect(0, 0, fieldWidth, fieldHeight, Qt::NoPen, fieldColor);
    QPen gridPen(QColor(25, 40, 70));
    for (int x = 0; x <= fieldWidth; x += tiles)
        scene->addLine(x, 0, x, fieldHeight, gridPen);
    for (int y = 0; y <= fieldHeight; y += tiles)
        scene->addLine(0, y, fieldWidth, y, gridPen);

    QGraphicsRectItem* sidePanel = scene->addRect(fieldWidth, 0, panelWidth, fieldHeight, Qt::NoPen, QColor(40, 30, 80));
    QGraphicsTextItem* scoreLabel = scene->addText("Score");
    scoreLabel->setDefaultTextColor(Qt::white);
    scoreLabel->setFont(QFont("Courier", 14, QFont::Bold));
    scoreLabel->setPos(fieldWidth + 35, 20);
    QGraphicsRectItem* scoreBox = scene->addRect(fieldWidth + 25, 50, 90, 30, Qt::NoPen, QColor(50, 70, 130));
    QGraphicsTextItem* scoreValue = scene->addText("0");
    scoreValue->setDefaultTextColor(Qt::white);
    scoreValue->setFont(QFont("Courier", 14, QFont::Bold));
    scoreValue->setPos(fieldWidth + 60, 52);
    QGraphicsTextItem* nextLabel = scene->addText("Next");
    nextLabel->setDefaultTextColor(Qt::white);
    nextLabel->setFont(QFont("Courier", 14, QFont::Bold));
    nextLabel->setPos(fieldWidth + 35, 100);
    QGraphicsRectItem* nextBox = scene->addRect(fieldWidth + 20, 130, 100, 100, Qt::NoPen, QColor(40, 40, 100));

    if (game) {
        qApp->removeEventFilter(game);
        delete game;
        game = nullptr;
    }
    game = new Game(scene, scoreValue);
    QObject::connect(game, &Game::tryAgainRequested, []() {
        startGame();
    });

    qApp->installEventFilter(game);
    game->start();
    view->setScene(scene);
    view->setFocus();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    view = new QGraphicsView();
    view->setFixedSize(fieldWidth + panelWidth + 2, fieldHeight + 2);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFocus();
    StartScene* startScene = new StartScene();
    QObject::connect(startScene, &StartScene::startGame, []() {
        startGame();
    });
    view->setScene(startScene);
    view->show();

    return a.exec();
}

