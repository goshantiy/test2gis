#ifndef WORDCOUNTER_H
#define WORDCOUNTER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QVector>
#include <QDebug>
#include <QTimer>

class WordCounter : public QObject
{
    Q_OBJECT

public:
    explicit WordCounter(QObject *parent = nullptr);
    ~WordCounter();

    Q_SIGNAL void progressGlobalCount(qint64 progress);
    Q_SIGNAL void progressUpdated(qint64 progress);
    Q_SIGNAL void wordCountUpdated(const QHash<QString, int> &wordCounts);

    Q_SLOT void processFile(const QString &filePath);
    Q_SLOT void stopProcessing();

    void pauseProcessing();

private:
    QMutex _mutex;
    bool _stop = false;
    bool _pause = true;
    QTimer _progressUpdateTimer;
    qint64 _bytesProcessed { 0 };
    bool _stopRequested = false;
    QHash<QString, int> _globalWordCounts;
    void processBlock(const QString &textBlock);
};

#endif // WORDCOUNTER_H
