#ifndef ITEM_H
#define ITEM_H

class Item {
    public:
        double x,y;
        int itemNo;
        Item(double x, double y, int itemNo): x(x), y(y), itemNo(itemNo) {}
        Item() {}
};

#endif
