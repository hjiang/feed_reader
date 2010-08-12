// -*- mode: c++; c-basic-offset: 4; -*-

#ifndef ONYX_FEED_READER_ARTICLE_LIST_MODEL_H__
#define ONYX_FEED_READER_ARTICLE_LIST_MODEL_H__

#include <QAbstractListModel>
#include <QObject>

#include "onyx/base/base.h"

namespace onyx {
namespace feed_reader {

class Article;
class Feed;

class ArticleListModel : public QAbstractListModel {
    Q_OBJECT;

  public:
    enum ArticleDataRole {
        ArticleIdentifierRole = Qt::UserRole,
        ArticleDisplayRole
    };

    explicit ArticleListModel(QObject* parent);
    virtual ~ArticleListModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    void switchToFeed(shared_ptr<Feed> feed);

  private:
    vector<shared_ptr<Article> > articles_;
};

}  // namespace feed_reader
}  // namespace onyx

#endif // ONYX_FEED_READER_ARTICLE_LIST_MODEL_H__
