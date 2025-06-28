#include "pausescene.h"
#include <QGraphicsTextItem>
#include <QFont>
#include <QPushButton>
#include <QGraphicsView>
#include <QFile>
#include <QDebug>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

PauseScene::PauseScene(int w, int h, QGraphicsView* view, QObject* parent)
    : QGraphicsScene(0, 0, w, h, parent)
    , resumeBtn(nullptr)
    , quitBtn(nullptr)
{
    setBackgroundBrush(QColor(0,0,0,150));
    auto txt = addText("PAUSED", QFont("Courier", 32, QFont::Bold));
    txt->setDefaultTextColor(Qt::white);
    qreal tx = (w - txt->boundingRect().width()) / 2;
    qreal ty = (h - txt->boundingRect().height()) / 2 - 60;
    txt->setPos(tx, ty);
    resumeBtn = new QPushButton("Resume", view);
    resumeBtn->setFixedSize(140, 50);
    resumeBtn->setStyleSheet("background-color: green;" "color: white;" "font: bold 16px 'Courier';" "border-radius: 12px;");
    resumeBtn->move((view->width() - resumeBtn->width()) / 2, ty + 80);
    resumeBtn->setFocusPolicy(Qt::NoFocus);
    resumeBtn->show();
    connect(resumeBtn, &QPushButton::clicked, this, [this]() {
        const QString wavPath = "C:/Users/natal/Downloads/didj-button-sound.wav";
        if (!QFile::exists(wavPath)) {
            qWarning() << "Resume sound not found:" << wavPath;
        } else {
            PlaySoundA(wavPath.toLocal8Bit().constData(), NULL, SND_FILENAME | SND_ASYNC);
        }
        emit resumeRequested();
    });
    quitBtn = new QPushButton("Quit", view);
    quitBtn->setFixedSize(140, 50);
    quitBtn->setStyleSheet( "background-color: red;" "color: white;" "font: bold 16px 'Courier';" "border-radius: 12px;");
    quitBtn->move((view->width() - quitBtn->width()) / 2, ty + 150);
    quitBtn->setFocusPolicy(Qt::NoFocus);
    quitBtn->show();
    connect(quitBtn, &QPushButton::clicked, this, [this]() {
        const QString wavPath = "C:/Users/natal/Downloads/didj-button-sound.wav";
        if (!QFile::exists(wavPath)) {
            qWarning() << "Quit sound not found:" << wavPath;
        } else {
            PlaySoundA(wavPath.toLocal8Bit().constData(), NULL, SND_FILENAME | SND_ASYNC);
        }
        emit quitRequested();
    });
}
PauseScene::~PauseScene()
{
    delete resumeBtn;
    delete quitBtn;
}

