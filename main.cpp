

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <vector>
#include <array>
#include <string>
#include <random>

using namespace std;


typedef struct paquets{
  int num_paquet;
  int attente;
}PAQUETS;

typedef struct Anneau{
  array<int,150> anneau;
}ANNEAU;

typedef struct station{
  std::vector<PAQUETS> v;
  float delta;
}STATION;


vector<double> Fct_Repart(ifstream &fichier){
  vector<double> fonction;
  string ligne;
  while (getline(fichier, ligne)){
    istringstream iss(ligne);
    string mot;
    iss >> mot;
    iss >> mot;
    fonction.push_back(fonction.back()+stof(mot));
  }
  cout << fonction.back() << endl;
  return fonction;
}

int Generer_Duree(vector<float> fonction, float valeur){
  int compteur=0;
  while(fonction.at(compteur)<valeur){
    compteur++;
  }
  return compteur-1;
}

void Init_Anneau(ANNEAU *anneau,STATION *station ){
  anneau->anneau.fill(0);
  station->delta=0;
}

void Decale_Anneau(ANNEAU * Anneau){
  int tmp=Anneau->anneau.at(Anneau->anneau.size()-1);
  for (int i = Anneau->anneau.size()-2; i >= 0; i--) {
    Anneau->anneau.at(i+1)=Anneau->anneau.at(i);
  }
  Anneau->anneau.at(0)=tmp;

}

void simulation(Anneau *anneau , STATION *station){


}


int main(int argc, char const *argv[]) {
  ANNEAU anneau;
  for (int i = 0; i < anneau.anneau.size(); i++) {
    anneau.anneau.at(i)=i;
    cout << anneau.anneau.at(i) <<endl ;
  }
  Decale_Anneau(&anneau);
  for (int i = 0; i < anneau.anneau.size(); i++) {
    cout << anneau.anneau.at(i) <<endl ;
  }

}
