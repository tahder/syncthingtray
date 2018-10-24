﻿#ifndef DATA_SYNCTHINGDIR_H
#define DATA_SYNCTHINGDIR_H

#include "./global.h"

#include <c++utilities/chrono/datetime.h>

#include <QFileInfo>
#include <QJsonObject>
#include <QMetaType>
#include <QString>

namespace Data {

enum class SyncthingDirStatus { Unknown, Idle, Scanning, Synchronizing, OutOfSync };
// note: update "visible: status === 4" in DirectoriesPage.qml (which references OutOfSync by
//       its raw value due to limitations of Qt/Qml) when updating this enum

QString LIB_SYNCTHING_CONNECTOR_EXPORT statusString(SyncthingDirStatus status);

enum class SyncthingDirType { Unknown, SendReceive, SendOnly, ReceiveOnly };

QString LIB_SYNCTHING_CONNECTOR_EXPORT dirTypeString(SyncthingDirType dirType);

struct LIB_SYNCTHING_CONNECTOR_EXPORT SyncthingItemError {
    SyncthingItemError(const QString &message = QString(), const QString &path = QString())
        : message(message)
        , path(path)
    {
    }

    bool operator==(const SyncthingItemError &other) const
    {
        return message == other.message && path == other.path;
    }

    QString message;
    QString path;
};

struct LIB_SYNCTHING_CONNECTOR_EXPORT SyncthingFileChange {
    QString action;
    QString type;
    QString modifiedBy;
    QString path;
    ChronoUtilities::DateTime eventTime;
    bool local = false;
};

struct LIB_SYNCTHING_CONNECTOR_EXPORT SyncthingItemDownloadProgress {
    SyncthingItemDownloadProgress(
        const QString &containingDirPath = QString(), const QString &relativeItemPath = QString(), const QJsonObject &values = QJsonObject());
    QString relativePath;
    QFileInfo fileInfo;
    int blocksCurrentlyDownloading = 0;
    int blocksAlreadyDownloaded = 0;
    int totalNumberOfBlocks = 0;
    unsigned int downloadPercentage = 0;
    int blocksCopiedFromOrigin = 0;
    int blocksCopiedFromElsewhere = 0;
    int blocksReused = 0;
    int bytesAlreadyHandled;
    int totalNumberOfBytes = 0;
    QString label;
    ChronoUtilities::DateTime lastUpdate;
    static constexpr unsigned int syncthingBlockSize = 128 * 1024;
};

struct LIB_SYNCTHING_CONNECTOR_EXPORT SyncthingCompletion {
    ChronoUtilities::DateTime lastUpdate;
    double percentage = 0;
    quint64 globalBytes = 0;
    struct Needed {
        quint64 bytes = 0;
        quint64 items = 0;
        quint64 deletes = 0;
        constexpr bool isNull() const;
        constexpr bool operator==(const Needed &other) const;
        constexpr bool operator!=(const Needed &other) const;
    } needed;
};

constexpr bool SyncthingCompletion::Needed::isNull() const
{
    return bytes == 0 && items == 0 && deletes == 0;
}

constexpr bool SyncthingCompletion::Needed::operator==(const SyncthingCompletion::Needed &other) const
{
    return bytes == other.bytes && items == other.items && deletes == other.deletes;
}

constexpr bool SyncthingCompletion::Needed::operator!=(const SyncthingCompletion::Needed &other) const
{
    return !(*this == other);
}

struct LIB_SYNCTHING_CONNECTOR_EXPORT SyncthingStatistics {
    quint64 bytes = 0;
    quint64 deletes = 0;
    quint64 dirs = 0;
    quint64 files = 0;
    quint64 symlinks = 0;

    constexpr bool isNull() const;
    constexpr bool operator==(const SyncthingStatistics &other) const;
    constexpr bool operator!=(const SyncthingStatistics &other) const;
    SyncthingStatistics &operator+=(const SyncthingStatistics &other);
};

constexpr bool SyncthingStatistics::isNull() const
{
    return bytes == 0 && deletes == 0 && dirs == 0 && files == 0 && symlinks == 0;
}

constexpr bool SyncthingStatistics::operator==(const SyncthingStatistics &other) const
{
    return bytes == other.bytes && deletes == other.deletes && dirs == other.dirs && files == other.files && symlinks == other.symlinks;
}

constexpr bool SyncthingStatistics::operator!=(const SyncthingStatistics &other) const
{
    return !(*this == other);
}

struct LIB_SYNCTHING_CONNECTOR_EXPORT SyncthingDir {
    SyncthingDir(const QString &id = QString(), const QString &label = QString(), const QString &path = QString());
    bool assignStatus(const QString &statusStr, ChronoUtilities::DateTime time);
    bool assignStatus(SyncthingDirStatus newStatus, ChronoUtilities::DateTime time);
    bool assignDirType(const QString &dirType);
    const QString &displayName() const;
    QString statusString() const;
    QString dirTypeString() const;
    QStringRef pathWithoutTrailingSlash() const;
    bool isLocallyUpToDate() const;
    bool areRemotesUpToDate() const;
    bool isUnshared() const;

    QString id;
    QString label;
    QString path;
    QStringList deviceIds;
    QStringList deviceNames;
    SyncthingDirType dirType = SyncthingDirType::Unknown;
    int rescanInterval = 0;
    int minDiskFreePercentage = 0;
    SyncthingDirStatus status = SyncthingDirStatus::Idle;
    ChronoUtilities::DateTime lastStatusUpdate;
    int completionPercentage = 0;
    int scanningPercentage = 0;
    double scanningRate = 0;
    double fileSystemWatcherDelay = 0.0;
    std::map<QString, SyncthingCompletion> completionByDevice;
    QString globalError;
    std::vector<SyncthingItemError> itemErrors;
    std::vector<SyncthingItemError> previousItemErrors;
    std::vector<SyncthingFileChange> recentChanges;
    SyncthingStatistics globalStats, localStats, neededStats;
    ChronoUtilities::DateTime lastStatisticsUpdate;
    ChronoUtilities::DateTime lastScanTime;
    ChronoUtilities::DateTime lastFileTime;
    QString lastFileName;
    std::vector<SyncthingItemDownloadProgress> downloadingItems;
    int blocksAlreadyDownloaded = 0;
    int blocksToBeDownloaded = 0;
    QString downloadLabel;
    unsigned int downloadPercentage = 0;
    bool ignorePermissions = false;
    bool ignoreDelete = false;
    bool ignorePatterns = false;
    bool autoNormalize = false;
    bool lastFileDeleted = false;
    bool fileSystemWatcherEnabled = false;
    bool paused = false;

private:
    bool checkWhetherStatusUpdateRelevant(ChronoUtilities::DateTime time);
    bool finalizeStatusUpdate(SyncthingDirStatus newStatus);
};

inline SyncthingDir::SyncthingDir(const QString &id, const QString &label, const QString &path)
    : id(id)
    , label(label)
    , path(path)
{
}

inline const QString &SyncthingDir::displayName() const
{
    return label.isEmpty() ? id : label;
}

inline QString SyncthingDir::dirTypeString() const
{
    return ::Data::dirTypeString(dirType);
}

inline bool SyncthingDir::isLocallyUpToDate() const
{
    return neededStats.isNull();
}

inline bool SyncthingDir::isUnshared() const
{
    return deviceIds.empty() && (status == SyncthingDirStatus::Idle || status == SyncthingDirStatus::Unknown);
}

inline bool SyncthingDir::assignStatus(SyncthingDirStatus newStatus, ChronoUtilities::DateTime time)
{
    return checkWhetherStatusUpdateRelevant(time) && finalizeStatusUpdate(newStatus);
}

struct LIB_SYNCTHING_CONNECTOR_EXPORT SyncthingOverallDirStatistics {
    SyncthingOverallDirStatistics();
    SyncthingOverallDirStatistics(const std::vector<SyncthingDir> &directories);

    SyncthingStatistics local;
    SyncthingStatistics global;
    SyncthingStatistics needed;

    bool isNull() const;
};

inline SyncthingOverallDirStatistics::SyncthingOverallDirStatistics()
{
}

inline bool SyncthingOverallDirStatistics::isNull() const
{
    return local.isNull() && global.isNull();
}

} // namespace Data

Q_DECLARE_METATYPE(Data::SyncthingItemError)
Q_DECLARE_METATYPE(Data::SyncthingItemDownloadProgress)
Q_DECLARE_METATYPE(Data::SyncthingStatistics)
Q_DECLARE_METATYPE(Data::SyncthingOverallDirStatistics)
Q_DECLARE_METATYPE(Data::SyncthingDir)

#endif // DATA_SYNCTHINGDIR_H
