#pragma once

#include <unordered_map>
#include <iostream>
#include <memory>

#include "Item.h"

namespace Accenture::POS {
	class Register {
		private:
			std::unordered_map<std::string, std::unique_ptr<ItemBase>> _items;
			double _runningTotal;

		public:
			Register() : _runningTotal(0) { }

			size_t ItemCount() const { return _items.size(); }

			double CurrentTotal() const { return _runningTotal; }

			void AddItem(const std::string& itemName, ItemType type) {
				auto it = _items.find(itemName);
				if (it == _items.end()) {
					if (type == ItemType::WT) {
						std::unique_ptr<ItemByWt> i = std::make_unique<ItemByWt>(itemName);
						_items.insert(std::pair<std::string, std::unique_ptr<ItemByWt>>(itemName, std::move(i)));
					} else if (type == ItemType::QTY) {
						std::unique_ptr<ItemByQty> i = std::make_unique<ItemByQty>(itemName);
						_items.insert(std::pair<std::string, std::unique_ptr<ItemByQty>>(itemName, std::move(i)));
					}
				}
			}

			void RemoveItem(const std::string& itemName) {
				auto it = _items.find(itemName);
				if (it != _items.end()) {
					_runningTotal -= it->second->Remove();
					_items.erase(it);
				}
			}

			void SetItemPrice(const std::string& itemName, double pr) {
				if (pr <= 0) return;
				auto it = _items.find(itemName);
				if (it != _items.end()) {
					_runningTotal += it->second->SetPrice(pr);
				}
			}

			void SetItemMarkdown(const std::string& itemName, double markdown) {
				if (markdown < 0) return;
				auto it = _items.find(itemName);
				if (it != _items.end()) {
					_runningTotal += it->second->SetMarkdown(markdown);
				}
			}

			void ScanItem(const std::string& itemName, double qty) {
				if (qty <= 0) return;
				auto it = _items.find(itemName);
				if (it != _items.end()) {
					if (it->second->Type() == ItemType::WT)
						_runningTotal += it->second->AddWt(qty);
					else if (it->second->Type() == ItemType::QTY)
						_runningTotal += it->second->AddQty(qty);
				}
			}

			void AttachWtSpecial(const std::string& itemName, double qualifyingQuantity, double discountQuantity, int8_t discountPct) {
				if (discountPct > 100 || discountPct < 0 || qualifyingQuantity < 0 || discountQuantity < 0) return;
				auto it = _items.find(itemName);
				if (it != _items.end()) {
					if (it->second->Type() == ItemType::WT) {
						_runningTotal += it->second->AttachWtSpecial(qualifyingQuantity, discountQuantity, discountPct);
					}
				}
			}

			void AttachQtyXForYSpecial(const std::string& itemName, int32_t qualifyingQuantity, double discountPrice) {
				if (qualifyingQuantity < 0 || discountPrice < 0) return;
				auto it = _items.find(itemName);
				if (it != _items.end()) {
					if (it->second->Type() == ItemType::QTY) {
						_runningTotal += it->second->AttachQtyXforYSpecial(qualifyingQuantity, discountPrice);
					}
				}
			}

			void AttachQtyBSGSSpecialWithLimit(const std::string& itemName, int32_t qualifyingQuantity, int32_t discountQuantity, int32_t limitQty,
				int8_t discountPct) {
				if (qualifyingQuantity < 0 || discountQuantity < 0 || limitQty < 0 || discountPct < 0 || discountPct > 100) return;
				auto it = _items.find(itemName);
				if (it != _items.end()) {
					if (it->second->Type() == ItemType::QTY) {
						_runningTotal += it->second->AttachBSGSSpecialWithLimit(qualifyingQuantity, discountQuantity, limitQty, discountPct);
					}
				}
			}

			void AttachQtyBSGSSpecialWithoutLimit(const std::string& itemName, int32_t qualifyingQuantity, int32_t discountQuantity, int8_t discountPct) {
				if (qualifyingQuantity < 0 || discountQuantity < 0 || discountPct < 0 || discountPct > 100) return;
				auto it = _items.find(itemName);
				if (it != _items.end()) {
					if (it->second->Type() == ItemType::QTY) {
						_runningTotal += it->second->AttachBSGSSpecialWithNoLimit(qualifyingQuantity, discountQuantity, discountPct);
					}
				}
			}
	};
}