// -*- mode: c++; c-basic-offset: 4; -*-

#include "feeds_page.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QKeyEvent>
#include <QLayout>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>

#include "onyx/screen/screen_proxy.h"

#include "add_feed_dialog.h"
#include "feed_list_model.h"
#include "singleton.h"
#include "widget_updater.h"

namespace onyx {
namespace feed_reader {

using onyx::screen::ScreenProxy;

FeedsPage::FeedsPage(FeedListModel* feed_list_model, QWidget* parent)
        : QWidget(parent),
          add_feed_dialog_(new AddFeedDialog(this)),
          feed_list_view_(new QTableView(this)),
          feed_list_model_(feed_list_model) {
    feed_list_view_->horizontalHeader()->hide();
    feed_list_view_->verticalHeader()->hide();
    feed_list_view_->setModel(feed_list_model);
    feed_list_view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // The feeds page has a list of subscribed feeds and three
    // buttons.
    QPushButton* add_feed_button(new QPushButton(this));
    QPushButton* refresh_button(new QPushButton(this));
    QPushButton* delete_feed_button(new QPushButton(this));
    QPushButton* quit_button(new QPushButton(this));

    // Set labels and size policies.
    add_feed_button->setText(tr("Add feed"));
    refresh_button->setText(tr("Refresh"));
    delete_feed_button->setText(tr("Delete feed"));
    quit_button->setText(tr("Quit"));
    QSizePolicy size_policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    feed_list_view_->setSizePolicy(size_policy);

    // Set up layout and sub-layout
    QLayout* button_layout = new QHBoxLayout;
    button_layout->addWidget(add_feed_button);
    button_layout->addWidget(refresh_button);
    button_layout->addWidget(delete_feed_button);
    button_layout->addWidget(quit_button);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(1, 0, 1, 0);
    layout->addWidget(feed_list_view_);
    layout->addLayout(button_layout);
    setLayout(layout);

    // Set up connections.
    connect(add_feed_button, SIGNAL(clicked()),
            this, SLOT(showAddFeedDialog()));
    connect(add_feed_dialog_, SIGNAL(accepted()),
            this, SLOT(addFeed()));
    connect(refresh_button, SIGNAL(clicked()),
            feed_list_model_, SLOT(refreshAllFeeds()));
    connect(quit_button, SIGNAL(clicked()),
            qApp, SLOT(quit()));

    connect(feed_list_view_, SIGNAL(activated(const QModelIndex&)),
            this, SLOT(handleActivated(const QModelIndex&)));
//    connect(feed_list_view_, SIGNAL(clicked(const QModelIndex&)),
//            this, SLOT(handleActivated(const QModelIndex&)));
    connect(feed_list_view_, SIGNAL(doubleClicked( const QModelIndex &)),
            this, SLOT(handleActivated(const QModelIndex&)));
    connect(delete_feed_button, SIGNAL(clicked()),
            this, SLOT(delFeed()));
    WidgetUpdater& updater(Singleton<WidgetUpdater>::instance());
    updater.addWidget(add_feed_button, ScreenProxy::GU);
    updater.addWidget(delete_feed_button, ScreenProxy::GU);
    updater.addWidget(refresh_button, ScreenProxy::GU);
    updater.addWidget(quit_button, ScreenProxy::GU);
    updater.addWidget(feed_list_view_, ScreenProxy::GU);

    // Retain pointers for testing
    add_feed_button_ = add_feed_button;
}

FeedsPage::~FeedsPage() {
}

void FeedsPage::showEvent(QShowEvent* event) {
    int total_width = 590;
    feed_list_view_->setColumnWidth(0, static_cast<int>(0.9 * total_width));
    feed_list_view_->setColumnWidth(
            1, total_width - feed_list_view_->columnWidth(0));
    QWidget::showEvent(event);
}

void FeedsPage::handleActivated(const QModelIndex& index) {
    emit feedActivated(
            feed_list_view_->model()->data(
                    index, FeedListModel::FeedIdentifierRole).toInt());
}

void FeedsPage::showAddFeedDialog() {
    QRect rect(add_feed_dialog_->geometry());
    rect.setX(0);
    rect.setWidth(width());
    add_feed_dialog_->setGeometry(rect);
    add_feed_dialog_->open();
}

void FeedsPage::addFeed() {
    feed_list_model_->addFeed(add_feed_dialog_->url());
}

void FeedsPage::deleteFeed() {
    int index = feed_list_view_->currentIndex().data(
    FeedListModel::FeedIdentifierRole).toInt();
    if (index < 0) {
          return;
    }
    shared_ptr<Feed> feed = feed_list_model_->getFeed(index);
    feed_list_model_->deleteFeed(feed);
    //TODO: refresh article list view; should emit a signal
    //emit deleted();
    }
}  // namespace feed_reader
}  // namespace onyx
