#include "startscene.h"
#include <QBrush>
#include <QFont>
#include <QGraphicsTextItem>
#include <QPropertyAnimation>
#include <QTimer>
#include <QPushButton>
#include <windows.h>
#include <mmsystem.h>
#include "constants.h"
#pragma comment(lib, "winmm.lib")

StartScene::StartScene(QGraphicsView* parentView, QObject* parent)
    : QGraphicsScene(0, 0, fieldWidth + panelWidth, fieldHeight, parent),
    view(parentView)
{
    setBackgroundBrush(QColor(10, 10, 60));
    createTitle();
    createStartButton();
    createDecorativeBlocks();

    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &StartScene::animateBlocks);
    animationTimer->start(50);
}

void StartScene::createTitle() {
    QGraphicsTextItem* title = addText("TETRIS");
    title->setDefaultTextColor(Qt::white);
    title->setFont(QFont("Courier", 36, QFont::Bold));
    title->setPos(width() / 2 - title->boundingRect().width() / 2, 50);

    QGraphicsTextItem* subtitle = new QGraphicsTextItem("made by Vintouvka");
    QFont subtitleFont("Courier", 16, QFont::Bold);
    subtitle->setFont(subtitleFont);
    subtitle->setDefaultTextColor(QColor(255, 255, 255, 50));
    subtitle->setTransformOriginPoint(subtitle->boundingRect().center());
    subtitle->setPos((fieldWidth + panelWidth) / 2 - 95, 185);
    addItem(subtitle);

    QPropertyAnimation* pulseAnim = new QPropertyAnimation(subtitle, "scale");
    pulseAnim->setDuration(2000);
    pulseAnim->setStartValue(1.0);
    pulseAnim->setKeyValueAt(0.5, 1.15);
    pulseAnim->setEndValue(1.0);
    pulseAnim->setLoopCount(-1);
    pulseAnim->start(QAbstractAnimation::DeleteWhenStopped);
}
void StartScene::createStartButton() {
    startButton = new QPushButton("START", view);
    startButton->setObjectName("startButton");
    startButton->setFixedSize(200, 60);
    startButton->setStyleSheet(
        "background-color: rgb(0, 200, 0);"
        "color: black;"
        "font: bold 20px 'Courier';"
        "border-radius: 5px;"
        );
    startButton->move(view->width() / 2 - 100, view->height() / 2 - 30);
    startButton->setFocusPolicy(Qt::NoFocus);
    startButton->show();

    connect(startButton, &QPushButton::clicked, this, [=]() {
        startButton->hide();
        PlaySound(
            TEXT("C:/Users/natal/Downloads/didj-button-sound.wav"), NULL, SND_FILENAME | SND_ASYNC);

        startClicked = true;
        emit startGame();
    });
}
void StartScene::createDecorativeBlocks() {
    int baseY = height() - 100;
    for (int i = 0; i < 7; ++i) {
        QGraphicsRectItem* block = addRect(0, 0, tiles, tiles);
        block->setBrush(QColor::fromHsv((i * 45) % 360, 255, 200));
        block->setPos(width() / 2 - 3 * tiles + i * tiles, baseY);
        decorativeBlocks.append(block);
        blockDirections.append((i % 2 == 0) ? 1 : -1);
    }
}
void StartScene::animateBlocks() {
    for (int i = 0; i < decorativeBlocks.size(); ++i) {
        QGraphicsRectItem* block = decorativeBlocks[i];
        QPointF pos = block->pos();
        int dir = blockDirections[i];

        pos.setY(pos.y() + dir);
        block->setPos(pos);

        if (pos.y() < height() - 110 || pos.y() > height() - 90) {
            blockDirections[i] *= -1;
        }
    }
}
