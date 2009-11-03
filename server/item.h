#ifndef ITEM_H
#define ITEM_H

class Item {
    public:
        double x,y;
        int itemNo;
		int id;
        Item(double x, double y, int itemNo, int id): x(x), y(y), itemNo(itemNo), id(id) {}
        Item() {}
};

#endif
