#ifndef APP_H
#define APP_H
#include <QDebug>
#include <QFile>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include "histogrammodel.h"
#include "wordcounter.h"

class TestApp
{
public:
    TestApp();

private:
    HistogramModel _histogramModel;
    QThread _wordCounterThread;
    WordCounter _wordCounter;
    QQmlApplicationEngine _engine;
};

#endif
