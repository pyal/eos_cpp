#pragma once


#include <QtCharts/QChartView>
#include <cpp/lib/Ref/refmanip.h>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include "qtdata.h"
#include <functional>
#include <QColorDialog>


QT_CHARTS_USE_NAMESPACE

class QtLabel: public QtMenuElement {
    string Label;
public:
    QtLabel(const string &label): Label(label){}
    virtual list<QWidget*> BuildWidget() {
        return list<QWidget*>{new QLabel(Label.c_str())};
    }
    virtual void Connect(const QObject *receiver, const char *member){};

};

class QtCheckBox: public QtMenuElement {
    string Label;
    bool *DefValue;
    QCheckBox* CreatedPtr;
    Q_OBJECT
public:
    QtCheckBox(const string &label, bool *defValue): Label(label), DefValue(defValue), CreatedPtr(nullptr){}
    virtual list<QWidget*> BuildWidget();
    virtual void Connect(const QObject *receiver, const char *member);
private: Q_SIGNALS:
    void UpdateRemote(QtMenuElement*);
private Q_SLOTS:
    void Update();

};

class QtSetBox: public QtMenuElement {
    string Label;
    int *DefIndex;
    vector<string> Items;
    QComboBox* CreatedPtr;
    Q_OBJECT
public:
    QtSetBox(const string &label, vector<string> &items, int &defIndex): Label(label), DefIndex(&defIndex),
            Items(items), CreatedPtr(nullptr){}
    virtual list<QWidget*> BuildWidget();
    virtual void Connect(const QObject *receiver, const char *member);
private: Q_SIGNALS:
    void UpdateRemote(QtMenuElement*);
private Q_SLOTS:
    void Update();

};

class QtLineEdit: public QtMenuElement {
    string Label;
    string *DefValue;
    QLineEdit* CreatedPtr;

    Q_OBJECT
public:
    QtLineEdit(const string &label, string &defValue): Label(label), DefValue(&defValue), CreatedPtr(nullptr){}
    virtual list<QWidget*> BuildWidget() {
        auto ret = new QLineEdit(Label.c_str());
        ret->setText(DefValue->c_str());
        CreatedPtr = ret;
        return list<QWidget*>{ret};
    }
    virtual void Connect(const QObject *receiver, const char *member){
        verify(CreatedPtr, "Widget is not created");
        CreatedPtr->connect(CreatedPtr, SIGNAL(textChanged(const QString&)), this, SLOT(Update()));
        CreatedPtr->connect(this, SIGNAL(UpdateRemote(QtMenuElement*)), receiver, member);
    };
public: Q_SIGNALS:
    void UpdateRemote(QtMenuElement*);
public Q_SLOTS:
    void Update();
};

class QtMenu: public QWidget {
public:
    using update_function = function<void(QtMenuElement*)>;
private:
    update_function Update;
    list<QtMenuElement*> Elements;
    list<QWidget*> BaseElements;

    Q_OBJECT
public:
    explicit QtMenu(const list<QtMenuElement*> &elements, update_function update, const list<QWidget*> &baseElements,
            QWidget *parent = 0):
            QWidget(parent), Update(update), Elements(elements), BaseElements(baseElements)
    {
        list<QWidget*> widgets;
        for(auto &elemnt: Elements) {
            for (auto &e: elemnt->BuildWidget()) widgets.push_back(e);
        }
        for(auto &elemnt: Elements) elemnt->Connect(this, SLOT(updateUI(QtMenuElement*)));
        // create layout
        QGridLayout *baseLayout = new QGridLayout();
        QHBoxLayout *settingsLayout = new QHBoxLayout();


        settingsLayout->setAlignment(Qt::AlignLeft);
//        settingsLayout->addStretch();
        baseLayout->addLayout(settingsLayout, 0, 0, 1, 3);

        for(auto w: widgets) settingsLayout->addWidget(w);
        for(auto w: BaseElements) baseLayout->addWidget(w);
        setLayout(baseLayout);

    }
private:
    void connectSignals() {
        for(auto &elemnt: Elements) {
            elemnt->Connect(this, SLOT(updateUI(QtMenuElement*)));
        }
    }

private Q_SLOTS:
    void updateUI(QtMenuElement* element) {
        Update(element);
    }


};