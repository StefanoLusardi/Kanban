#include "Model.h"
#include "DbManager.h"
#include <random>

Model::Model() 
{
	mKanbanModel = std::make_shared<KanbanModel>(DbManager::instance(), 0);
}

//QString Model::getRandomColor(const QString& mix)
//{
//	//std::random_device rd; 
//	//std::mt19937 gen(rd());
//	//const std::uniform_int_distribution<> dis(0, 255);
//	//	
//	//const int r = (dis(gen) + mix.red()) / 2;
//	//const int g = (dis(gen) + mix.green()) / 2;
//	//const int b = (dis(gen) + mix.blue()) / 2;
//	//const int a = 128;
//
//	return QString("#123456");
//}

//QColor Model::getRandomColor(const QColor& mix)
//{
//	std::random_device rd; 
//	std::mt19937 gen(rd());
//	const std::uniform_int_distribution<> dis(0, 255);
//		
//	const int r = (dis(gen) + mix.red()) / 2;
//	const int g = (dis(gen) + mix.green()) / 2;
//	const int b = (dis(gen) + mix.blue()) / 2;
//	const int a = 128;
//
//	return {r, g, b, a};
//}

void Model::loadData() const
{
	/*mPageModel->loadPages();
	for (auto&& page : mPageModel->getPages())
	{
		mKanbanModel->loadKanbanItems(pagr);
	}*/

	mKanbanModel->loadKanbanItems(0);
	
	/*
	const auto mix {getRandomColor()};
	mKanbanModel->addKanban({"a_000", getRandomColor(mix), "A"});
	mKanbanModel->addKanban({"a_001", getRandomColor(mix), "A"});
	mKanbanModel->addKanban({"a_002", getRandomColor(mix), "A"});

	mKanbanModel->addKanban({"b_000", getRandomColor(mix), "B"});
	mKanbanModel->addKanban({"b_001", getRandomColor(mix), "B"});
	
	mKanbanModel->addKanban({"c_000", getRandomColor(mix), "C"});
	mKanbanModel->addKanban({"c_001", getRandomColor(mix), "C"});
	
	mKanbanModel->addKanban({"d_000", getRandomColor(mix), "D"});
	mKanbanModel->addKanban({"d_001", getRandomColor(mix), "D"});
	mKanbanModel->addKanban({"d_002", getRandomColor(mix), "D"});
	mKanbanModel->addKanban({"d_003", getRandomColor(mix), "D"});
	
	mKanbanModel->addKanban({"e_000", getRandomColor(mix), "E"});
	mKanbanModel->addKanban({"e_001", getRandomColor(mix), "E"});
	mKanbanModel->addKanban({"e_002", getRandomColor(mix), "E"});
	mKanbanModel->addKanban({"e_003", getRandomColor(mix), "E"});*/
}
