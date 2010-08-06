// -*- mode: c++; c-basic-offset: 4; -*-

#include "article_list_model.h"

#include <QDebug>
#include <QFont>

#include "article.h"

namespace onyx {
namespace feed_reader {

ArticleListModel::ArticleListModel(QObject* parent)
        : QAbstractListModel(parent) {
}

ArticleListModel::~ArticleListModel() {
}

int ArticleListModel::rowCount(const QModelIndex &parent) const {
    return articles_.size();
}

QVariant ArticleListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() >= static_cast<int>(articles_.size()))
        return QVariant();

    if (role == Qt::DisplayRole) {
        return articles_.at(index.row())->title();
    }
    else if (role == ArticleIdentifierRole) {
        return articles_.at(index.row())->url();
    }
    else if (role == ArticleDisplayRole) {
        return QVariant::fromValue(articles_.at(index.row()));
    }
    else if (role == Qt::FontRole) {
        if (articles_.at(index.row())->read()) {
            return QVariant::fromValue(QFont("Serif", 16));
        }
        else {
            return QVariant::fromValue(QFont("Serif", 16, QFont::Bold));
        }
    }
    else {
        return QVariant();
    }
}

void ArticleListModel::switchToFeed(shared_ptr<Feed> feed) {
    Article::loadByFeed(feed, &articles_);
    reset();
}

}  // namespace feed_reader
}  // namespace onyx
