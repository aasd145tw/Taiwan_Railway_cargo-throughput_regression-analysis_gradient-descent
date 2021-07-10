#include <unsupported/Eigen/MatrixFunctions>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <complex>
#include <cstdlib>
#include <cstdio>
#include<ctime>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include<time.h>


using namespace Eigen;
using namespace std;

int main(){

	ofstream oFile;

    oFile.open("市區道路交通事故延續時間預測.csv", ios::out | ios::trunc);


//--------------------------------參數項(X矩陣)----------------------------------------//

//-------------------------行為參數項種類; 列為不同筆參數-----------------------------//
 MatrixXd X(108, 3);
    X <<
    //平均每日噸數/噸延公里/平均每噸運距   
        28879, 71937134, 80,
        24802, 48105096, 63,
        32003, 70757053, 71,
        29987, 59381009, 64,
        34354, 65318679, 61,
        29014, 50856943, 57,
        20807, 42964809, 67,
        21787, 49413489, 73,
        23805, 51278140, 70,
        25914, 53036443, 73,
        28925, 57804112, 69,
        23305, 42189576, 65,
        24533, 41511599, 60,
        26054, 42690712, 59,
        19276, 28909193, 52,
        21194, 35353314, 60,
        15825, 31178780, 70,
        16328, 29678516, 65,
        30245, 77431373, 83,
        30980, 72229911, 75,
        30997, 69128913, 72,
        32510, 59468836, 59,
        33623, 63102952, 61,
        28529, 52376170, 59,
        24647, 46873910, 61,
        21497, 47487509, 71,
        22348, 47830622, 69,
        30669, 74509672, 81,
        31530, 74146914, 78,
        29087, 59394544, 68,
        31389, 57683740, 61,
        31165, 54395805, 58,
        25913, 49241530, 63,
        22993, 39299184, 57,
        20699, 45095540, 73,
        19057, 44375861, 78,
        28906, 72258390, 81,
        32669, 80784769, 80,
        27707, 55972379, 65,
        30611, 58513641, 62,
        32921, 63298056, 62,
        27153, 51961312, 62,
        24555, 46169444, 61,
        23583, 50871781, 70,
        21218, 45687123, 70,
        27886, 71754147, 86,
        27187, 67617916, 83,
        26882, 57456464, 71,
        30177, 55686280, 62,
        31660, 54856831, 58,
        27169, 50245980, 62,
        23009, 43223021, 63,
        23768, 48617461, 68,
        20775, 41927323, 67,
        28690, 72152824, 81,
        32683, 79951947, 79,
        29754, 63137802, 69,
        29330, 57004107, 63,
        29299, 51007838, 56,
        23223, 45322724, 63,
        21067, 41613503, 64,
        21459, 48368803, 73,
        20290, 44492007, 71,
        27992, 69725660, 80,
        24054, 56552734, 76,
        27348, 57306118, 68,
        31454, 59538041, 61,
        23005, 36356226, 51,
        26721, 50732259, 61,
        19935, 44139157, 71,
        21126, 43571158, 67,
        18573, 42911221, 75,
        30588, 72902097, 79,
        31704, 73029663, 77,
        28202, 56663959, 67,
        28928, 55174513, 64,
        26804, 43420633, 54,
        21501, 39988739, 62,
        18360, 40897050, 74,
        19772, 40139720, 68,
        18702, 40594338, 72,
        26368, 65671353, 80,
        32768, 78995498, 78,
        30101, 65152236, 70,
        32331, 65390638, 65,
        28912, 50861576, 57,
        24137, 45905226, 61,
        17191, 40609689, 76,
        21740, 47228587, 70,
        20273, 43847209, 70,
        31142, 70609012, 76,
        31966, 72889915, 76,
        32694, 65858673, 67,
        29283, 55038032, 63,
        30947, 54160047, 58,
        24875, 49223720, 66,
        19467, 42634003, 73,
        21922, 45995888, 70,
        19508, 40594516, 69,
        33011, 75718955, 74,
        27294, 66366084, 78,
        31783, 64261088, 65,
        30934, 57030220, 60,
        29599, 54750936, 60,
        24205, 47512630, 63,
        21968, 48595355, 71,
        20151, 44528089, 71,
        19136, 43360751, 73;


int mm=X.row(0).size();  //組間數量
int nn=X.col(0).size(); // 回歸數量
MatrixXd Theta_1(mm,1);             //解析回歸係數解
MatrixXd Theta_2 (mm,1);            //梯度下降回歸係數解

srand( time(NULL) );

double S1;
double S2;

double PROD_Multi;
double Cossim;

double R_square;
double SST;
double SSE;
double SSR;
double Y_Nor_avg;
//-------------------梯度下降迴歸係數初始化; 範圍設定[-1,1]---------------------------//

for(int j=0;j<Theta_2.row(0).size();++j){

for(int i=0;i<Theta_2.col(0).size();++i){

Theta_2 (i,j)=(double) rand() /(RAND_MAX+1 );
}

}

//------------------------------------------------------------------------------------//

double  f;
double  F;
//------------------------------------------------------------------------------------//

MatrixXd X_avg(1,mm);    // 每一項參數平均值
MatrixXd u(1,mm);        // 每一項參數最大值與最小值差
MatrixXd X_Nor(nn,mm);    // 輸入X矩陣正規化結果

//------------------------------自變數X矩陣正規化----------------------------------------//
for(int i=0;i<X.row(0).size();++i){
 X_avg(0,i)=(X.col(i)).sum()/X.col(0).size();
 u(0,i)=X.col(i).maxCoeff()-X.col(i).minCoeff();

}

for(int i=0;i<X.col(0).size();++i){
for(int j=0;j<X.row(0).size();++j){

X_Nor(i,j)=(X(i,j)-X_avg(0,j))/u(0,j);
X_Nor(i,0)=1;

}
}
//cout<<u<<endl;
//cout<<X_Nor<<endl;
//------------------------------------------------------------------------------------//
//----------------------------延遲時間正規化(單位:s)----------------------------------//
MatrixXd Y(nn, 1);
    Y <<
        83280558,
        56199488,
        82485436,
        70757222,
        77944783,
        60946733,
        50588161,
        57563425,
        62370445,
        62025563,
        68457531,
        49869590,
        49949816,
        51876563,
        34784385,
        42167262,
        36114739,
        35884022,
        90087942,
        84499078,
        80257728,
        71333249,
        76075166,
        62461663,
        55906988,
        55271503,
        58001367,
        85942331,
        88007911,
        70062230,
        67820438,
        65960469,
        58136530,
        48625801,
        53408971,
        54707235,
        83455590,
        94821496,
        65670341,
        68728821,
        75304631,
        61229580,
        57444943,
        60705235,
        57559958,
        83355499,
        79044471,
        66341523,
        66896138,
        65955558,
        58939553,
        52884261,
        57927953,
        52481746,
        83456865,
        93746651,
        73856513,
        66891959,
        61417737,
        52984844,
        50273740,
        57501244,
        55696574,
        81580147,
        65863880,
        67653504,
        70280096,
        44466747,
        60546056,
        53461249,
        51135895,
        52692516,
        85287258,
        84387049,
        66991872,
        64195736,
        53231210,
        47424471,
        48893845,
        47728717,
        50361840,
        76031632,
        91436121,
        76138816,
        76396327,
        60298727,
        55871269,
        49163846,
        57417189,
        53828625,
        82336457,
        85857840,
        78945279,
        64995799,
        63238163,
        58398895,
        51838808,
        56226640,
        50089897,
        88209710,
        76998079,
        76445723,
        67427556,
        63956703,
        56374881,
        63677785,
        54006102,
        54388041;



MatrixXd Y_avg(nn,1);
MatrixXd Y_Nor(nn,1);

for(int i=0;i<Y.col(0).size();++i){
for(int j=0;j<Y.row(0).size();++j){

Y_avg(i,j)=Y.sum()/Y.size();

Y_Nor(i,j)=(Y(i,j)-Y_avg(0,j))/(Y.maxCoeff()-Y.minCoeff());


}
}
//------------------------------------------------------------------------------------//
//----------------------------------回歸解析解----------------------------------------//

Theta_1=(X_Nor.transpose()*X_Nor).inverse()*X_Nor.transpose()*Y_Nor;


//----------------------------------梯度下降法計算-------------------------------------//
MatrixXd J_Th(1,1);
MatrixXd MSE(1,1);
 
//double s=pow(2*(X.row(0)).size(),-1);
MatrixXd Grad_J(nn,1);
double Lr=0.05;                 // 學習率
double ep=60000;               //疊代數
MatrixXd J_Th_Lambda(1,1);
double apha=0.1;
double J[60000];
MatrixXd PROD(1,1);           //
MatrixXd Y_R(nn,1);
MatrixXd Y_Inv_nor(nn,1);
MatrixXd  MAPE(nn,1);
MatrixXd  MAPE_ratio(nn,1);
double MAPE_sum;
//---------------------------------------"疊代開始"----------------------------------------------------------// 
  for(int k=0;k<ep;++k)

    {

//J_Th= pow(2*(X_Nor.col(0)).size(),-1)*(((X_Nor*Theta_2).array() )-(Y_Nor.array()) ).square();

J_Th= pow(2*(X_Nor.col(0)).size(),-1)*((X_Nor*Theta_2-Y_Nor ).transpose()) *(X_Nor*Theta_2-Y_Nor) + apha*pow(2*(X_Nor.col(0)).size(),-1)*Theta_2.transpose() * Theta_2 ;
//J_Th= pow(2*(X_Nor.col(0)).size(),-1)*((X_Nor*Theta_2-Y_Nor ).transpose()) *(X_Nor*Theta_2-Y_Nor) + apha*Theta_2.transpose() * Theta_2 ;
MSE= pow(2*(X_Nor.col(0)).size(),-1)*((X_Nor*Theta_2-Y_Nor ).transpose()) *(X_Nor*Theta_2-Y_Nor);


J[k]=J_Th.sum();

Grad_J= 2*pow(2*(X_Nor.col(0)).size(),-1)*(X_Nor*Theta_2-Y_Nor).transpose() * X_Nor;

//Grad_J= 2*pow(2*(X_Nor.col(0)).size(),-1)*(X_Nor*Theta_2-Y_Nor).transpose() * X_Nor;
Theta_2=(1-Lr*apha*pow((X_Nor.col(0)).size(),-1))*Theta_2-Lr*Grad_J.transpose();
//Theta_2=Theta_2-Lr*Grad_J.transpose();

//-------------------------收斂條件:當下疊帶值與前一次疊帶值差值 eps<10^-8---------------------------//
	if(abs(J[k] -J[k-1]) <pow(10,-8) )
		{
            break;

        }

//-------------殘差平方和SSE(實際值Vs預測值)---------------//
  SSE=2*(X_Nor.col(0)).size()* MSE(0,0);//Σ(yi-y_hat_i)^2


//-------------總平方和SST(實際值Vs平均值)---------------//

MatrixXd SS_T(nn,1);

Y_Nor_avg=Y_Nor.sum()/Y_Nor.size();

for(int i=0;i<Y.col(0).size();++i){
for(int j=0;j<Y.row(0).size();++j){

SS_T(i,j)=pow((Y_Nor(i,j)-Y_Nor_avg),2);//Σ(yi-y_avg)^2

}
}
SST=SS_T.sum();




R_square=1- SSE/SST;

Y_R=X_Nor*Theta_2;

MatrixXd SS_R(nn,1);

//-------------總平方和SSR(實際結果Vs平均值)---------------//

for(int i=0;i<Y.col(0).size();++i){
for(int j=0;j<Y.row(0).size();++j){

SS_R(i,j)=pow((Y_R(i,j)-Y_Nor_avg),2);//Σ(y_hat_i-y_avg)^2


}
}

 SSR=SS_R.sum();
f=(SSR)/(SSE);

F=f*(nn-mm-1)/mm;


//-----------------------複相關分析-------------------------// 






oFile <<"epoch="<< k  << "  ,  " <<" MSE= "<< "  ,  "<<J[k] << "  ,  " << R_square<< endl;


}





for(int i=0;i<Y.col(0).size();++i){
for(int j=0;j<Y.row(0).size();++j){

Y_Inv_nor(i,j)=Y_R(i,j)*(Y.maxCoeff()-Y.minCoeff())+Y_avg(0,j);

}
}

for(int iii=0;iii<Y.col(0).size();++iii){
for(int jjj=0;jjj<Y.row(0).size();++jjj){

MAPE_ratio(iii,jjj)=(Y(iii,jjj)-Y_Inv_nor(iii,jjj))/Y(iii,jjj); 

}
}


MatrixXd COV1(nn,1);
MatrixXd COV2(nn,1);

PROD=(Y_Inv_nor).transpose()*Y;

PROD_Multi=PROD(0,0);



COV1=(Y_Inv_nor).array().square();
COV2=(Y).array().square();

S1=COV1.sum();
S2=COV2.sum();


Cossim=PROD_Multi/sqrt(S1*S2);


oFile <<Y_Inv_nor<<Y<<endl;

//cout<<endl;
//cout<<X<<endl;
//cout<<"X矩陣正規化"<<endl;
//cout<<endl;
//cout<<X_Nor<<endl;
//cout<<endl;
//cout<<" 房價 (Y)  "   <<endl;
//cout<<endl;
//cout<< Y <<endl;
//cout<<endl;
//cout<<"模擬後房價結果"<<endl;
//cout<<endl;
//cout<<Y_Inv_nor<<endl;
//cout<<endl;
cout<<"---------------------------------------------------"<<endl;
cout<<"|                     迴歸係數                     |"<<endl;
cout<<"---------------------------------------------------"<<endl;
cout<<endl;
for(int i=0;i<Theta_2.col(0).size();++i){
for(int j=0;j<Theta_2.row(0).size();++j){
cout<<"Beta_"<<i<<" = "<<Theta_2(i,j)<<endl;
cout<<endl;
}
}

cout<<"----------------------------------------------------"<<endl;
cout<<"|                 R平方值與擬合程度                 |"<<endl;
cout<<"----------------------------------------------------"<<endl;

cout<<"SSE="<<SSE<<endl;
cout<<endl;

cout<<"SST="<<SST<<endl;
cout<<endl;

cout<<"R_sqare=1-(SSE/SST)"<<endl;
cout<<endl;

cout<<"R_square="<<R_square<<endl;
cout<<endl;
if(R_square<0.5)

cout<<"欠擬合(underfitting)"<<endl;

else if (R_square>0.5&&R_square<0.9)

cout<<"擬合"<<endl;

else if (R_square>0.9)

cout<<"過擬合(overfitting)"<<endl;

cout<<"F-value"<<endl;

cout<<F<<endl;

cout<<"預測與實際準確度 "<<endl;  
cout<<Cossim<<endl;  

cout<<"MAPE值"<<endl; 

cout<< abs(MAPE_ratio.sum())/nn<<endl;           

}



