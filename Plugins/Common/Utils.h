#pragma once

class QString;
class QColor;

#include <random>

namespace Utils
{
	inline QString getUniqueName(const QString& name, const QStringList& allNames)
	{
		QString tryThis = name;
		const auto stringsMatch = [&tryThis](const QString& exists) -> bool { return tryThis == exists; };

		int i = 0;
		while (std::count_if(allNames.begin(), allNames.end(), stringsMatch) > 0)
		{
			tryThis = name + " (" + QString::number(++i) + ")";
		}

		return tryThis;
	}

	inline QString colorToString(const QColor& color) { return color.name(QColor::NameFormat::HexArgb); }

	inline QColor stringToColor(const QString& str) { return QColor(str); }

	inline QColor getRandomColor(const QColor& mix = Qt::white)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		const std::uniform_int_distribution<> dis(0, 255);

		const int r = (dis(gen) + mix.red()) / 2;
		const int g = (dis(gen) + mix.green()) / 2;
		const int b = (dis(gen) + mix.blue()) / 2;
		const int a = 128;

		return {r, g, b, a};
	}

}