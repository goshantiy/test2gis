#include "histogrammodel.h"
#include <QDebug>
HistogramModel::HistogramModel(QObject *parent) : QAbstractListModel(parent)
{}

int HistogramModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return _data.count();
}

QVariant HistogramModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= _data.count() || index.row() < 0)
        return QVariant();

    switch (role) {
    case WordRole:
        return QVariant::fromValue(_data[index.row()].first);
    case CountRole:
        return QVariant::fromValue(_data[index.row()].second);
    default:
        return QVariant();
    }
}

void HistogramModel::onProgressGlobalCountUpdated(qint64 progress)
{
    setGlobalProgress(progress);
}

void HistogramModel::onProgressUpdated(qint64 progress)
{
    setProgress(progress);
}

void HistogramModel::loadData(const QString &path)
{
    emit processFile(path);
}

void HistogramModel::addData(const QHash<QString, int> &wordCounts)
{
    beginResetModel();
    QList<QPair<QString, int>> wordList;
    for (auto it = wordCounts.begin(); it != wordCounts.end(); ++it) {
        wordList.append(qMakePair(it.key(), it.value()));
    }
    QVector<QPair<QString, int>> topData(15);

    std::partial_sort_copy(wordList.begin(), wordList.end(), topData.begin(), topData.end(),
                           [](const QPair<QString, int> &a, const QPair<QString, int> &b) {
                               return a.second > b.second;
                           });
    _data = topData;

    endResetModel();
}

int HistogramModel::getBarHeight(int index) const
{
    if (index >= 0 && index < _data.count()) {
        // Пример: ширина зависит от количества в процентах от максимального количества
        // Нормализуем количество от 0 до 100 процентов
        int maxValue = 0;
        maxValue = _data.first().second;

        // Вычисляем ширину в процентах
        double percentHeight = 0.0;
        if (maxValue > 0) {
            percentHeight = static_cast<double>(_data[index].second) / maxValue * 100.0;
        }

        // Возвращаем ширину в процентах
        return static_cast<int>(percentHeight);
    }
    return 0;
}

void HistogramModel::onStop()
{
    clear();
    setProgress(0);
    setGlobalProgress(0);
    emit stop();
}

void HistogramModel::onPause()
{
    emit pause();
}

void HistogramModel::clear()
{
    beginResetModel();
    _data.clear();
    endResetModel();
}

qint64 HistogramModel::globalProgress() const
{
    return _globalProgress;
}

void HistogramModel::setGlobalProgress(qint64 newGlobalProgress)
{
    if (_globalProgress == newGlobalProgress)
        return;
    _globalProgress = newGlobalProgress;
    emit globalProgressChanged();
}

qint64 HistogramModel::progress() const
{
    return _progress;
}

void HistogramModel::setProgress(qint64 newProgress)
{
    if (_progress == newProgress)
        return;
    _progress = newProgress;
    emit progressChanged();
}

QHash<int, QByteArray> HistogramModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[WordRole] = "word";
    roles[CountRole] = "count";
    return roles;
}
