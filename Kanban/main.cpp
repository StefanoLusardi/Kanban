#include "KanbanCore.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	KanbanCore core;
	return core.run();
}
