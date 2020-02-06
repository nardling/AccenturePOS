#pragma once
#include <climits>

namespace Accenture::POS {
	class QtySpecial {
	public:
		QtySpecial(uint64_t qq) : _qualifyingQty(qq) {}
		virtual double DiscountedPrice(uint64_t qty, double basePrice) = 0;
	protected:
		uint64_t _qualifyingQty;
	};

	class QtyXForYSpecial : public QtySpecial {
		public:
			QtyXForYSpecial(uint64_t qq, double sp) : QtySpecial(qq), _specialPrice(sp) {}

			double _specialPrice;

			double DiscountedPrice(uint64_t qty, double basePrice) override {
				double discountPrice = 0;

				while (qty >= _qualifyingQty) {
					discountPrice += _specialPrice;
					qty -= _qualifyingQty;
				}

				discountPrice += qty * basePrice;
				return discountPrice;
			}
	};

	class QtyBSGSSpecial : public QtySpecial {
		public:
			QtyBSGSSpecial(uint64_t qq, uint64_t dq, uint64_t lq, uint8_t dp) : QtySpecial(qq), _discountQty(dq),
				_combinedQty(qq + dq), _discountPct(1 - dp/100.0), _limitQty(lq) { }

			uint64_t _discountQty;
			uint64_t _combinedQty;
			uint64_t _limitQty;
			double _discountPct;

			double DiscountedPrice(uint64_t qty, double basePrice) override {
				double discountPrice = 0;

				if (qty > _limitQty) {
					discountPrice = (qty - _limitQty) * basePrice;
					qty = _limitQty;
				}

				while (qty >= _combinedQty) {
					discountPrice += (_qualifyingQty * basePrice + _discountQty * basePrice * _discountPct);
					qty -= _combinedQty;
				}

				discountPrice += qty * basePrice;
				return discountPrice;
			}
	};

	class WtSpecial {
		public:
			WtSpecial(double qq, double dq, uint8_t dp) : _qualifyingQty(qq), _discountQty(dq),
				_combinedQty(qq + dq), _discountPct(1 - dp/100.0) { }

			double _qualifyingQty;
			double _discountQty;
			double _combinedQty;
			double _discountPct;

			double DiscountedPrice(double qty, double basePrice) {
				double discountPrice = 0;

				while (qty >= _combinedQty) {
					discountPrice += (_qualifyingQty * basePrice + _discountQty * basePrice * _discountPct);
					qty -= _combinedQty;
				}
				discountPrice += qty * basePrice;
				return discountPrice;
			}
	};
}