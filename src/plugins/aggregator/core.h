#ifndef CORE_H
#define CORE_H
#include <memory>
#include <QAbstractItemModel>
#include <QString>
#include <QMap>
#include <QPair>
#include <QDateTime>
#include <interfaces/idownload.h>
#include <boost/shared_ptr.hpp>
#include "item.h"
#include "channel.h"
#include "feed.h"
#include "storagebackend.h"

class ChannelsModel;
class QTimer;
class QNetworkReply;
class QFile;
class ItemModel;
class QWebView;
class JobHolderRepresentation;

namespace LeechCraft
{
	namespace Util
	{
		class TagsCompletionModel;
	};
};

class Core : public QAbstractItemModel
{
    Q_OBJECT

    QMap<QString, QObject*> Providers_;

    enum Columns
    {
        ColumnName = 0
        , ColumnDate = 1
    };

    struct PendingJob
    {
        enum Role
        {
            RFeedAdded
            , RFeedUpdated
            , RFeedExternalData
        } Role_;
        QString URL_;
        QString Filename_;
        QStringList Tags_;
    };
    struct ExternalData
    {
        enum Type
        {
            TImage
            , TIcon
        } Type_;
        Channel_ptr RelatedChannel_;
        Feed_ptr RelatedFeed_;
    };
    QMap<int, PendingJob> PendingJobs_;
    QMap<QString, ExternalData> PendingJob2ExternalData_;

	// First is ParentURL_ and second is Title_
	QPair<QString, QString> CurrentChannelHash_;
	items_shorts_t CurrentItems_;
    QStringList ItemHeaders_;
    ChannelsModel *ChannelsModel_;
	LeechCraft::Util::TagsCompletionModel *TagsCompletionModel_;
    QTimer *UpdateTimer_, *CustomUpdateTimer_;
    bool SaveScheduled_;
	std::auto_ptr<StorageBackend> StorageBackend_;
    ItemModel *ItemModel_;
	JobHolderRepresentation *JobHolderRepresentation_;
	int CurrentRow_;
	QMap<QString, QDateTime> Updates_;

    Core ();
public:
	struct ChannelInfo
	{
		QString Link_;
		QString Description_;
		QString Author_;
	};

    static Core& Instance ();
    void Release ();
    void DoDelayedInit ();
    void SetProvider (QObject*, const QString&);
    void AddFeed (const QString&, const QStringList&);
    void RemoveFeed (const QModelIndex&);
    void Activated (const QModelIndex&);
    void FeedActivated (const QModelIndex&);
	void Selected (const QModelIndex&);
    Item_ptr GetItem (const QModelIndex&) const;
    QAbstractItemModel* GetChannelsModel ();
	LeechCraft::Util::TagsCompletionModel* GetTagsCompletionModel ();
    void UpdateTags (const QStringList&);
    void MarkItemAsUnread (const QModelIndex&);
	bool IsItemRead (int) const;
	bool IsItemCurrent (int) const;
    void MarkChannelAsRead (const QModelIndex&);
    void MarkChannelAsUnread (const QModelIndex&);
    QStringList GetTagsForIndex (int) const;
	ChannelInfo GetChannelInfo (const QModelIndex&) const;
	QPixmap GetChannelPixmap (const QModelIndex&) const;
	void SetTagsForIndex (const QString&, const QModelIndex&);
	QStringList GetCategories (const QModelIndex&) const;
	QStringList GetItemCategories (int) const;
	Feed::FeedSettings GetFeedSettings (const QModelIndex&) const;
	void SetFeedSettings (const Feed::FeedSettings&, const QModelIndex&);
    void UpdateFeed (const QModelIndex&);
    QModelIndex GetUnreadChannelIndex () const;
	int GetUnreadChannelsNumber () const;
	void AddToItemBucket (const QModelIndex&) const;
	void AddFromOPML (const QString&,
			const QString&,
			const std::vector<bool>&);
	void ExportToOPML (const QString&,
			const QString&,
			const QString&,
			const QString&,
			const std::vector<bool>&) const;
	void ExportToBinary (const QString&,
			const QString&,
			const QString&,
			const QString&,
			const std::vector<bool>&) const;
	JobHolderRepresentation* GetJobHolderRepresentation () const;
    ItemModel* GetItemModel () const;
	StorageBackend* GetStorageBackend () const;
	void SubscribeToComments (const QModelIndex&);
	QWebView* CreateWindow ();
	void GetChannels (channels_shorts_t&) const;
	void AddFeeds (const feeds_container_t&, const QString&);

    virtual int columnCount (const QModelIndex& = QModelIndex ()) const;
    virtual QVariant data (const QModelIndex&,
			int = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags (const QModelIndex&) const;
    virtual bool hasChildren (const QModelIndex&) const;
    virtual QVariant headerData (int, Qt::Orientation,
			int = Qt::DisplayRole) const;
    virtual QModelIndex index (int, int,
			const QModelIndex& = QModelIndex()) const;
    virtual QModelIndex parent (const QModelIndex&) const;
    virtual int rowCount (const QModelIndex& = QModelIndex ()) const;
public slots:
	void openLink (const QString&);
    void currentChannelChanged (const QModelIndex&);
    void updateFeeds ();
    void updateIntervalChanged ();
    void showIconInTrayChanged ();
    void handleSslError (QNetworkReply*);
private slots:
    void fetchExternalFile (const QString&, const QString&);
    void scheduleSave ();
    void handleJobFinished (int);
    void handleJobRemoved (int);
    void handleJobError (int, IDownload::Error);
    void saveSettings ();
	void handleChannelDataUpdated (Channel_ptr);
	void handleItemDataUpdated (Item_ptr, Channel_ptr);
	void handleCustomUpdates ();
private:
    void UpdateUnreadItemsNumber () const;
	void FetchPixmap (const Channel_ptr&);
	void FetchFavicon (const Channel_ptr&);
	void HandleExternalData (const QString&, const QFile&);
	QString HandleFeedUpdated (const channels_container_t&,
			const channels_container_t&,
			const channels_container_t&,
			const PendingJob&);
	void MarkChannel (const QModelIndex&, bool);
	void UpdateFeed (const QString&, IDownload*);
signals:
    void error (const QString&) const;
    void showDownloadMessage (const QString&);
    void channelDataUpdated ();
    void unreadNumberChanged (int) const;
};

#endif
