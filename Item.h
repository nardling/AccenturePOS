#pragma once

#include <string>
#include <vector>

#include "Specials.h"

namespace Accenture::POS {
	enum class ItemType : uint8_t {
		WT,
		QTY,
		UNK
	};

	class ItemBase {
		protected:
			std::string _name;
			double _subtotal;
			int32_t _qty;
			double _wt;
			double _unitPrice;
			double _markdown;

		public:
			ItemBase(const std::string& itemName) : _name(itemName), _subtotal(0), _qty(0), _wt(0), _unitPrice(0), _markdown(0) { }

			double Subtotal() { return _subtotal; }

			virtual double ReCalculate() = 0;
			virtual double AddQty(int32_t qty) = 0;
			virtual double AddWt(double wt) = 0;
			virtual double AttachWtSpecial(double qualifyingQuantity, double discountQuantity, double discountPct) = 0;
			virtual double AttachQtyXforYSpecial(int32_t qq, double sp) = 0;
			virtual double AttachBSGSSpecialWithNoLimit(int32_t qualifyingQuantity, int32_t discountQuantity, int8_t discountPct) = 0;
			virtual double AttachBSGSSpecialWithLimit(int32_t qualifyingQuantity, int32_t discountQuantity, int32_t limitQuantity,
				int8_t discountPct) = 0;

			virtual ItemType Type() const  = 0;

			double Remove() { return _subtotal; }

			double SetPrice(double pr) { 
				_unitPrice = pr;
				return ReCalculate();
			}
			double SetMarkdown(double m) { 
				_markdown = m; 
				return ReCalculate();
			}

	};

	class ItemByWt : public ItemBase {
		public:
			ItemByWt(const std::string& itemName) : ItemBase(itemName) {}
			double ReCalculate() override {
				double newSt = _wt * (_unitPrice - _markdown);
				for (auto& s : _specials) {
					// Making the assumption that markdowns and specials interact
					auto t = s.DiscountedPrice(_wt, (_unitPrice - _markdown));
					if (t < newSt)
						newSt = t;
				}
				double delta = newSt - _subtotal;
				_subtotal = newSt;
				return delta;
			}

			double AddQty(int32_t qty) override {
				return 0;
			}

			double AddWt(double wt) override {
				_wt += wt;
				return ReCalculate();
			}

			double AttachWtSpecial(double qualifyingQuantity, double discountQuantity, double discountPct) override {
				_specials.emplace_back(qualifyingQuantity, discountQuantity, discountPct);
				return ReCalculate();
			}

			double AttachQtyXforYSpecial(int32_t qq, double sp) override { return 0; }

			double AttachBSGSSpecialWithNoLimit(int32_t qualifyingQuantity, int32_t discountQuantity, int8_t discountPct) override { return 0; }

			double AttachBSGSSpecialWithLimit(int32_t qualifyingQuantity, int32_t discountQuantity, int32_t limitQuantity,
				int8_t discountPct) override { return 0; }

			ItemType Type() const override { return ItemType::WT; }

		private:
			std::vector<WtSpecial> _specials;
	};

	class ItemByQty : public ItemBase {
		public:
			ItemByQty(const std::string& itemName) : ItemBase(itemName) {}

			double ReCalculate() override {
				double newSt = _qty * (_unitPrice - _markdown);
				for (auto& s : _specials) {
					// Making the assumption that markdowns and specials interact
					auto t = s->DiscountedPrice(_qty, (_unitPrice - _markdown));
					if (t < newSt)
						newSt = t;
				}
				double delta = newSt - _subtotal;
				_subtotal = newSt;
				return delta;
			}

			double AddQty(int32_t qty) override {
				_qty += qty;
				return ReCalculate();
			}

			double AddWt(double wt) override {
				return 0;
			}

			double AttachWtSpecial (double qualifyingQuantity, double discountQuantity, double discountPct) override { return 0; }

			double AttachQtyXforYSpecial(int32_t qq, double sp) override {
				_specials.push_back(std::make_shared<QtyXForYSpecial>(qq, sp));
				return ReCalculate();
			}

			double AttachBSGSSpecialWithLimit(int32_t qualifyingQuantity, int32_t discountQuantity, int32_t limitQuantity,
				int8_t discountPct) override {
				_specials.push_back(std::make_shared<QtyBSGSSpecial>(qualifyingQuantity, discountQuantity, limitQuantity, discountPct));
				return ReCalculate();
			}

			double AttachBSGSSpecialWithNoLimit(int32_t qualifyingQuantity, int32_t discountQuantity, int8_t discountPct) override {
				_specials.push_back(std::make_shared<QtyBSGSSpecial>(qualifyingQuantity, discountQuantity, INT_MAX, discountPct));
				return ReCalculate();
			}

			ItemType Type() const override { return ItemType::QTY; }

		private:
			std::vector<std::shared_ptr<QtySpecial>> _specials;
	};

}