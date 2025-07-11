#include "startscene.h"
#include <QBrush>
#include <QFont>
#include <QGraphicsTextItem>
#include <QPropertyAnimation>
#include <QTimer>
#include <QPushButton>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "constants.h"

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
    auto* title = addText("TETRIS", QFont("Courier", 36, QFont::Bold));
    title->setDefaultTextColor(Qt::white);
    title->setPos((width() - title->boundingRect().width())/2, 50);

    auto* subtitle = new QGraphicsTextItem("made by Vintouvka");
    subtitle->setFont(QFont("Courier", 16, QFont::Bold));
    subtitle->setDefaultTextColor(QColor(255, 255, 255, 50));
    subtitle->setTransformOriginPoint(subtitle->boundingRect().center());
    subtitle->setPos((fieldWidth + panelWidth)/2 - subtitle->boundingRect().width()/2, 185);
    addItem(subtitle);

    auto* pulseAnim = new QPropertyAnimation(subtitle, "scale");
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
    startButton->setFlat(true);
    startButton->setAttribute(Qt::WA_StyledBackground, true);
    startButton->setStyleSheet(R"(
        QPushButton {
            background-color: rgb(0,200,0);
            color: black;
            font: bold 20px 'Courier';
            border: none;
            border-radius: 12px;
        }
        QPushButton:pressed {
            background-color: rgb(0,180,0);
        }
    )");
    startButton->move(view->width()/2 - startButton->width()/2,
                      view->height()/2 - startButton->height()/2);
    startButton->setFocusPolicy(Qt::NoFocus);
    startButton->show();
    auto* audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(0.5);

    auto* clickPlayer = new QMediaPlayer(this);
    clickPlayer->setAudioOutput(audioOutput);
    clickPlayer->setSource(QUrl("qrc:/sounds/resources/didj-button-sound.wav"));

    connect(startButton, &QPushButton::clicked, this, [this, clickPlayer]() {
        startButton->hide();
        clickPlayer->stop();
        clickPlayer->play();
        emit startGame();
    });
}

void StartScene::createDecorativeBlocks() {
    int baseY = height() - 100;
    for (int i = 0; i < 7; ++i) {
        auto* block = addRect(0, 0, tiles, tiles);
        block->setBrush(QColor::fromHsv((i * 45) % 360, 255, 200));
        block->setPos(width()/2 - 3*tiles + i*tiles, baseY);
        decorativeBlocks.append(block);
        blockDirections.append((i % 2 == 0) ? 1 : -1);
    }
}

void StartScene::animateBlocks() {
    for (int i = 0; i < decorativeBlocks.size(); ++i) {
        auto* block = decorativeBlocks[i];
        QPointF pos = block->pos();
        int dir = blockDirections[i];
        pos.setY(pos.y() + dir);
        block->setPos(pos);
        if (pos.y() < height() - 110 || pos.y() > height() - 90) {
            blockDirections[i] = -dir;
        }
    }
}

