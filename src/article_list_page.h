// -*- mode: c++; c-basic-offset: 4; -*-

#ifndef ONYX_FEED_READER_ARTICLE_LIST_PAGE_H__
#define ONYX_FEED_READER_ARTICLE_LIST_PAGE_H__

#include <vector>
#include <QWidget>

#include "onyx/base/base.h"

#include "article.h"

class QAbstractItemModel;
class QListView;
class QModelIndex;

namespace onyx {
namespace feed_reader {

class Article;

class ArticleListPage : public QWidget {
    Q_OBJECT;
public:
    ArticleListPage(QAbstractItemModel* article_list_model,
            QWidget* parent = NULL);
    virtual ~ArticleListPage();

signals:
    void articleActivated(shared_ptr<Article> article);

private slots:
    void handleActivated(const QModelIndex& index);

private:
    QListView* article_list_view_;

    NO_COPY_AND_ASSIGN(ArticleListPage);
};

}  // namespace feed_reader
}  // namespace onyx

#endif  // ONYX_FEED_READER_ARTICLE_LIST_PAGE_H__
