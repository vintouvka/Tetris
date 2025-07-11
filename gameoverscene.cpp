#include "gameoverscene.h"
#include <QGraphicsTextItem>
#include <QFont>
#include <QPushButton>
#include <QSoundEffect>
#include <QUrl>
#include <QGraphicsView>

extern QGraphicsView* view;

GameOverScene::GameOverScene(int finalScore, QObject* parent)
    : QGraphicsScene(0, 0, 400, 600, parent),
    view(::view)
{
    auto title = addText("Game Over", QFont("Courier", 36, QFont::Bold));
    title->setDefaultTextColor(Qt::red);
    title->setPos((width() - title->boundingRect().width())/2, 100);

    auto scoreItem =
        addText(QString("Score: %1").arg(finalScore),
                QFont("Courier", 24, QFont::Bold));
    scoreItem->setDefaultTextColor(Qt::white);
    scoreItem->setPos((width() - scoreItem->boundingRect().width())/2, 170);

    setupTryAgainButton();
}

void GameOverScene::setupTryAgainButton() {
    tryAgainButton = new QPushButton("Try Again", view);
    tryAgainButton->setObjectName("tryAgainButton");
    tryAgainButton->setFixedSize(200, 60);
    tryAgainButton->setStyleSheet(
        "background-color: rgb(0,200,0);"
        "color: black;"
        "font: bold 20px 'Courier';"
        "border-radius: 5px;"
        );
    int x = (view->width() - tryAgainButton->width())/2;
    tryAgainButton->move(x, 350);
    tryAgainButton->setFocusPolicy(Qt::NoFocus);
    tryAgainButton->show();

    auto* clickEffect = new QSoundEffect(this);
    clickEffect->setSource(QUrl("qrc:/sounds/resources/didj-button-sound.wav"));
    clickEffect->setLoopCount(1);
    clickEffect->setVolume(0.8f);

    connect(tryAgainButton, &QPushButton::clicked, this,
            [this, clickEffect]() {
                tryAgainButton->hide();
                clickEffect->play();
                emit tryAgainClicked();
            }
            );
}



