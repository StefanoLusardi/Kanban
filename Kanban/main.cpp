#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include <QStyleFactory>
//#include "../Style/DarkStyle.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	auto  k = QStyleFactory::keys();
	auto s = QStyleFactory::create("dark");
	QApplication::setStyle(s);

	//a.setStyle(new DarkStyle);
	MainWindow w;
	return w.run(a);
}
