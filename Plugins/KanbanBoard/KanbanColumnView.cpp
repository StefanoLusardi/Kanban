#include "ui_KanbanColumnView.h"
#include "KanbanColumnView.h"

#include <QDebug>
#include <QPropertyAnimation>
#include <QStyledItemDelegate>
#include <QSortFilterProxyModel>
#include <QGraphicsOpacityEffect>
#include <QColor>

KanbanColumnView::KanbanColumnView(const QString& title, const QColor& columnColor, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KanbanColumnView),
	mTitle{title},
	mColor{columnColor}
{
    ui->setupUi(this);
    ui->mButtonSpoiler->setCheckable(true);
    ui->mLabelTitleVertical->setVisible(false);

	setupListView();
	setTitle(title);
	setColor(columnColor);
	
	setMinimumWidth(mCollapsedWidth);

	// TODO: cleanup the animation setup. Too verbose.
    mAnimation.addAnimation(new QPropertyAnimation(this, "minimumWidth"));
    static_cast<QPropertyAnimation*>(mAnimation.animationAt(0))->setDuration(mAnimationTime);
    static_cast<QPropertyAnimation*>(mAnimation.animationAt(0))->setEasingCurve(QEasingCurve::Linear);
    
    mAnimation.addAnimation(new QPropertyAnimation(this, "maximumWidth"));
	static_cast<QPropertyAnimation*>(mAnimation.animationAt(1))->setDuration(mAnimationTime);
    static_cast<QPropertyAnimation*>(mAnimation.animationAt(1))->setEasingCurve(QEasingCurve::Linear);	

	QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect(this);
	ui->mLabelTitleVertical->setGraphicsEffect(opacity);
	mAnimation.addAnimation(new QPropertyAnimation(opacity,"opacity"));
	static_cast<QPropertyAnimation*>(mAnimation.animationAt(2))->setDuration(mAnimationTime);
	static_cast<QPropertyAnimation*>(mAnimation.animationAt(2))->setEasingCurve(QEasingCurve::Linear);

	QGraphicsOpacityEffect *opacity2 = new QGraphicsOpacityEffect(this);
	ui->mLabelTitle->setGraphicsEffect(opacity2);
	mAnimation.addAnimation(new QPropertyAnimation(opacity2,"opacity"));
	static_cast<QPropertyAnimation*>(mAnimation.animationAt(3))->setDuration(mAnimationTime);
	static_cast<QPropertyAnimation*>(mAnimation.animationAt(3))->setEasingCurve(QEasingCurve::Linear);

	QGraphicsOpacityEffect *opacity3 = new QGraphicsOpacityEffect(this);
	ui->mListView->setGraphicsEffect(opacity3);
	mAnimation.addAnimation(new QPropertyAnimation(opacity3,"opacity"));
	static_cast<QPropertyAnimation*>(mAnimation.animationAt(4))->setDuration(mAnimationTime);
	static_cast<QPropertyAnimation*>(mAnimation.animationAt(4))->setEasingCurve(QEasingCurve::Linear);

    connect(ui->mButtonSpoiler, &QToolButton::clicked, [this](bool isToggled)
    {
        mIsCollapsed = isToggled;

		if (mIsCollapsed)
		{
			// If we are collapsing, the current width is the width of the expanded column.
			// Store the expanded width to be used when triggering next expansion.
			mExpandedWidth = width();
		}

        const auto targetWidth = mIsCollapsed ? mCollapsedWidth : mExpandedWidth;
        ui->mButtonSpoiler->setArrowType(mIsCollapsed ? Qt::RightArrow : Qt::LeftArrow);
		
    	static_cast<QPropertyAnimation*>(mAnimation.animationAt(0))->setStartValue(width());
		static_cast<QPropertyAnimation*>(mAnimation.animationAt(0))->setEndValue(targetWidth);
			
		static_cast<QPropertyAnimation*>(mAnimation.animationAt(1))->setStartValue(width());
		static_cast<QPropertyAnimation*>(mAnimation.animationAt(1))->setEndValue(targetWidth);

		static_cast<QPropertyAnimation*>(mAnimation.animationAt(2))->setStartValue(!mIsCollapsed);
		static_cast<QPropertyAnimation*>(mAnimation.animationAt(2))->setEndValue(mIsCollapsed);
		
		static_cast<QPropertyAnimation*>(mAnimation.animationAt(3))->setStartValue(mIsCollapsed);
		static_cast<QPropertyAnimation*>(mAnimation.animationAt(3))->setEndValue(!mIsCollapsed);

        static_cast<QPropertyAnimation*>(mAnimation.animationAt(4))->setStartValue(mIsCollapsed);
		static_cast<QPropertyAnimation*>(mAnimation.animationAt(4))->setEndValue(!mIsCollapsed);
    	
    	mAnimation.start();
    });

    connect(&mAnimation, &QParallelAnimationGroup::finished, [this, &parent]()
    {
		ui->mListView->setVisible(!mIsCollapsed);
        ui->mLabelTitle->setVisible(!mIsCollapsed);        
        ui->mButtonOptions->setVisible(!mIsCollapsed);
        ui->mLabelTitleVertical->setVisible(mIsCollapsed);

		if (!mIsCollapsed)
		{
			// when not collapsed remove max/min width settings
			setMinimumWidth(mCollapsedWidth);
			setMaximumWidth(std::numeric_limits<int>::max());
		}
    });

    connect(ui->mButtonOptions, &QToolButton::clicked, [this](bool)
    {
        emit columnDeleted(mTitle);
    });	

	connect(ui->mButtonAdd, &QPushButton::clicked, [this](bool)
	{
		emit kanbanCreated(mTitle);
	});
}

KanbanColumnView::~KanbanColumnView()
{
    qDebug() << "Deleted " << getTitle();
    delete ui;
}

void KanbanColumnView::setTitle(const QString& title)
{
	mTitle = title;
	ui->mLabelTitle->setText(mTitle);
    ui->mLabelTitleVertical->setText(mTitle);
}

QString KanbanColumnView::getTitle() const
{
    return mTitle;
}

void KanbanColumnView::setupListView() const
{
	// TODO: check if all this settings are needed or if it's possible to set them from the designer

	ui->mListView->setSpacing(5);
	//ui->mListView->setResizeMode(QListView::Adjust);
	//ui->mListView->setFlow(QListView::TopToBottom);
	//ui->mListView->setUniformItemSizes(false);
	//ui->mListView->setMovement(QListView::Free);
	//ui->mListView->setMouseTracking(true);
	ui->mListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	ui->mListView->setDragEnabled(true);
	ui->mListView->setAcceptDrops(true);
	//ui->mListView->setDropIndicatorShown(true);
	ui->mListView->setDefaultDropAction(Qt::MoveAction);
	//ui->mListView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
 //   ui->mListView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void KanbanColumnView::setModel(QSortFilterProxyModel* model) const
{
	ui->mListView->setModel(model);
}

void KanbanColumnView::setDelegate(QStyledItemDelegate* delegate) const
{
	ui->mListView->setItemDelegate(delegate);
}

void KanbanColumnView::setColor(const QColor& color)
{
	mColor = color;
	ui->mLabelTitle->setBackgroudColor(mColor);
	ui->mLabelTitleVertical->setBackgroudColor(mColor);

	const auto textColor {color.darker()};
	ui->mLabelTitle->setTextColor(textColor);
	ui->mLabelTitleVertical->setTextColor(textColor);
}

QColor KanbanColumnView::getColor() const
{
	return mColor;
}

void KanbanColumnView::mouseDoubleClickEvent(QMouseEvent*)
{
	ui->mButtonSpoiler->click();
}
