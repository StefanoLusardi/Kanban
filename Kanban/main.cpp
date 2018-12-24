#include "KanbanCore.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	KanbanCore core;
	return core.run(a);
}
