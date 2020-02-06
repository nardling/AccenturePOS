#pragma once
#include <climits>

namespace Accenture::POS {
	class QtySpecial {
		public:
			QtySpecial(int32_t  qq) : _qualifyingQty(qq) {}
			virtual double DiscountedPrice(int32_t  qty, double basePrice) = 0;
			
		protected:
			int32_t  _qualifyingQty;
	};

	class QtyXForYSpecial : public QtySpecial {
		public:
			QtyXForYSpecial(int32_t  qq, double sp) : QtySpecial(qq), _specialPrice(sp) {}

			double _specialPrice;

			double DiscountedPrice(int32_t  qty, double basePrice) override {
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
			QtyBSGSSpecial(int32_t  qq, int32_t  dq, int32_t  lq, uint8_t dp) : QtySpecial(qq), _discountQty(dq),
				_combinedQty(qq + dq), _discountPct(1 - dp/100.0), _limitQty(lq) { }

			int32_t  _discountQty;
			int32_t  _combinedQty;
			int32_t  _limitQty;
			double _discountPct;

			double DiscountedPrice(int32_t  qty, double basePrice) override {
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