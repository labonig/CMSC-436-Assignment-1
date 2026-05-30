/****
 * Author: Laboni Goswami
 * Email: labonig1@umbc.edu
 * Description: This program reads data.csv and outputs a txt file with data formatted for vega-lite
 ****/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

const int NUM_COUNTRIES = 28;
const int COUNTRIES_PER_ROW = 7;

int main(int argc, char** argv){
    const string ifilename = "data.csv";
    const string ofilename = "formatted_data.txt";
    ifstream input(ifilename);
    ofstream output;
    string country, str_recovery_rate, trash;
    vector<pair<string,int>> country_rate;
    int numgray;

    //check if the input file opened successfully
    if(!input.is_open()){
        cerr << "Error: could not open " << ifilename << "\n";
        return 0;
    }

    // ignore the first line of the csv file
    getline(input, trash);

    //read the data and store it in the country_rate vector
    while(getline(input, country, ',') &&
          getline(input, str_recovery_rate))
          {
            country_rate.push_back(pair<string,int>(country, stoi(str_recovery_rate)));
          }

    input.close();

    //Check if the output file opens successfully
    output.open(ofilename);
    if(!output.is_open()){
        cerr << "Error: could not open " << ofilename << "\n";
        return 0;
    }
    
    //The output data is formatted for use in Vega-Lite
    // {"id":___, "country": ___, "color":___, "r:"___, "c":___},
    output << "\"data\": {\n"
           << "\t\"values\": [\n";

    //The following loop visually represents each country's percentage of recovered plastic by generating 100 colored squares per country 
    //Squares are generated from right to left, top to bottom in a 10x10 grid
    //For coloring, there are four general bins: "g" (gray), "b" (blue), "o" (orange), and "w" (white)
    //Nuances in the shade of blue/orange to use are given by a number n = 1, 2, 3,..., with increasing n being darker/more saturated
    //The placement of a country within the larger vis (a 4x7 grid) is determined by r (row) and c (column)
    for(int id = 1; id <= NUM_COUNTRIES; id++){
        numgray = 100 - country_rate.at(id-1).second;
        for(int subid = 1; subid <= 100; subid++){
            output << "\t\t{\"id\":" << subid << ", \"country\":\"" << country_rate.at(id-1).first << " " << country_rate.at(id-1).second << "%" << "\", \"color\":";
            if(numgray > 0){
                output << "\"g\"";
                numgray--;
            }
            else if(country_rate.at(id-1).first == "EU (AVG)")
                output << "\"w\"";
            else if(country_rate.at(id-1).second < 90)
                output << "\"b" << ceil(double(100 + 1 - subid)/10) << "\"";
            else
                output << "\"o" << ceil(double(100 + 1 - subid)/10) << "\"";

            output << ", \"r\":" << (id-1)/COUNTRIES_PER_ROW + 1
                   << ", \"c\":" << (id-1) % COUNTRIES_PER_ROW + 1;

            output << "}";
            if(id == NUM_COUNTRIES && subid == 100)
                output << "\n";
            else
                output << ",\n";
        }
    }
    output << "\t]\n}";
    
    output.close();

    return 0;
}