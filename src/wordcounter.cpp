#include "wordcounter.h"
#include "QtConcurrent/qtconcurrentrun.h"
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
#include <QFuture>

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
    QString content = in.readAll();
    file.close();

    _progressUpdateTimer.start(100);

    // Разделение данных на блоки
    const int blockSize = content.size() / QThread::idealThreadCount();
    QStringList blocks;
    for (int i = 0; i < content.size(); i += blockSize) {
        blocks << content.mid(i, blockSize);
    }

    // Маппинг с использованием всех потоков
    QList<QFuture<QMap<QString, int>>> futures;
    for (const QString &block : blocks) {
        futures.append(QtConcurrent::run([this, block] {
            QMap<QString, int> wordCounts;
            QStringList words = block.split(QRegExp("[^a-zа-я0-9]+"), Qt::SkipEmptyParts);
            for (const QString &word : words) {
                wordCounts[word.toLower()]++;
            }
            _mutex.lock();
            _bytesProcessed += block.size();
            _mutex.unlock();

            emit progressUpdated(_bytesProcessed);
            return wordCounts;
        }));
    }

    // Объединение результатов
    for (const QFuture<QMap<QString, int>> &future : qAsConst(futures)) {
        QMap<QString, int> result = future.result();
        for (auto it = result.constBegin(); it != result.constEnd(); ++it) {
            _globalWordCounts[it.key()] += it.value();
        }
    }

    _progressUpdateTimer.stop();

    emit progressUpdated(_bytesProcessed);
    emit wordCountUpdated(_globalWordCounts);
}

void WordCounter::pauseProcessing()
{
    if (_pause == false) {
        _progressUpdateTimer.start();
        _pause = true;
    } else {
        _pause = false;
        _progressUpdateTimer.stop();
    }
}

void WordCounter::stopProcessing()
{
    _progressUpdateTimer.stop();
    _stop = true;
}

WordCounter::~WordCounter()
{}
