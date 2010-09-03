// -*- mode: c++; c-basic-offset: 4; -*-

#include "article_list_model.h"

#include <QDebug>
#include <QFont>
#include <QIcon>

#include "article.h"

namespace onyx {
namespace feed_reader {

ArticleListModel::ArticleListModel(QObject* parent)
        : QAbstractTableModel(parent) {
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

    shared_ptr<Article> article = articles_.at(index.row());
     if (index.column() == 0 && role == Qt::DecorationRole ) {
        if (article->read()) {
            return  QIcon(":/images/mail-mark-read.png");
        } else {
            return  QIcon(":/images/mail-mark-unread-new.png");
        }
    }

    if (index.column() == 1) {
        if (role == Qt::DisplayRole) {
            return article->title();
        } else if (role == ArticleIdentifierRole) {
            return article->url();
        } else if (role == ArticleDisplayRole) {
            return QVariant::fromValue(article);
        } else if (role == Qt::FontRole) {
            if (articles_.at(index.row())->read()) {
                return QVariant::fromValue(QFont("Serif", 16));
            } else {
                return QVariant::fromValue(QFont("Serif", 16, QFont::Bold));
            }
        }
    }
    return QVariant();
}


bool ArticleListModel::setData(const QModelIndex& index, const QVariant& Value, int role)
{
    if (index.column() == 0 && role == Qt::ItemIsUserCheckable  ) {
        qDebug()<<"setData";
        if (Value == QIcon(":/images/mail-mark-read.png")) {
            articles_.at(index.row())->set_read(true);
            articles_.at(index.row())->saveOrUpdate();
        }
        else {
            articles_.at(index.row())->set_read(false);
            articles_.at(index.row())->saveOrUpdate();
        }

        return true;
    }

return false;
}

void ArticleListModel::switchToFeed(shared_ptr<Feed> feed) {
    Article::loadByFeed(feed, &articles_);
    reset();
}

Qt::ItemFlags ArticleListModel::flags(const QModelIndex& index) const {
    if (!index.isValid()) {
        return 0;
    }

    if (index.column() == 0) {
        return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;  //CheckBox
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


}  // namespace feed_reader
}  // namespace onyx
