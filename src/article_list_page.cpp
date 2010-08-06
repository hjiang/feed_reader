// -*- mode: c++; c-basic-offset: 4; -*-

#include "article_list_page.h"

#include <QAbstractItemModel>
#include <QListView>
#include <QSizePolicy>
#include <QVBoxLayout>

#include "onyx/screen/screen_proxy.h"

#include "article_list_model.h"
#include "singleton.h"
#include "widget_updater.h"

namespace onyx {
namespace feed_reader {

ArticleListPage::ArticleListPage(QAbstractItemModel* article_list_model,
        QWidget* parent)
        : QWidget(parent),
        article_list_view_(new QListView(this)) {
    article_list_view_->setModel(article_list_model);
    QSizePolicy size_policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    article_list_view_->setSizePolicy(size_policy);
    article_list_view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(article_list_view_);
    setLayout(layout);
    connect(article_list_view_, SIGNAL(activated(const QModelIndex&)),
            this, SLOT(handleActivated(const QModelIndex&)));
    connect(article_list_view_, SIGNAL(clicked(const QModelIndex&)),
            this, SLOT(handleActivated(const QModelIndex&)));
    using onyx::screen::ScreenProxy;
    WidgetUpdater& updater(Singleton<WidgetUpdater>::instance());
    updater.addWidget(article_list_view_, ScreenProxy::GU);
}

ArticleListPage::~ArticleListPage() {}

void ArticleListPage::handleActivated(const QModelIndex& index) {
    shared_ptr<Article> article(article_list_view_->model()->data(
                index,
                ArticleListModel::ArticleDisplayRole)
            .value<shared_ptr<Article> >());
    article->set_read(true);
    article->saveOrUpdate();
    emit articleActivated(article);
}

}  // namespace onyx
}  // namespace feed_reader
