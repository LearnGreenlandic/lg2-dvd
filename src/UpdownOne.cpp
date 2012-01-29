#include "UpdownOne.hpp"

UpdownOne::UpdownOne(TaskChooser& tc, QString which, QString title) :
QWidget(0, Qt::Window | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
tc(tc),
data(tc.dirs, which),
curAt(-1)
{
    setWindowTitle(title);

    QLabel *ql = new QLabel(tr("Prøv at forstå og at udtale følgende ord:"));

    QVBoxLayout *qvbl = new QVBoxLayout;
    qvbl->addWidget(ql);

    QVBoxLayout *curWord = new QVBoxLayout;
    down = new QLabel;
    up = new QLabel;
    help = new QPushButton(tr("Vis hjælp"));
    connect(help, SIGNAL(clicked()), this, SLOT(showHelp()));
    curWord->addWidget(down);
    curWord->addSpacing(5);
    curWord->addWidget(help);
    curWord->addWidget(up);
    up->hide();

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

void UpdownOne::skipExercise() {
    tc.showNext(windowTitle());
    close();
}

void UpdownOne::showNext() {
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
    up->hide();
    help->show();
    down->setText(QString("<center><h2>") + data.updowns.at(curAt).first + "</h2></center>");
    adjustSize();
}

void UpdownOne::showHelp() {
    help->hide();
    up->setText(QString("<center><i>") + QString(data.updowns.at(curAt).second).replace("<", "&lt;").replace(">", "&gt;") + "</i></center>");
    up->setToolTip(data.glossUpperDetailed(data.updowns.at(curAt).second));
    up->show();
    adjustSize();
}
