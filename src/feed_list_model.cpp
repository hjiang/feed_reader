// -*- mode: c++; c-basic-offset: 4; -*-

#include "feed_list_model.h"

#include <QDebug>

#include "onyx/base/base.h"

#include "feed.h"
#include "feed_fetcher.h"

namespace onyx {
namespace feed_reader {

static const int FEED_UPDATE_INTERVAL_IN_MS = 300000;  // 5 min

FeedListModel::FeedListModel(QObject* parent, FeedFetcher* feed_fetcher)
        : QAbstractTableModel(parent),
          feed_fetcher_(feed_fetcher),
          timer_() {
    timer_.setSingleShot(false);
    timer_.start(FEED_UPDATE_INTERVAL_IN_MS);
    connect(feed_fetcher_.get(), SIGNAL(feedUpdated(shared_ptr<Feed>)),
            this, SLOT(updateFeed(shared_ptr<Feed>)));
    connect(&timer_, SIGNAL(timeout()), this, SLOT(refreshAllFeeds()));
}

FeedListModel::~FeedListModel() {
}

int FeedListModel::rowCount(const QModelIndex &parent) const {
    return feeds_.size();
}

int FeedListModel::columnCount(const QModelIndex& parent) const {
    return 2;
}

QVariant FeedListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() >= static_cast<int>(feeds_.size()))
        return QVariant();

    if (role == Qt::DisplayRole) {
        Feed* feed = feeds_.at(index.row()).get();
        if (index.column() == 0) {
            // The first column shows feed titles.
            if (!feed->title().isEmpty()) {
                return feed->title();
            } else {
                return feed->feed_url().toString();
            }
        } else if (index.column() == 1){
            return QString::number(feed->unreadCount());
        } else {
            qDebug() << "ERROR: trying to display more than two columns";
            return QVariant();
        }
    } else if (role == FeedIdentifierRole){
        return feeds_.at(index.row())->id();
    } else {
        return QVariant();
    }
}

shared_ptr<Feed> FeedListModel::getFeed(int id) {
    for (size_t i = 0; i < feeds_.size(); ++i) {
        if (feeds_[i]->id() == id) {
            return feeds_[i];
        }
    }
    return shared_ptr<Feed>();
}

void FeedListModel::loadFromDatabase() {
    if (Feed::all(&feeds_)) {
        // Tell the view to query all data again.
        reset();
    } else {
        qDebug() << "Failed to load feeds from database.";
    }
}

void FeedListModel::addFeed(shared_ptr<Feed> feed) {
    // TODO: handle failure. Can happen if the feed already exists.
    feed->saveNew();
    loadFromDatabase();
}

void FeedListModel::addFeed(const QUrl& url) {
    shared_ptr<Feed> feed(new Feed);
    feed->set_feed_url(url);
    feed_fetcher_->scheduleFetch(feed);
    addFeed(feed);
}

void FeedListModel::updateFeed(shared_ptr<Feed> feed) {
    if(!feed->update()) {
        qDebug() << "Error updating feed.";
    }
    if(!feed->saveArticles()) {
        qDebug() << "Error saving articles.";
    }
    loadFromDatabase();
}

void FeedListModel::refreshAllFeeds() {
    vector<shared_ptr<Feed> > feeds;
    if (!Feed::all(&feeds)) {
        qDebug() << "Failed to load feeds from the database.";
        return;
    }
    qDebug() << "Refreshing " << feeds.size() << " feeds";
    for (size_t i = 0; i < feeds.size(); ++i) {
        feed_fetcher_->scheduleFetch(feeds[i]);
    }
}

}  // namespace feed_reader
}  // namespace onyx
