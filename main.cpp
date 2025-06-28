#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QFont>
#include <QPen>
#include <QBrush>
#include <QPushButton>
#include "constants.h"
#include "game.h"
#include "startscene.h"

QGraphicsView* view = nullptr;
Game* game = nullptr;

void startGame() {
    if (auto oldBtn = view->findChild<QPushButton*>("pauseButton")) {
        oldBtn->deleteLater();
    }
    QGraphicsScene* scene = new QGraphicsScene(0, 0, fieldWidth + panelWidth, fieldHeight);
    scene->setBackgroundBrush(QColor(150, 200, 255));
    scene->addRect(0, 0, fieldWidth, fieldHeight, Qt::NoPen, QColor(12, 20, 40));
    QPen gridPen(QColor(25, 40, 70));
    for (int x = 0; x <= fieldWidth; x += tiles)
        scene->addLine(x, 0, x, fieldHeight, gridPen);
    for (int y = 0; y <= fieldHeight; y += tiles)
        scene->addLine(0, y, fieldWidth, y, gridPen);
    scene->addRect(fieldWidth, 0, panelWidth, fieldHeight, Qt::NoPen, QColor(40, 30, 80));
    auto scoreLabel = scene->addText("Score", QFont("Courier", 14, QFont::Bold));
    scoreLabel->setDefaultTextColor(Qt::white);
    scoreLabel->setPos(fieldWidth + 35, 20);

    auto scoreBox = scene->addRect(fieldWidth + 25, 50, 90, 30, Qt::NoPen, QColor(50, 70, 130));
    auto scoreValue = scene->addText("0", QFont("Courier", 14, QFont::Bold));
    scoreValue->setDefaultTextColor(Qt::white);
    scoreValue->setPos(fieldWidth + 60, 52);

    auto nextLabel = scene->addText("Next", QFont("Courier", 14, QFont::Bold));
    nextLabel->setDefaultTextColor(Qt::white);
    nextLabel->setPos(fieldWidth + 35, 100);
    scene->addRect(fieldWidth + 20, 130, 100, 100, Qt::NoPen, QColor(40, 40, 100));

    QPushButton* pauseButton = new QPushButton("Pause", view);
    pauseButton->setObjectName("pauseButton");
    pauseButton->setFixedSize(80, 40);
    pauseButton->setStyleSheet("background-color: green;" "color: white;" "font: bold 14px 'Courier';" "border-radius: 5px;");
    pauseButton->move(view->width() - 110, 300);
    pauseButton->setFocusPolicy(Qt::NoFocus);
    pauseButton->show();
    if (game) {
        qApp->removeEventFilter(game);
        delete game;
        game = nullptr;
    }
    game = new Game(scene, scoreValue);
    QObject::connect(game, &Game::tryAgainRequested, []() { startGame(); });
    QObject::connect(pauseButton, &QPushButton::clicked, []() {
        if (game) game->pauseGame();
    });
    qApp->installEventFilter(game);
    game->start();
    view->setScene(scene);
    view->setFocus();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    view = new QGraphicsView();
    view->setFixedSize(fieldWidth + panelWidth + 2, fieldHeight + 2);
    view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFocus();
    StartScene* startScene = new StartScene(view);
    QObject::connect(startScene, &StartScene::startGame, []() {
        startGame();
    });
    view->setScene(startScene);
    view->show();

    return app.exec();
}

