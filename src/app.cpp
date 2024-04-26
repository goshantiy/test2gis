#include "app.h"

TestApp::TestApp()
{
    _wordCounter.moveToThread(&_wordCounterThread);
    _wordCounterThread.start();

    _engine.rootContext()->setContextProperty("histogramModel", &_histogramModel);
    _engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    QObject::connect(&_wordCounter, &WordCounter::progressUpdated, &_histogramModel,
                     &HistogramModel::onProgressUpdated);

    QObject::connect(&_wordCounter, &WordCounter::wordCountUpdated, &_histogramModel,
                     &HistogramModel::addData, Qt::ConnectionType::QueuedConnection);

    QObject::connect(&_wordCounter, &WordCounter::progressGlobalCount, &_histogramModel,
                     &HistogramModel::onProgressGlobalCountUpdated);

    QObject::connect(&_histogramModel, &HistogramModel::processFile, &_wordCounter,
                     &WordCounter::processFile);

    QObject::connect(&_histogramModel, &HistogramModel::stop, &_wordCounter,
                     &WordCounter::stopProcessing);

    QObject::connect(&_histogramModel, &HistogramModel::pause, &_wordCounter,
                     &WordCounter::pauseProcessing);
}
