#include "UpdownThree.hpp"

UpdownThree::UpdownThree(TaskChooser& tc, QString which, QString title) :
QWidget(0, Qt::Window | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
tc(tc),
data(tc.dirs, which),
curAt(-1)
{
    setWindowTitle(title);

    QLabel *ql = new QLabel(tr("Oversæt følgende glossering til et ord i færdig form:"));

    QVBoxLayout *qvbl = new QVBoxLayout;
    qvbl->addWidget(ql);

    QVBoxLayout *curWord = new QVBoxLayout;
    up = new QLabel;
    up->setWordWrap(true);
    input = new QLineEdit;
    connect(input, SIGNAL(returnPressed()), this, SLOT(checkInput()));
    check = new QPushButton(tr("Check"));
    connect(check, SIGNAL(clicked()), this, SLOT(checkInput()));
    yield = new QPushButton(tr("Giv op..."));
    connect(yield, SIGNAL(clicked()), this, SLOT(yieldWord()));
    result = new QLabel;
    curWord->addWidget(up);
    curWord->addSpacing(5);
    curWord->addWidget(input);
    curWord->addWidget(check);
    curWord->addWidget(result);
    curWord->addWidget(yield);
    result->hide();
    yield->hide();

    QPushButton *nb = new QPushButton(tr("Gå til næste ord"));
    connect(nb, SIGNAL(clicked()), this, SLOT(showNext()));

    qvbl->addLayout(curWord);
    qvbl->addSpacing(15);
    qvbl->addWidget(nb);

    nb = new QPushButton(tr("Luk og gå til næste del"));
    connect(nb, SIGNAL(clicked()), this, SLOT(skipExercise()));
    qvbl->addSpacing(15);
    qvbl->addWidget(nb);

    setLayout(qvbl);

    showNext();
}

void UpdownThree::skipExercise() {
    tc.showNext(windowTitle());
    close();
}

void UpdownThree::showNext() {
    ++curAt;
    if (curAt >= data.updowns.size()) {
        QMessageBox mbox(QMessageBox::Question, tr("Færdig!"), tr("Der er ikke mere i denne del. Vil du fortsætte med næste del?"));
        QPushButton *yes = mbox.addButton(tr("Ja, næste del"), QMessageBox::YesRole);
        mbox.addButton(tr("Nej, tilbage til menuen"), QMessageBox::NoRole);
        mbox.exec();

        if (mbox.clickedButton() == yes) {
            tc.showNext(windowTitle());
        }
        close();
        return;
    }
    result->hide();
    yield->hide();
    input->setText("");
    input->setFocus();
    up->setText(QString("<center><h2>") + data.glossUpperShort(data.updowns.at(curAt).second) + "</h2></center>");
    adjustSize();
}

void UpdownThree::checkInput() {
    if (input->text() == data.updowns.at(curAt).first) {
        result->setText(QString("<center><span style='color: darkgreen;'><b>") + tr("Korrekt!") + "</b></span></center>");
        yield->hide();
    }
    else if (input->text().compare(data.updowns.at(curAt).first, Qt::CaseInsensitive) == 0) {
        result->setText(QString("<center><span style='color: darkyellow;'><b>") + tr("Næsten korrekt.\nStore og små bogstaver gælder...") + "</b></span></center>");
        yield->show();
    }
    else {
        result->setText(QString("<center><span style='color: darkred;'><b>") + tr("Ikke korrekt.\nPrøv igen...") + "</b></span></center>");
        yield->show();
    }
    result->show();
    yield->show();
    input->setFocus();
    input->selectAll();
    adjustSize();
}

void UpdownThree::yieldWord() {
    QMessageBox::information(0, "Hrhm...", QString("<h1>") + tr("Det korrekte færdige ord var:") + QString("</h1><br>") + data.updowns.at(curAt).first);
    showNext();
}
