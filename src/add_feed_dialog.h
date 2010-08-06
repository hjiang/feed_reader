// -*- mode: c++; c-basic-offset: 4; -*-

#ifndef ONYX_FEED_READER_ADD_FEED_DIALOG_H__
#define ONYX_FEED_READER_ADD_FEED_DIALOG_H__

#include <QDialog>
#include <QUrl>

class QLineEdit;
class QPushButton;

namespace ui {
class KeyBoard;
}

namespace onyx {
namespace feed_reader {

class AddFeedDialog : public QDialog {
    Q_OBJECT;

public:
    AddFeedDialog(QWidget *parent = 0);
    ~AddFeedDialog();

    const QUrl& url() {
        return url_;
    }

public slots:
    void addClicked();

private:
    friend class AcceptanceTest;

    ui::KeyBoard* key_board_;
    QPushButton *add_button_;
    QPushButton *cancel_button_;
    QLineEdit *url_edit_;
    QUrl url_;
};

}  // namespace feed_reader
}  // namespace onyx

#endif  // ONYX_FEED_READER_ADD_FEED_DIALOG_H__
