#pragma once
#include "ItemManager.h"
#include "SettingsItem.h"

class QVariant;
class QSqlDatabase;

class SettingsManager : public ItemManager<SettingsItem>
{
public:
	explicit SettingsManager(QSqlDatabase& db);

	void init() const override;
	void insertItem(SettingsItem& item) const override;
	void removeItem(int id) const override {}
	std::vector<SettingsItem> getItems(int id) const override;
	void removeAllItems(int pageId) const override {}
	void saveAllItems() const override {}
	void setData(int id, const char* property, const QVariant& value) const;

private:
    QSqlDatabase& mDb;
};

