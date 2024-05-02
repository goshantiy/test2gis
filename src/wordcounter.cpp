#include "wordcounter.h"
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QMap>
#include <QVector>
#include <QTimer>
#include <QCoreApplication>
#include <QUrl>
#include <QFileInfo>
#include <QThread>

WordCounter::WordCounter(QObject *parent) : QObject(parent), _progressUpdateTimer(this)
{
    qRegisterMetaType<QHash<QString, int>>("QHash<QString, int>");
    connect(&_progressUpdateTimer, &QTimer::timeout, [this]() {
        emit progressUpdated(_bytesProcessed);
        emit wordCountUpdated(_globalWordCounts);
    });
}

void WordCounter::processFile(const QString &filePath)
{
    _stop = false;
    _pause = false;
    _bytesProcessed = 0;
    _globalWordCounts.clear();
    auto filepath = QUrl(filePath).toLocalFile();
    QFileInfo info(filepath);
    emit progressGlobalCount(info.size());
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString word;
    _progressUpdateTimer.start(50);
    while (!(in >> word).atEnd() && !_stop) {
        while (_pause) {
            QCoreApplication::processEvents();
            QThread::sleep(1);
        }
        QString cleanWord = word.toLower().remove(QRegExp("[^a-zа-я0-9]"));
        _bytesProcessed += word.count() + 2;
        if (!cleanWord.isEmpty()) {
            _globalWordCounts[cleanWord]++;
            if (_bytesProcessed % 1000)
                QCoreApplication::processEvents();
        }
    }
    file.close();
    _progressUpdateTimer.stop();
    emit progressUpdated(_bytesProcessed);
    emit wordCountUpdated(_globalWordCounts);
}

void WordCounter::pauseProcessing()
{
    if (_pause == false) {
        _progressUpdateTimer.stop();
        _pause = true;
    } else {
        _pause = false;
        _progressUpdateTimer.start(50);
    }
}

void WordCounter::stopProcessing()
{
    _progressUpdateTimer.stop();
    _stop = true;
}

WordCounter::~WordCounter()
{}
