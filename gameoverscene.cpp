#include "gameoverscene.h"
#include <QGraphicsTextItem>
#include <QFont>
#include <QGraphicsProxyWidget>
#include <QPushButton>

GameOverScene::GameOverScene(int finalScore, QObject* parent)
    : QGraphicsScene(parent)
{
    setSceneRect(0, 0, 400, 600);
    QGraphicsTextItem* textItem = new QGraphicsTextItem("Game Over");
    QFont gameOverFont("Courier", 36, QFont::Bold);
    textItem->setFont(gameOverFont);
    textItem->setDefaultTextColor(Qt::red);
    textItem->setPos(width() / 2 - textItem->boundingRect().width() / 2, 100);
    addItem(textItem);

    QGraphicsTextItem* scoreItem = new QGraphicsTextItem(QString("Score: %1").arg(finalScore));
    QFont scoreFont("Courier", 24, QFont::Bold);
    scoreItem->setFont(scoreFont);
    scoreItem->setDefaultTextColor(Qt::white);
    scoreItem->setPos(width() / 2 - scoreItem->boundingRect().width() / 2, 170);
    addItem(scoreItem);
    QPushButton* tryAgainButton = new QPushButton("Try Again");
    tryAgainButton->setFixedSize(200, 60);
    tryAgainButton->setStyleSheet("background-color: rgb(0,200,0);"
                                  "color: black;"
                                  "font-family: 'Courier';"
                                  "font-size: 20px;"
                                  "font-weight: bold;");

    QGraphicsProxyWidget* proxy = addWidget(tryAgainButton);
    proxy->setPos(width() / 2 - tryAgainButton->width() / 2, 350);

    connect(tryAgainButton, &QPushButton::clicked, this, &GameOverScene::tryAgainClicked);
}

