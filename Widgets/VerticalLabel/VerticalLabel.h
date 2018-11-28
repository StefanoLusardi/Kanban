#pragma once

#include <QtCore/qglobal.h>
#if defined API_VERTICALLABEL
	#define MSLABEL_BUILD_DLL_SPEC Q_DECL_EXPORT
#else
	#define MSLABEL_BUILD_DLL_SPEC Q_DECL_IMPORT
#endif

//#include <QtUiPlugin/QDesignerExportWidget> // QDESIGNER_WIDGET_EXPORT

#include <QtWidgets/QLabel>

class MSLABEL_BUILD_DLL_SPEC VerticalLabel : public QLabel
{
	Q_OBJECT

public:
	VerticalLabel(QWidget *parent = Q_NULLPTR);
};
