#include "ProjectionHistogramView.h"

using namespace genericinterface;
using namespace imagein;

ProjectionHistogramView::ProjectionHistogramView(const Image* image, const Rectangle* rect, int value, bool horizontal): AlternativeImageView(image), _rectangle(rect), _value(value), _horizontal(horizontal)
{
	_qwtPlot = new QwtPlot();
	_qwtPlot->setTitle("Projection Histogram");
	
	init();
}

ProjectionHistogramView::~ProjectionHistogramView()
{
	delete _principalPicker;
	delete _leftPicker;
	delete _rightPicker;
}

void ProjectionHistogramView::init()
{
    this->setMouseTracking(true); //Switch on mouse tracking (no need to press button)

    _qwtPlot->setTitle("Histogram");
    
    _qwtPlot->setCanvasBackground(QColor(Qt::gray));
    _qwtPlot->plotLayout()->setAlignCanvasToScales(true);

    _qwtPlot->setAxisTitle(QwtPlot::yLeft, "");
    _qwtPlot->setAxisTitle(QwtPlot::xBottom, "");

	QwtLegend* legend = new QwtLegend();
    legend->setItemMode(QwtLegend::CheckableItem);
    _qwtPlot->insertLegend(legend, QwtPlot::RightLegend);

    populate();
    
    _qwtPlot->canvas()->setMouseTracking(true);
    
    if(_horizontal)
		_principalPicker = new HistogramPicker(QwtPlotPicker::HLineRubberBand, QwtPicker::AlwaysOn, _qwtPlot->canvas());
    else
		_principalPicker = new HistogramPicker(QwtPlotPicker::VLineRubberBand, QwtPicker::AlwaysOn, _qwtPlot->canvas());
    _principalPicker->setStateMachine(new QwtPickerDragPointMachine());
    _principalPicker->setTrackerPen(QColor(Qt::white));
    _principalPicker->setRubberBandPen(QColor(Qt::yellow));
    
    _leftPicker = new HistogramPicker(_qwtPlot->canvas());
    _leftPicker->setStateMachine(new QwtPickerDragPointMachine());
    
    _rightPicker = new HistogramPicker(_qwtPlot->canvas());
    _rightPicker->setStateMachine(new QwtPickerDragPointMachine());
    _rightPicker->setRubberBand(QwtPlotPicker::VLineRubberBand);
    _rightPicker->setRubberBandPen(QColor(Qt::yellow));
	_rightPicker->setMousePattern(QwtPicker::MouseSelect1, Qt::RightButton);

    connect(_qwtPlot, SIGNAL(legendChecked(QwtPlotItem*, bool)), this, SLOT(showItem(QwtPlotItem*, bool)));
    connect(_rightPicker, SIGNAL(selected(const QPointF&)), this, SLOT(rightClick(const QPointF&)));
    connect(_leftPicker, SIGNAL(selected(const QPointF&)), this, SLOT(leftClick(const QPointF&)));
    connect(_principalPicker, SIGNAL(moved(const QPointF&)), this, SLOT(move(const QPointF&)));

    _qwtPlot->replot(); // creating the legend items

    QwtPlotItemList items = _qwtPlot->itemList(QwtPlotItem::Rtti_PlotHistogram);
    for(int i = 0; i < items.size(); i++)
    {
        if(i == 0)
        {
            QwtLegendItem* legendItem = qobject_cast<QwtLegendItem*>(legend->find(items[i]));
            if(legendItem)
                legendItem->setChecked(true);

            items[i]->setVisible(true);
        }
        else
        {
            items[i]->setVisible(false);
        }
    }

    _qwtPlot->setAutoReplot(true);
}

void ProjectionHistogramView::populate()
{
    QwtPlotGrid* grid = new QwtPlotGrid();
    grid->enableX(true);
    grid->enableY(false);
    grid->enableXMin(false);
    grid->enableYMin(false);
    grid->setMajPen(QPen(Qt::black, 0, Qt::DotLine));
    grid->attach(_qwtPlot);

	for(unsigned int i = 0; i < _image->getNbChannels(); ++i)
	{
		imagein::ProjectionHistogram histogram(*_image, _value, _horizontal, *_rectangle, i);
		int values[histogram.getWidth()];

		for(unsigned int j = 0; j < histogram.getWidth(); ++j)
			values[j] = histogram[j];
		
		GraphicalHistogram* graphicalHisto;
		switch(i)
		{
			case 0:
				graphicalHisto = new GraphicalHistogram("Red", Qt::red);
			break;
			case 1:
				graphicalHisto = new GraphicalHistogram("Green", Qt::green);
			break;
			case 2:
				graphicalHisto = new GraphicalHistogram("Blue", Qt::blue);
			break;
			case 3:
				graphicalHisto = new GraphicalHistogram("Alpha", Qt::black);
			break;
			default:
				graphicalHisto = new GraphicalHistogram("Default", Qt::black);
		}
		graphicalHisto->setValues(sizeof(values) / sizeof(int), values);
		if(_horizontal)
			graphicalHisto->setOrientation(Qt::Horizontal);
		graphicalHisto->attach(_qwtPlot);
	}
}

void ProjectionHistogramView::showItem(QwtPlotItem *item, bool on) const
{
    item->setVisible(on);
}

void ProjectionHistogramView::leftClick(const QPointF& pos) const
{
	if(_horizontal)
		emit(leftClickedValue((int)pos.y()));
	else
		emit(leftClickedValue((int)pos.x()));
}

void ProjectionHistogramView::rightClick(const QPointF& pos) const
{
    if(_horizontal)
		emit(rightClickedValue((int)pos.y()));
	else
		emit(rightClickedValue((int)pos.x()));
}

void ProjectionHistogramView::move(const QPointF& pos) const
{
	if(_horizontal)
		emit(hoveredValue((int)pos.y()));
	else
		emit(hoveredValue((int)pos.x()));
}
