#include "pausescene.h"
#include <QGraphicsRectItem>
#include <QPushButton>
#include <QGraphicsView>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QPen>
#include <QBrush>

extern QGraphicsView* view;

PauseScene::PauseScene(int w, int h, QGraphicsView* viewArg, QObject* parent)
    : QGraphicsScene(0, 0, w, h, parent)
{
    addRect(0, 0, w, h,
            QPen(Qt::NoPen),
            QBrush(QColor(0, 0, 0, 200)))
        ->setZValue(0);
    const int pausedW = 240;
    const int pausedH = 100;
    const int pausedX = (w - pausedW) / 2;
    const int pausedY = (h - pausedH) / 2 - 70;
    pausedBtn = new QPushButton("PAUSED", viewArg);
    pausedBtn->setEnabled(false);
    pausedBtn->setFixedSize(pausedW, pausedH);
    pausedBtn->setStyleSheet(
        "background-color: #000000;"
        "color: #ffffff;"
        "font: bold 60px Courier;"
        "border-radius: 40px;"
        );
    pausedBtn->move(pausedX, pausedY);
    pausedBtn->show();

    resumeBtn = new QPushButton("Resume", viewArg);
    resumeBtn->setFixedSize(140, 50);
    resumeBtn->setStyleSheet(
        "background-color: green;"
        "color: white;"
        "font: bold 16px 'Courier';"
        "border-radius: 12px;"
        );

    resumeBtn->move(
        (w - resumeBtn->width()) / 2,
        pausedY + pausedH + 10
        );
    resumeBtn->setFocusPolicy(Qt::NoFocus);
    resumeBtn->show();

    {
        auto* ao = new QAudioOutput(this);
        ao->setVolume(0.5);
        auto* player = new QMediaPlayer(this);
        player->setAudioOutput(ao);
        player->setSource(QUrl("qrc:/sounds/resources/didj-button-sound.wav"));
        connect(resumeBtn, &QPushButton::clicked, this, [this, player]() {
            player->stop();
            player->play();
            pausedBtn->hide();
            emit resumeRequested();
        });
    }


    quitBtn = new QPushButton("Quit", viewArg);
    quitBtn->setFixedSize(140, 50);
    quitBtn->setStyleSheet(
        "background-color: red;"
        "color: white;"
        "font: bold 16px 'Courier';"
        "border-radius: 12px;"
        );
    quitBtn->move(
        (w - quitBtn->width()) / 2,
        pausedY + pausedH + resumeBtn->height() + 20
        );
    quitBtn->setFocusPolicy(Qt::NoFocus);
    quitBtn->show();
    {
        auto* ao = new QAudioOutput(this);
        ao->setVolume(0.5);
        auto* player = new QMediaPlayer(this);
        player->setAudioOutput(ao);
        player->setSource(QUrl("qrc:/sounds/resources/didj-button-sound.wav"));
        connect(quitBtn, &QPushButton::clicked, this, [this, player]() {
            player->stop();
            player->play();
            pausedBtn->hide();
            emit quitRequested();
        });
    }
}

PauseScene::~PauseScene()
{
    if (resumeBtn) resumeBtn->deleteLater();
    if (quitBtn)   quitBtn->deleteLater();
    if (pausedBtn) pausedBtn->deleteLater();
}
