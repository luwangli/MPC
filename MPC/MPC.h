#ifndef MPC_H
#define MPC_H

typedef struct{
    int HLLSize;
    int HLLNum;
    char *HLL;
    float Car = 0;
    float alpha;
}MPCEst;

MPCEst *newMPCEst(int m,int size){
    if(size<4||30<size){
        throw std::invalid_argument("bit shoud be in [5,30]");
    }
    MPCEst *mh = (MPCEst*)malloc(sizeof(MPCEst));
    mh->HLLSize = size;
    mh->HLLNum = m;
    mh->alpha = 0.7213/(1+1.079/m);
   // cout<<"alpha: "<<mh->alpha<<endl;
    mh->HLL = (char*)malloc(sizeof(char)* mh->HLLNum);
    for(int i=0;i<mh->HLLNum;i++){
        mh->HLL[i] = 0;
    }
    return mh;
}
float AddMH(MPCEst *mh){
    float prob = 0;
    for(int i=0;i<mh->HLLNum;i++){
        prob += std::pow(2,-(int)mh->HLL[i]);
    }
 //   cout<<"prob :"<<prob<<endl;
    prob = prob / mh->HLLNum;
    float value = pow(prob,-1);
    return value;

}

void MPCEstInsert(MPCEst *mh, uint32_t elemID){
    srand((unsigned)time(NULL));
    uint32_t bucketID = murmurhash((const char*)&elemID,4,rand()) % mh->HLLNum;
    uint32_t hashValue = murmurhash((const char*)&elemID,4,rand());
    int fstOnePos = NumberOfLeadingZeros(hashValue) + 1;
    if((int)mh->HLL[bucketID] < fstOnePos){
        mh->Car += AddMH(mh);
        mh->HLL[bucketID] = (char)(fstOnePos&0x000000ff);
    }
}

float MPCEstGetCar(MPCEst *mh){
    return mh->Car;
}

#endif
