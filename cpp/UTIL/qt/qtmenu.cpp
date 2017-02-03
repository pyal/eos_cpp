#include "qtmenu.h"


list<QWidget*> QtCheckBox::BuildWidget() {
    auto ret = new QCheckBox(Label.c_str());
    ret->setChecked(*DefValue);
    CreatedPtr = ret;
    return list<QWidget*>{ret};
}
void QtCheckBox::Connect(const QObject *receiver, const char *member){
    verify(CreatedPtr, "Widget is not created");
    CreatedPtr->connect(CreatedPtr, SIGNAL(toggled(bool)), this, SLOT(Update()));
    CreatedPtr->connect(this, SIGNAL(UpdateRemote(QtMenuElement*)), receiver, member);
};

void QtCheckBox::Update() {
    *DefValue = CreatedPtr->isChecked();
    emit UpdateRemote(this);
}

void QtLineEdit::Update() {
    *DefValue = CreatedPtr->text().toStdString();
    emit UpdateRemote(this);
}


list<QWidget*> QtSetBox::BuildWidget() {
    CreatedPtr = new QComboBox();
    for(auto &str: Items)  CreatedPtr->addItem(QString(str.c_str()));
    CreatedPtr->setCurrentIndex(*DefIndex);
    CreatedPtr->setToolTip(Label.c_str());
    return list<QWidget*>{CreatedPtr};
}
void QtSetBox::Connect(const QObject *receiver, const char *member) {
    verify(CreatedPtr, "Widget is not created");
    CreatedPtr->connect(CreatedPtr, SIGNAL(activated(int)), this, SLOT(Update()));
    CreatedPtr->connect(this, SIGNAL(UpdateRemote(QtMenuElement*)), receiver, member);
}
void QtSetBox::Update() {
    *DefIndex = CreatedPtr->currentIndex();
    emit UpdateRemote(this);

}
