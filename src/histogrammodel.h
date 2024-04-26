#ifndef HISTOGRAMMODEL_H
#define HISTOGRAMMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include <QString>
#include <QVariant>

class HistogramModel : public QAbstractListModel
{
    Q_OBJECT

public:
    Q_PROPERTY(int progress READ progress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(int globalProgress READ globalProgress WRITE setGlobalProgress NOTIFY globalProgressChanged)
    enum Roles
    {
        WordRole = Qt::UserRole + 1, // Роль для слова
        CountRole, // Роль для количества вхождений
        ProgressRole // Роль для прогресса
    };

    explicit HistogramModel(QObject *parent = nullptr);

    void addData(const QMap<QString, int> &wordList);
    void clear();

    Q_SIGNAL void processFile(const QString &);
    Q_SIGNAL void progressChanged();
    Q_SIGNAL void globalProgressChanged();

    Q_SLOT void onProgressGlobalCountUpdated(qint64 progress);
    Q_SLOT void onProgressUpdated(qint64 progress);
    Q_INVOKABLE void loadData(const QString &);
    Q_INVOKABLE int getBarHeight(int index) const;
    Q_INVOKABLE void onStop();
    Q_INVOKABLE void onPause();
    Q_SIGNAL void stop();
    Q_SIGNAL void pause();

private:
    qint64 _globalProgress;
    qint64 _progress;
    QList<QPair<QString, int>> _data;
    qint64 progress() const;
    void setProgress(qint64 newProgress);
    qint64 globalProgress() const;
    void setGlobalProgress(qint64 newGlobalProgress);

    // QAbstractItemModel interface
public:
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
};

#endif // HISTOGRAMMODEL_H
