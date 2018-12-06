#include "Model.h"
#include <random>

Model::Model() 
{
	mKanbanModel = std::make_shared<KanbanModel>();
}

void Model::loadData()
{
	const auto getRandomColor = [](QColor mix = {0, 0, 0}) -> QColor
	{
		std::random_device rd; 
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, 255);
		
		const int r = (dis(gen) + mix.red()) / 2;
		const int g = (dis(gen) + mix.green()) / 2;
		const int b = (dis(gen) + mix.blue()) / 2;
		const int a = 128;

		return {r, g, b, a};
	};

	const auto mix {Qt::blue};
	mKanbanModel->addKanban({"a_123", getRandomColor(mix), "A"});
	mKanbanModel->addKanban({"a_456", getRandomColor(mix), "A"});
	mKanbanModel->addKanban({"b_123", getRandomColor(mix), "A"});
	mKanbanModel->addKanban({"b_456", getRandomColor(mix), "B"});
	mKanbanModel->addKanban({"c_000", getRandomColor(mix), "C"});
	mKanbanModel->addKanban({"d_001", getRandomColor(mix), "C"});
	mKanbanModel->addKanban({"d_002", getRandomColor(mix), "D"});
	mKanbanModel->addKanban({"d_003", getRandomColor(mix), "D"});
	mKanbanModel->addKanban({"d_004", getRandomColor(mix), "D"});
	mKanbanModel->addKanban({"d_005", getRandomColor(mix), "D"});
	mKanbanModel->addKanban({"d_006", getRandomColor(mix), "E"});
	mKanbanModel->addKanban({"d_007", getRandomColor(mix), "E"});
	mKanbanModel->addKanban({"d_008", getRandomColor(mix), "E"});
	mKanbanModel->addKanban({"d_009", getRandomColor(mix), "E"});
}
