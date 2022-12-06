//
//  makePFM.cpp
//  
//
//  Created by Ghada Bakbouk on 3/20/22.
//

#include <stdio.h>
#include <fstream>
#include "particles.h"

using namespace std;

int main( int argc, char** argv ) {
    ofstream out( "/Users/macbookair/Desktop/GraphicsPapers/Walk_on_Spheres/pfm/rod_from_matlab.pfm", std::ios::binary );
    out << "PF" << "\n";
    out << 128 << " ";
    out << 128 << "\n";
    out << "-1.000000" << "\n";
    fstream fin;
    fin.open("values_for_rod.csv", ios::in);
    string temp, line, snumber;
    double number;
    color col;
    while (fin >> temp){
        getline(fin, line);
       // cout << temp << endl;
        stringstream s(temp);
        while (getline(s, snumber, ',')){
            number = stof(snumber);
            col.r = number;
            col.g = number;
            col.b = number;
            //cout << number << endl;
             out.write(reinterpret_cast<char*>(&col), sizeof(col));
            
        }
    }
     out.close();
    return 0;
}
