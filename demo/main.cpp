#include "../common/param.h"
#include "../common/Read.h"
//#include "../bitmap/bitmap.h"
#include "../HLL/HLLEstBitV.h"
#include "../LL/LLEstBitV.h"
#include "../MPC/MPC.h"

#include <unistd.h>
using namespace std;

///#define UINT32_MAX (0xFFFFFFFF)

int main(){
    vector<pair<uint32_t, uint32_t>> Stream;
    vector<pair<uint32_t, uint32_t>> Bench;

    int pkt_num;
    int flow_num;

    int length = 1000;
    char filename[100] = "../Data/1.txt";

    pkt_num = Read(filename, Stream, flow_num);
    cout<<endl;
    timespec start_time, end_time;
  //  clock_t start_clock, end_clock;
    double insert_throughput=0, query_throughput=0;
    long long timediff=0;
    int query_num = 1000;
   // double ab_error = 0;
    int est_car;
    ofstream outFile;
    int test_time = 10;


    /************************HLL*****************/
    int hll_size = 5;


 for(int t=0;t<test_time;t++)
{
    cout<<endl<<"*********************test time: "<<t<<endl;
    cout<<"\nHLL:"<<endl;
    auto hll = newHLLEst(length/hll_size,hll_size);

    clock_gettime(CLOCK_MONOTONIC,&start_time);
    for(int i=0;i<pkt_num;i++){
        HLLEstInsert(hll,Stream[i].second);
    }
    clock_gettime(CLOCK_MONOTONIC,&end_time);

  //  est_car = HLLEstGetCar(hll);

    timediff = (long long)(end_time.tv_sec - start_time.tv_sec) * 1000000000LL + (end_time.tv_nsec - start_time.tv_nsec);
    insert_throughput = (double)1000.0*pkt_num / timediff;


    clock_gettime(CLOCK_MONOTONIC,&start_time);
    for(int i=0;i<query_num;i++){
        est_car = HLLEstGetCar(hll);
    }
    clock_gettime(CLOCK_MONOTONIC,&end_time);
    timediff = (long long)(end_time.tv_sec - start_time.tv_sec) * 1000000000LL + (end_time.tv_nsec - start_time.tv_nsec);
//    query_throughput = (double)1000.0*query_num /timediff;
    query_throughput =timediff / 1000;


    cout<<"estimate value: "<< est_car<<"\t error: "<<abs(est_car-flow_num)
            <<"\t insert: "<<insert_throughput
            <<"\t query: "<<query_throughput<<endl;

    outFile.open("result.csv",ios::app);
    outFile<<filename<<","<<length<<",HLL,"<<est_car<<","<<est_car*1.0/pkt_num
            <<","<<insert_throughput<<","<<query_throughput<<endl;
    outFile.close();

    //*****************MPC**********************/
    cout<<endl<<"\nMPC:"<<endl;
    auto MPC = newMPCEst(length/hll_size,hll_size);
    clock_gettime(CLOCK_MONOTONIC,&start_time);
    for(int i=0;i<pkt_num;i++){
        MPCEstInsert(MPC,Stream[i].second);
    }
    clock_gettime(CLOCK_MONOTONIC,&end_time);
    timediff = (long long)(end_time.tv_sec - start_time.tv_sec) * 1000000000LL + (end_time.tv_nsec - start_time.tv_nsec);
    insert_throughput = (double)1000.0*pkt_num / timediff;

    est_car = MPCEstGetCar(MPC);

    clock_gettime(CLOCK_MONOTONIC,&start_time);
    for(int i=0;i<query_num;i++){
        est_car = MPCEstGetCar(MPC);
    }
    clock_gettime(CLOCK_MONOTONIC,&end_time);
    timediff = (long long)(end_time.tv_sec - start_time.tv_sec) * 1000000000LL + (end_time.tv_nsec - start_time.tv_nsec);
    //query_throughput = (double)1000.0*query_num /timediff;
    query_throughput =timediff / 1000;

    cout<<"estimate value: "<< est_car<<"\t error: "<<abs(est_car-flow_num)
            <<"\t insert: "<<insert_throughput<<"\t query: "<<query_throughput<<endl;
    outFile.open("result.csv",ios::app);
    outFile<<filename<<","<<length<<",MPC,"<<est_car<<","<<est_car*1.0/pkt_num
        <<","<<insert_throughput<<","<<query_throughput<<endl;
    outFile.close();
    //*******************LogLog**********/
    cout<<endl<<"\nLL: "<<endl;
    auto ll = newLLEst(length/hll_size,hll_size);
    clock_gettime(CLOCK_MONOTONIC,&start_time);
    for(int i=0;i<pkt_num;i++){
        LLEstInsert(ll,Stream[i].second);
    }
    clock_gettime(CLOCK_MONOTONIC,&end_time);
    timediff = (long long)(end_time.tv_sec - start_time.tv_sec) * 1000000000LL + (end_time.tv_nsec - start_time.tv_nsec);
    insert_throughput = (double)1000.0*pkt_num / timediff;


    clock_gettime(CLOCK_MONOTONIC,&start_time);
    for(int i=0;i<query_num;i++){
        est_car = LLEstGetCar(ll);
    }
    clock_gettime(CLOCK_MONOTONIC,&end_time);
    timediff = (long long)(end_time.tv_sec - start_time.tv_sec) * 1000000000LL + (end_time.tv_nsec - start_time.tv_nsec);
    query_throughput =timediff / 1000;
//    query_throughput = (double)1000.0*query_num /timediff;

    cout<<"estimate value: "<< est_car<<"\t error: "<<abs(est_car-flow_num)
       <<"\t insert: "<<insert_throughput
        <<"\t query: "<<query_throughput<<endl;
    outFile.open("result.csv",ios::app);
    outFile<<filename<<","<<length<<",LL,"<<est_car<<","<<est_car*1.0/pkt_num<<endl;
    outFile.close();
    usleep(1000000);

    delete[] hll;
    delete[] MPC;
    delete[] ll;


  }

    /***********************PCSA****************/
    /*
    cout<<endl<<"\nPCSA"<<endl;
    int pcsa_size = 18;
    auto pcsa = newPCSAEst(length/pcsa_size,pcsa_size);
    for(int i=0;i<pkt_num;i++){
        PCSAEstInsert(pcsa, Stream[i].second);
    }

    est_car = PCSAEstGetCar(pcsa);
    cout<<"estimate value: "<< est_car<<"\t error: "<<abs(est_car-flow_num)
    <<"\t insert: "<<insert_throughput
    <<"\t query: "<<query_throughput<<endl;
*/

        /***********************MPCSA****************/
    /*
    cout<<endl<<"\nMPCSA"<<endl;
    int mpcsa_size = 18;
    auto mpcsa = newMPCSAEst(length/mpcsa_size,mpcsa_size);
    for(int i=0;i<pkt_num;i++){
        MPCSAEstInsert(mpcsa, Stream[i].second);
    }

    est_car = MPCSAEstGetCar(mpcsa);
    cout<<"estimate value: "<< est_car<<"\t error: "<<abs(est_car-flow_num)
    <<"\t insert: "<<insert_throughput
    <<"\t query: "<<query_throughput<<endl;
*/
       /***********************MPCSA****************/
       /*
    cout<<endl<<"\nACUT"<<endl;
    int acut_size = 18;
    auto acut = newACUTEst(length/acut_size,acut_size);
    for(int i=0;i<100;i++){
        ACUTEstInsert(acut, Stream[i].second);
    }

    est_car = ACUTEstGetCar(acut);
    cout<<"estimate value: "<< est_car<<"\t error: "<<abs(est_car-flow_num)
    <<"\t insert: "<<insert_throughput
    <<"\t query: "<<query_throughput<<endl;*/
}
