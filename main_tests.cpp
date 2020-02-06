#include <cassert>
#include <iostream>
#include <iomanip>
#include <math.h>

#include "Register.h"
#include "Specials.h"

using namespace Accenture::POS;

inline bool CompareNear(double a, double b) {
	return abs(a - b) < 0.0001;
}

void AddItemsTest() {
	Register r;
	r.AddItem("pears", ItemType::QTY);
	assert(r.ItemCount() == 1);
	r.AddItem("apples", ItemType::QTY);
	r.AddItem("peaches", ItemType::WT);
	assert(r.ItemCount() == 3);
	r.AddItem("apples", ItemType::WT);
	assert(r.ItemCount() == 3);
}

void SetPriceTestForQty() {
	Register r;
	r.AddItem("pears", ItemType::QTY);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 3);
	assert(CompareNear(r.CurrentTotal(), 4.50));
	r.SetItemPrice("pears", 2.50);
	assert(CompareNear(r.CurrentTotal(), 7.50));
}

void SetMarkdownTestForQty() {
	Register r;
	r.AddItem("pears", ItemType::QTY);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 3);
	assert(CompareNear(r.CurrentTotal(), 4.50));
	r.SetItemMarkdown("pears", .50);
	assert(CompareNear(r.CurrentTotal(), 3.00));
}

void SetPriceTestForWt() {
	Register r;
	r.AddItem("pears", ItemType::WT);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 3.5);
	assert(CompareNear(r.CurrentTotal(), 5.25));
	r.SetItemPrice("pears", 2.50);
	assert(CompareNear(r.CurrentTotal(), 8.75));
}

void SetMarkdownTestForWt() {
	Register r;
	r.AddItem("pears", ItemType::WT);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 3.5);
	assert(CompareNear(r.CurrentTotal(), 5.25));
	r.SetItemMarkdown("pears", .50);
	assert(CompareNear(r.CurrentTotal(), 3.50));
}

void RemoveTestForQty() {
	Register r;
	r.AddItem("pears", ItemType::WT);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 3);
	assert(CompareNear(r.CurrentTotal(), 4.50));
	r.AddItem("apples", ItemType::WT);
	r.SetItemPrice("apples", 2.00);
	r.ScanItem("apples", 1);
	assert(CompareNear(r.CurrentTotal(), 6.50));
	r.RemoveItem("pears");
	assert(CompareNear(r.CurrentTotal(), 2.00));
}

void RemoveTestForWt() {
	Register r;
	r.AddItem("pears", ItemType::QTY);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 3);
	assert(r.CurrentTotal() == 4.50);
	r.AddItem("apples", ItemType::QTY);
	r.SetItemPrice("apples", 2.00);
	r.ScanItem("apples", 1);
	assert(CompareNear(r.CurrentTotal(), 6.50));
	r.RemoveItem("pears");
	assert(CompareNear(r.CurrentTotal(), 2.00));
}

void WtSpecialTest1() {
	WtSpecial ws(1, .5, 100);
	auto p = ws.DiscountedPrice(4, 5.00);
	assert(CompareNear(p, 15.00));
}

void WtSpecialTest2() {
	WtSpecial ws(2, 1, 50);
	auto p = ws.DiscountedPrice(3, 4.00);
	assert(CompareNear(p, 10.00));
}

void WtSpecialTest3() {
	WtSpecial ws(2, 1, 25);
	auto p = ws.DiscountedPrice(11, 1.00);
	assert(CompareNear(p, 10.25));
}

void XForYSpecialTest1() {
	QtyXForYSpecial xy(3, 10.00);
	auto p = xy.DiscountedPrice(11, 7.50);
	assert(CompareNear(p, 45.00));
}

void XForYSpecialTest2() {
	QtyXForYSpecial xy(3, 10.00);
	auto p = xy.DiscountedPrice(2, 7.50);
	assert(CompareNear(p, 15.00));
}

void XForYSpecialTest3() {
	QtyXForYSpecial xy(3, 10.00);
	auto p = xy.DiscountedPrice(3, 7.50);
	assert(CompareNear(p, 10.00));
}

void BSGSSpecialLimitTest1() {
	QtyBSGSSpecial bsgs(2, 1, 6, 100);
	auto p = bsgs.DiscountedPrice(6, 2.00);
	assert(CompareNear(p, 8.00));
}

void BSGSSpecialLimitTest2() {
	QtyBSGSSpecial bsgs(2, 1, 6, 100);
	auto p = bsgs.DiscountedPrice(10, 2.00);
	assert(CompareNear(p, 16.00));
}

void BSGSSpecialLimitTest3() {
	QtyBSGSSpecial bsgs(2, 1, 6, 100);
	auto p = bsgs.DiscountedPrice(4, 2.00);
	assert(CompareNear(p, 6.00));
}

void BSGSSpecialNoLimitTest1() {
	QtyBSGSSpecial bsgs(2, 1, INT_MAX, 100);
	auto p = bsgs.DiscountedPrice(6, 2.00);
	assert(CompareNear(p, 8.00));
}

void BSGSSpecialNoLimitTest2() {
	QtyBSGSSpecial bsgs(2, 1, INT_MAX, 100);
	auto p = bsgs.DiscountedPrice(10, 2.00);
	assert(CompareNear(p, 14.00));
}

void BSGSSpecialNoLimitTest3() {
	QtyBSGSSpecial bsgs(2, 1, INT_MAX, 100);
	auto p = bsgs.DiscountedPrice(4, 2.00);
	assert(CompareNear(p, 6.00));
}

void AttachWtSpecial1() {
	Register r;
	r.AddItem("pears", ItemType::WT);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 5.5);
	assert(CompareNear(r.CurrentTotal(), 8.25));
	r.AttachWtSpecial("pears", 3, 1.5, 50);
	assert(CompareNear(r.CurrentTotal(), 7.125));
}

void AttachWtSpecial2() {
	Register r;
	r.AddItem("pears", ItemType::WT);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 10);
	assert(CompareNear(r.CurrentTotal(), 15.00));
	r.AttachWtSpecial("pears", 10, 1, 50);
	assert(CompareNear(r.CurrentTotal(), 15.00));
}

void AttachWtSpecial3() {
	Register r;
	r.AddItem("pears", ItemType::WT);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 10);
	assert(CompareNear(r.CurrentTotal(), 15.00));
	r.AttachWtSpecial("pears", 20, 10, 50);
	assert(CompareNear(r.CurrentTotal(), 15.00));
}

void AttachMultipleWtSpecial() {
	Register r;
	r.AddItem("pears", ItemType::WT);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 10);
	assert(CompareNear(r.CurrentTotal(), 15.00));
	r.AttachWtSpecial("pears", 20, 10, 50);
	r.AttachWtSpecial("pears", 10, 1, 50);
	r.AttachWtSpecial("pears", 3, 1, 50);
	assert(CompareNear(r.CurrentTotal(), 13.50));
}

void AttachQtyXForYSpecial1() {
	Register r;
	r.AddItem("pears", ItemType::QTY);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 5);
	assert(CompareNear(r.CurrentTotal(), 7.50));
	r.AttachQtyXForYSpecial("pears", 2, 2.50);
	assert(CompareNear(r.CurrentTotal(), 6.50));
}

void AttachQtyXForYSpecial2() {
	Register r;
	r.AddItem("pears", ItemType::QTY);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 5);
	assert(CompareNear(r.CurrentTotal(), 7.50));
	r.AttachQtyXForYSpecial("pears", 5, 2.50);
	assert(CompareNear(r.CurrentTotal(), 2.50));
}

void AttachQtyXForYSpecial3() {
	Register r;
	r.AddItem("pears", ItemType::QTY);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 5);
	assert(CompareNear(r.CurrentTotal(), 7.50));
	r.AttachQtyXForYSpecial("pears", 7, 2.50);
	assert(CompareNear(r.CurrentTotal(), 7.50));
}

void AttachMultipleXForY() {
	Register r;
	r.AddItem("pears", ItemType::QTY);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 5);
	assert(CompareNear(r.CurrentTotal(), 7.50));
	r.AttachQtyXForYSpecial("pears", 5, 2.50);
	assert(CompareNear(r.CurrentTotal(), 2.50));
	r.AttachQtyXForYSpecial("pears", 7, 2.50);
	assert(CompareNear(r.CurrentTotal(), 2.50));
	r.AttachQtyXForYSpecial("pears", 2, 2.50);
	assert(CompareNear(r.CurrentTotal(), 2.50));
}

void AttachQtyBSGSWithLimit1() {
	Register r;
	r.AddItem("pears", ItemType::QTY);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 8);
	assert(CompareNear(r.CurrentTotal(), 12.00));
	r.AttachQtyBSGSSpecialWithLimit("pears", 2, 1, 10, 100);
	assert(CompareNear(r.CurrentTotal(), 9.00));
}

void AttachQtyBSGSWithLimit2() {
	Register r;
	r.AddItem("pears", ItemType::QTY);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 8);
	assert(CompareNear(r.CurrentTotal(), 12.00));
	r.AttachQtyBSGSSpecialWithLimit("pears", 2, 1, 3, 20);
	assert(CompareNear(r.CurrentTotal(), 11.70));
}

void AttachQtyBSGSWithLimit3() {
	Register r;
	r.AddItem("pears", ItemType::QTY);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 8);
	assert(CompareNear(r.CurrentTotal(), 12.00));
	r.AttachQtyBSGSSpecialWithLimit("pears", 10, 2, 24, 20);
	assert(CompareNear(r.CurrentTotal(), 12.00));
}

void AttachMultipleBSGSWithLimit() {
	Register r;
	r.AddItem("pears", ItemType::QTY);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 8);
	assert(CompareNear(r.CurrentTotal(), 12.00));
	r.AttachQtyBSGSSpecialWithLimit("pears", 10, 2, 24, 20);
	r.AttachQtyBSGSSpecialWithLimit("pears", 2, 1, 10, 100);
	r.AttachQtyBSGSSpecialWithLimit("pears", 2, 1, 3, 20);
	assert(CompareNear(r.CurrentTotal(), 9.00));
}

void AttachQtyBSGSWithoutLimit1() {
	Register r;
	r.AddItem("pears", ItemType::QTY);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 8);
	assert(CompareNear(r.CurrentTotal(), 12.00));
	r.AttachQtyBSGSSpecialWithoutLimit("pears", 2, 2, 10);
	assert(CompareNear(r.CurrentTotal(), 11.40));
}

void AttachQtyBSGSWithoutLimit2() {
	Register r;
	r.AddItem("pears", ItemType::QTY);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 8);
	assert(CompareNear(r.CurrentTotal(), 12.00));
	r.AttachQtyBSGSSpecialWithoutLimit("pears", 10, 2, 20);
	assert(CompareNear(r.CurrentTotal(), 12.00));
}

void AttachQtyBSGSWithoutLimit3() {
	Register r;
	r.AddItem("pears", ItemType::QTY);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 8);
	assert(CompareNear(r.CurrentTotal(), 12.00));
	r.AttachQtyBSGSSpecialWithoutLimit("pears", 1, 1, 80);
	assert(CompareNear(r.CurrentTotal(), 7.20));
}

void AttachMultipleBSGS() {
	Register r;
	r.AddItem("pears", ItemType::QTY);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 8);
	assert(CompareNear(r.CurrentTotal(), 12.00));
	r.AttachQtyBSGSSpecialWithoutLimit("pears", 1, 1, 80);
	r.AttachQtyBSGSSpecialWithoutLimit("pears", 10, 2, 20);
	r.AttachQtyBSGSSpecialWithoutLimit("pears", 2, 2, 10);
	assert(CompareNear(r.CurrentTotal(), 7.20));
}

void MarkdownsAndSpecials() {
	Register r;
	r.AddItem("pears", ItemType::QTY);
	r.SetItemPrice("pears", 1.50);
	r.ScanItem("pears", 8);
	assert(CompareNear(r.CurrentTotal(), 12.00));
	r.AttachQtyBSGSSpecialWithLimit("pears", 2, 1, 3, 20);
	assert(CompareNear(r.CurrentTotal(), 11.70));
	r.SetItemMarkdown("pears", 0.25);
	assert(CompareNear(r.CurrentTotal(), 9.75));
}

int main() {
	std::cout << "Begin Tests\n";
	AddItemsTest();
	SetPriceTestForWt();
	SetPriceTestForQty();
	SetMarkdownTestForQty();
	SetMarkdownTestForWt();
	RemoveTestForWt();
	RemoveTestForQty();
	WtSpecialTest1();
	WtSpecialTest2();
	WtSpecialTest3();
	XForYSpecialTest1();
	XForYSpecialTest2();
	XForYSpecialTest3();
	BSGSSpecialLimitTest1();
	BSGSSpecialLimitTest2();
	BSGSSpecialLimitTest3();
	BSGSSpecialNoLimitTest1();
	BSGSSpecialNoLimitTest2();
	BSGSSpecialNoLimitTest3();
	AttachWtSpecial1();
	AttachWtSpecial2();
	AttachWtSpecial3();
	AttachMultipleWtSpecial();
	AttachQtyXForYSpecial1();
	AttachQtyXForYSpecial2();
	AttachQtyXForYSpecial3();
	AttachMultipleXForY();
	AttachQtyBSGSWithLimit1();
	AttachQtyBSGSWithLimit2();
	AttachQtyBSGSWithLimit3();
	AttachMultipleBSGSWithLimit();
	AttachQtyBSGSWithoutLimit1();
	AttachQtyBSGSWithoutLimit2();
	AttachQtyBSGSWithoutLimit3();
	AttachMultipleBSGS();
	MarkdownsAndSpecials();
	std::cout << "Tests Successful\n";
}