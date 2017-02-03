#pragma once


#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <cpp/lib/Ref/refmanip.h>
#include <QValueAxis>
#include <QColorDialog>

#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>

#include "qtdata.h"
#include "qtmenu.h"
#include <functional>

QT_CHARTS_USE_NAMESPACE

class QtEvents: public QChartView {
    bool m_isTouching;
    void print(const string &str) {
        qInfo("%s", str.c_str());
    }
    using update_function = function<void()>;

    QGraphicsSimpleTextItem *m_coordX;
    QGraphicsSimpleTextItem *m_coordY;
    bool xLog, yLog;
    update_function UpdatePoints;
public:
    struct SRect {
        double MinX, MaxX, MinY, MaxY;
        string ToString() {
            return Stroka("X [") + MinX + ", " + MaxX + "] Y [" + MinY + ", " +  MaxY + "]";
        }
    };
    SRect PlotRect, DrawRect;
    QtEvents(QChart *chart, update_function update, QWidget *parent = 0):
            QChartView(chart, parent), m_isTouching(false), UpdatePoints(update){
        setRubberBand(QChartView::RectangleRubberBand);
        this->viewport()->setMouseTracking(true);
        yLog = xLog = false;

        m_coordX = new QGraphicsSimpleTextItem(chart);
        m_coordX->setText("X: ");
        m_coordY = new QGraphicsSimpleTextItem(chart);
        m_coordY->setText("Y: ");
    }
    void SetScale(SRect plotRect, SRect drawRect) {
        PlotRect = plotRect;
        DrawRect = drawRect;
    }
    bool viewportEvent(QEvent *event) {
        if (event->type() == QEvent::TouchBegin) {
            m_isTouching = true;
            chart()->setAnimationOptions(QChart::NoAnimation);
        }
        return QChartView::viewportEvent(event);
    }
    void mousePressEvent(QMouseEvent *event) {
        QChartView::mousePressEvent(event);
    }
    void mouseMoveEvent(QMouseEvent *event) {
        m_coordX->setText(QString("X: %1").arg(chart()->mapToValue(event->pos()).x()));
        m_coordY->setText(QString("Y: %1").arg(chart()->mapToValue(event->pos()).y()));
        QChartView::mouseMoveEvent(event);
    }
    void mouseReleaseEvent(QMouseEvent *event) {
        if (m_isTouching) m_isTouching = false;
        chart()->setAnimationOptions(QChart::SeriesAnimations);
        QChartView::mouseReleaseEvent(event);
    }
    void keyPressEvent(QKeyEvent *event) {
        switch (event->key()) {
            case Qt::Key_Plus: chart()->zoomIn(); break;
            case Qt::Key_Minus: chart()->zoomOut(); break;
            case Qt::Key_Left: chart()->scroll(-10, 0); break;
            case Qt::Key_Right: chart()->scroll(10, 0); break;
            case Qt::Key_Up: chart()->scroll(0, 10); break;
            case Qt::Key_Down: chart()->scroll(0, -10); break;
            case Qt::Key_Equal: chart()->zoomReset(); break;
            case Qt::Key_Escape: exit(0); break;
            case Qt::Key_K: setLogAxis(true); break;
            case Qt::Key_L: setLogAxis(false); break;
            case Qt::Key_Space: UpdatePoints(); break;
            default: QGraphicsView::keyPressEvent(event); break;
        }
    }
    void resizeEvent(QResizeEvent *event) {
        if (scene()) {
            scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
            chart()->resize(event->size());
            m_coordX->setPos(chart()->size().width()/2 - 50, chart()->size().height() - 20);
            m_coordY->setPos(chart()->size().width()/2 + 50, chart()->size().height() - 20);
        }
        QGraphicsView::resizeEvent(event);
    }
    void resetAxis() {
        xLog = !xLog;
        yLog = !yLog;
        setLogAxis(true);
        setLogAxis(false);
    }
    void setLogAxis(bool x) {
        if ((x && (xLog || PlotRect.MinX <= 0)) || (!x && (yLog || PlotRect.MinY <= 0))) setLogAxis(new QValueAxis, x);
        else setLogAxis(new QLogValueAxis, x);
        x?xLog = !xLog : yLog = !yLog;
    }
    void setLogAxis(QAbstractAxis *axis, bool x) {
        axis->setTitleText(x?"X":"Y");
        x?chart()->setAxisX(axis):chart()->setAxisY(axis);
        for(auto serie:chart()->series()) {
            serie->attachAxis(axis);
        }
        if(x) axis->setRange(DrawRect.MinX, DrawRect.MaxX);
        else axis->setRange(DrawRect.MinY, DrawRect.MaxY);
//        chart()->zoomReset();
    }

};

class QtMenuAny: public QtMenuElement {
    QWidget *Remote;
public:
    QtMenuAny(QWidget *remote): Remote(remote){}
    virtual list<QWidget*> BuildWidget() {
        return list<QWidget*>{Remote};
    }
    virtual void Connect(const QObject *receiver, const char *member){};

};


class QtPlot {
public:
    Ref<QtData::IDataSource> DataSource;
    string plotName;
    int startColor, editLineIndex;
    QChart *Chart;
    QtEvents *ChartEvents;
    struct SConfig {
        bool DrawLine;
        QColor LineColor;
        SConfig(): DrawLine(true), LineColor(""){};
    };
    vector<SConfig> ConfigData;
    QtSetBox *SelectorPtr;



    list<QtMenuElement*> Elements;
    QtPlot(QtData::IDataSource *dataSource, int color = 0x059000):
            DataSource(dataSource), startColor(color), ChartEvents(nullptr) {
    };
    bool LoadData() {
        if (!DataSource->getNext()) {
            log_always("no new data");
            return false;
        }
        UpdateData(nullptr);
        return true;
    }
    pair<double, double> SetLimit(double low, double up) {
        double scale = up - low;
        if(fabs(scale) < MathZer) return {low - 1, low + 1};
        double coef = 1;
        while(scale * coef < 20) if (scale * coef > 10) coef *= 2; else coef *= 10;
        while(scale * coef > 20) if (scale * coef < 40) coef /= 2; else coef /= 10;

        double step = 1./ coef;
        double rlow = int(low / step - 1 )*step;
        double rup =  int(up / step + 1 )*step;
        return {rlow, rup};
        
    }
    void SelectColor() {
        auto color = QColorDialog::getColor(ConfigData[editLineIndex].LineColor);
        if(color.isValid()) ConfigData[editLineIndex].LineColor = color;
        log_always("Selected color " + color.name().toStdString());

    }
    bool UpdateData(QtMenuElement *updated_element) {
        if(updated_element == SelectorPtr) {
            log_always("Updating color index " + to_string(editLineIndex));
            SelectColor();
        }
        if (ConfigData.size() == 0) SetMenuParams();
        Chart->removeAllSeries();
        pair<string, vector<QtData::Data>> points = DataSource->getData();
        QtEvents::SRect plotRect = QtEvents::SRect{1e300, -1e300, 1e300, -1e300};
        for(size_t i = 0; i < points.second.size(); i++) {
            if(!ConfigData[i].DrawLine) continue;
            QLineSeries *series0 = new QLineSeries();
            for(auto &pnt: points.second[i].pnts) {
                *series0 << QPointF(pnt.first, pnt.second);
                if(pnt.first > plotRect.MaxX) plotRect.MaxX = pnt.first;
                if(pnt.first < plotRect.MinX) plotRect.MinX = pnt.first;
                if(pnt.second > plotRect.MaxY) plotRect.MaxY = pnt.second;
                if(pnt.second < plotRect.MinY) plotRect.MinY = pnt.second;
            }
            QPen pen(ConfigData[i].LineColor);
            pen.setWidth(3);
            series0->setPen(pen);
            series0->setName(points.second[i].name.c_str());
            Chart->addSeries(series0);
        }
        Chart->setTitle(points.first.c_str());
        verify(ChartEvents, "Have to set scale");

        auto x = SetLimit(plotRect.MinX, plotRect.MaxX);
        auto y = SetLimit(plotRect.MinY, plotRect.MaxY);
        QtEvents::SRect drawRect = {x.first, x.second, y.first, y.second};
        ChartEvents->SetScale(plotRect, drawRect);

        ChartEvents->resetAxis();
        return true;
    }
    void SetMenuParams() {
        pair<string, vector<QtData::Data>> points = DataSource->getData();
        vector<QColor> colorArray{0x22ff07, 0x0c37ff, 0xfc2d07, 0x21fffc, 0x207f05, 0x207805, 0x684db2};
        QColor curColor = startColor;
        ConfigData.clear();
        ConfigData.resize(points.second.size());
        vector<string> lineNames;
        for(size_t i = 0; i < points.second.size(); i++) {
            curColor = colorArray[i % colorArray.size()];
            ConfigData[i].LineColor = QColor(curColor);
            QtCheckBox *box = new QtCheckBox(points.second[i].name, &ConfigData[i].DrawLine);
            lineNames.push_back(points.second[i].name);
            Elements.push_back(box);
        }
        SelectorPtr = new QtSetBox("SetColor", lineNames, editLineIndex);
        Elements.push_back(SelectorPtr);
    }

    void Plot() {
        int argc;
        QApplication app(argc, nullptr);

        Chart = new QChart();

        Chart->createDefaultAxes();
        ChartEvents = new QtEvents(Chart, [this]{LoadData();});
        if (!LoadData()) throw info_except("There is no data");
        ChartEvents->setRenderHint(QPainter::Antialiasing);
        QtMenu *menuView = new QtMenu(Elements, [this](QtMenuElement*el){UpdateData(el);}, {ChartEvents});

        QMainWindow window;
        window.setCentralWidget(menuView);
        window.resize(800, 600);
        window.show();

        app.exec();

    }
};
