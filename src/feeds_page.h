// -*- mode: c++; c-basic-offset: 4; -*-

#ifndef ONYX_FEED_READER_FEEDS_PAGE_H__
#define ONYX_FEED_READER_FEEDS_PAGE_H__

#include <QHash>
#include <QWidget>

#include "onyx/base/base.h"

#include "feed.h"

class QAbstractItemModel;
class QTableView;
class QPushButton;
class QModelIndex;
class QShowEvent;
class QUrl;
class QItemSelectionModel;

namespace onyx {
namespace feed_reader {

class AddFeedDialog;
class FeedListItem;
class FeedListModel;

class FeedsPage : public QWidget {
    Q_OBJECT;
public:
    explicit FeedsPage(FeedListModel* feed_list_model,
            QWidget* parent = NULL);
    virtual ~FeedsPage();

signals:
    void feedActivated(int);
    void pageRight();

protected:
    virtual void showEvent(QShowEvent* event);

private slots:
    void showAddFeedDialog();
    void addFeed();
    void handleActivated(const QModelIndex& index);
    void deleteFeed();
    void deleteFeeds();///<call deleteFeed();

private:
    friend class AcceptanceTest;

    AddFeedDialog* add_feed_dialog_;
    QTableView* feed_list_view_;
    FeedListModel* feed_list_model_;
    QItemSelectionModel *selectionModel_;
    // Those pointers are retained for testing
    QPushButton* add_feed_button_;

    NO_COPY_AND_ASSIGN(FeedsPage);
//   void Mark(QModelIndex arg1);
};

}  // namespace feed_reader
}  // namespace onyx

#endif  // ONYX_FEED_READER_FEEDS_PAGE_H__
// kate: indent-mode cstyle; space-indent on; indent-width 0;
