#include "PageModel.h"

PageModel::PageModel(DbManager& db) : mDb{db}
{
	mPages[0] = QString("Page_1");
	mPages[1] = QString("Page_2");
}
