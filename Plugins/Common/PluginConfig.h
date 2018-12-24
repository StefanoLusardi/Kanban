#pragma once

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtWidgets/QMessageBox>
#include <QCoreApplication>


namespace PluginConfigManager
{
	const QString configDir { qApp->applicationDirPath()+"/Config/" };

	inline QJsonObject parse(const QString& filePath)
	{
		QFile file;
		file.setFileName(PluginConfigManager::configDir + filePath);
		file.open(QIODevice::ReadOnly | QIODevice::Text);

		QJsonParseError jsonError{};
		QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll(), &jsonError);
		
		if (jsonError.error != QJsonParseError::NoError)
		{
			QMessageBox::information(
				nullptr, 
				"Plugin Configuration file parse error", 
				jsonError.errorString(), 
				QMessageBox::StandardButton::Ok, 
				QMessageBox::StandardButton::Ok);
		}

		return jsonDocument.object();
	}

	inline void write(const QString& filePath, QJsonObject& obj)
	{
		QJsonDocument jsonDocument(obj);

		QFile file;
		file.setFileName(PluginConfigManager::configDir + filePath);
		file.open(QIODevice::WriteOnly | QIODevice::Text);
		file.write(jsonDocument.toJson(QJsonDocument::JsonFormat::Indented));
	}
}