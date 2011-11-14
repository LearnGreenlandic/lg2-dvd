#include "QATextOnly.hpp"

QATextOnly::QATextOnly(const dirmap_t& dirs, QString which, QString title) :
QWidget(0, Qt::Window | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
data(dirs, which),
curAt(-1)
{
    setWindowTitle(title);

    QVBoxLayout *qvbl = new QVBoxLayout;

    QLabel *ql = new QLabel(tr(which.toStdString().c_str()));
    ql->setWordWrap(true);
    qvbl->addWidget(ql);
    qvbl->addSpacing(5);

    query = new QLabel;
    qvbl->addWidget(query);

    qvbl->addSpacing(5);

    input = new QLineEdit;
    connect(input, SIGNAL(returnPressed()), this, SLOT(checkInput()));
    QPushButton *check = new QPushButton(tr("Check"));
    connect(check, SIGNAL(clicked()), this, SLOT(checkInput()));

    yield = new QPushButton(tr("Giv op..."));
    connect(yield, SIGNAL(clicked()), this, SLOT(yieldWord()));

    result = new QLabel;
    qvbl->addWidget(new QLabel(tr("Skriv svaret:")));
    qvbl->addWidget(input);
    qvbl->addWidget(check);
    qvbl->addWidget(result);
    qvbl->addWidget(yield);
    result->hide();
    yield->hide();

    qvbl->addSpacing(15);

    QPushButton *nb = new QPushButton(tr("Gå til næste ord"));
    connect(nb, SIGNAL(clicked()), this, SLOT(showNext()));
    qvbl->addWidget(nb);

    setLayout(qvbl);

    showNext();
}

void QATextOnly::showNext() {
    ++curAt;
    if (curAt >= data.size()) {
        QMessageBox mbox(QMessageBox::Question, tr("Færdig!"), tr("Der er ikke mere i denne øvelse. Vil du fortsætte med næste øvelse?"));
        QPushButton *yes = mbox.addButton(tr("Ja, næste øvelse"), QMessageBox::YesRole);
        mbox.addButton(tr("Nej, tilbage til menuen"), QMessageBox::NoRole);
        mbox.exec();

        if (mbox.clickedButton() == yes) {
            //tc.showStructureTwo();
        }
        close();
        return;
    }
    QString text = QString("<b>") + data.getQ(curAt) + "</b>";
    if (!data.getStar(curAt).isEmpty()) {
        text += QString("<br/><i>") + data.getStar(curAt) + "</i>";
    }
    query->setText(text);
    result->hide();
    yield->hide();
    input->setText("");
    input->setFocus();
    adjustSize();
}

void QATextOnly::checkInput() {
    if (input->text() == data.getA(curAt)) {
        result->setText(QString("<center><span style='color: darkgreen;'><b>") + tr("Korrekt!") + "</b></span></center>");
        yield->hide();
    }
    else if (input->text().compare(data.getA(curAt), Qt::CaseInsensitive) == 0) {
        result->setText(QString("<center><span style='color: darkyellow;'><b>") + tr("Næsten korrekt.\nStore og små bogstaver gælder...") + "</b></span></center>");
        yield->show();
    }
    else {
        result->setText(QString("<center><span style='color: darkred;'><b>") + tr("Ikke korrekt.\nPrøv igen...") + "</b></span></center>");
        yield->show();
    }
    result->show();
    input->setFocus();
    input->selectAll();
    adjustSize();
}

void QATextOnly::yieldWord() {
    QMessageBox::information(0, tr("Hrhm..."), QString("<h1>") + tr("Det korrekte ord var:") + QString("</h1><br>") + data.getA(curAt));
    showNext();
}
