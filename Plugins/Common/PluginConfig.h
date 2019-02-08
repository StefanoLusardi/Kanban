#pragma once

#include <QFile>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtWidgets/QMessageBox>
#include <QCoreApplication>
#include <optional>
#include <QDir>

namespace PluginConfigManager
{
	const QString configDir { qApp->applicationDirPath()+"/Config/" };

	inline std::optional<QJsonObject> parse(const QString& filePath)
	{
		QFile file;
		file.setFileName(PluginConfigManager::configDir + filePath);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "Configuration file" + file.fileName() + " cannot be opened";
			return {};
		}

		QJsonParseError jsonError{};
		QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll(), &jsonError);
		
		if (jsonError.error != QJsonParseError::NoError)
		{
			QMessageBox::information(
				nullptr, 
				"Error: Plugin Configuration " + file.fileName(), 
				jsonError.errorString(),
				QMessageBox::StandardButton::Ok);
		}

		return jsonDocument.object();
	}

	template <typename Json>
	inline void write(const QString& filePath, const Json& json)
	{
		// Json can be QJsonDocument, QJsonObject or QJsonArray
		QJsonDocument jsonDocument(json);

		QDir dir;
		if (dir.mkpath(PluginConfigManager::configDir))
		{
			QFile file;
			file.setFileName(PluginConfigManager::configDir + filePath);
			if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				qDebug() << "Configuration file" + file.fileName() + " cannot be written.";
				return;
			}
			file.write(jsonDocument.toJson(QJsonDocument::JsonFormat::Indented));
		}
	}
}
