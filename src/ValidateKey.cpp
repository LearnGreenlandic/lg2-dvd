#include "common.hpp"
#include "ValidateKey.hpp"
#include <QNetworkRequest>
#include <QNetworkReply>

ValidateKey::ValidateKey() :
QDialog(0, Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint)
{
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle(tr("Validering af licensnøgle"));
    setMinimumWidth(500);

    nam = new QNetworkAccessManager(this);
    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));

    QVBoxLayout *qvbl = new QVBoxLayout;

    QLabel *label = new QLabel(tr("license key text"));
    label->setWordWrap(true);
    label->setTextFormat(Qt::RichText);
    label->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::LinksAccessibleByKeyboard);
    label->setOpenExternalLinks(true);
    qvbl->addWidget(label);

    qvbl->addSpacing(15);

    email1 = new QLineEdit;
    connect(email1, SIGNAL(returnPressed()), this, SLOT(checkInput()));
    email2 = new QLineEdit;
    connect(email2, SIGNAL(returnPressed()), this, SLOT(checkInput()));
    lkey = new QLineEdit;
    connect(lkey, SIGNAL(returnPressed()), this, SLOT(checkInput()));

    QFormLayout *form = new QFormLayout;
    form->addRow(tr("Email"), email1);
    form->addRow(tr("Gentag email"), email2);
    form->addRow(tr("Licensnøgle"), lkey);
    qvbl->addLayout(form);

    result = new QLabel;
    qvbl->addWidget(result);

    QPushButton *check = new QPushButton(tr("Valider online"));
    connect(check, SIGNAL(clicked()), this, SLOT(checkInput()));
    qvbl->addWidget(check);

    QPushButton *close = new QPushButton(tr("Afslut program"));
    connect(close, SIGNAL(clicked()), this, SLOT(bailOut()));
    qvbl->addWidget(close);

    setLayout(qvbl);
}

void ValidateKey::checkInput() {
    if (email1->text().simplified() != email2->text().simplified()) {
        result->setText(tr("Email-addresserne er ikke ens!"));
        return;
    }
    result->setText(tr("... checker licensnøgle via learngreenlandic.com ..."));

    QNetworkRequest request(QUrl("http://learngreenlandic.com/callback/"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrl params;
    params.addQueryItem("app", "LG2");
    params.addQueryItem("revision", QVariant(lg2_revision).toString());
    params.addQueryItem("email", email1->text().simplified());
    params.addQueryItem("key", lkey->text().simplified());
    params.addQueryItem("language", tr("danish"));

#if defined(Q_WS_WIN)
    params.addQueryItem("os", "win");
#elif defined(Q_WS_MAC)
    params.addQueryItem("os", "mac");
#else
    params.addQueryItem("os", "linux");
#endif

    QByteArray data;
    data.append(params.toString());
    data.remove(0, 1);

    nam->post(request, data);

    adjustSize();
}

void ValidateKey::bailOut() {
    QCoreApplication::quit();
    close();
}

void ValidateKey::finished(QNetworkReply *reply) {
    QString rv = reply->readAll();

    if (rv[0] == 'P') {
        QSettings settings;
        settings.setValue("license_key", lkey->text().simplified());
        settings.setValue("encryption_key", rv.trimmed());
        close();
        return;
    }

    result->setText(rv);
    adjustSize();
}
