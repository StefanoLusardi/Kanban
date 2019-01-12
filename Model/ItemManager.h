#pragma once

#include <vector>

template <typename Item>
class ItemManager
{
public:
	virtual ~ItemManager() = default;

	virtual void init() const = 0;
    virtual void insertItem(Item& item) const = 0;
    virtual void removeItem(int id) const = 0;
	virtual std::vector<Item> getItems(int pageId = -1) const = 0;
	virtual void removeAllItems(int pageId = -1) const = 0;
};
