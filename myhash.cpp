

#include <iostream>
#include "myhash.h"

using namespace std;
int main(){
    inicializarSemilla();
    string data_1 = "La transaccion tiene un valor de 455.5 dolares";
    string data_2 = "La transaccion tiene un valor de 455 dolares";
    cout<<"data_1 : ";
    cout<< data_1<<endl;
    string hash_data_1 = f_hash(data_1);
    cout<<"hash de la data : " ;
    cout << hash_data_1<<endl;

    cout<<"data_2 : ";
    cout<< data_2<<endl;
    string hash_data_2 = f_hash(data_2);
    cout<<"hash de la data : " ;
    cout << hash_data_2<<endl;

    return 0;

}