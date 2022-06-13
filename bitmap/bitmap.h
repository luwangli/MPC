#ifndef BITMAP_H
#define BITMAP_H

class BitMap{
private:
    int n;
    uint32_t zero_num = 0;
    const int mask;
    const int shift;
    const unsigned int pos;
    vector<unsigned int> a;

public:
    BitMap(int num):n(num),mask(31),shift(5),pos(1<<mask),a(1+n/32,0){}

    void set(int i){
        a[i>>shift] |= (pos>>(i&mask));
    }
    int get(int i){
        return (a[i>>shift] & (pos>>(i&mask)));
//        return (a[i>>shift] & (pos>>(i & mask))) > 0 ? 1 : 0;
    }
    void clr(int i){
        a[i>>shift] &= ~(pos>>(i&mask));
    }

    int countzerobits(int from, int to){
        int z = 0;
        for(int i =from;i<to;i++){
            if(!get(i))
                z++;
        }
        return z;
    }

    int countConsecOne(int size){
        int re = 0;
        for(int i=1;i<size;i++){
        //    cout<<"i:? "<<get(i)<<endl;
            if(!!get(i)){
                re +=1;
            }else{
                break;
            }
        }
        return re;
    }



};

#endif
