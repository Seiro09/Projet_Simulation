#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <vector>
#include <array>
#include <string>
#include <random>
#include <functional>
#include <queue>

using namespace std;

#define K 4
#define DELTA 10

double horloge=0;
int paquets_totaux=0;
int paquets_perdus=0;
int paquets_traites=0;


enum type {
  TIC,
  ARRIVEE_CONTENEUR,
  ENTREE_CONTENEUR,
  SORTIE_CONTENEUR
 };

typedef struct Event{
  type type_event;
  double date;
  int station;

}event;


struct Comparedate{
  bool operator()(event const& e1 ,event const& e2 ){
    return e1.date>e2.date;
  }
};
 typedef struct Echeancier{
   priority_queue<event,std::vector<event> ,Comparedate> echeancier;
 }echeancier;


typedef struct paquets{
  int num_paquet;
}PAQUETS;

typedef struct Anneau{
  array<int,150> anneau;
}ANNEAU;

typedef struct station{
  std::vector<PAQUETS> v;
}STATION;


int Generer_Duree(vector<double> fonction, double valeur){
  int compteur=0;
  while(fonction.at(compteur)<valeur){
    compteur++;
  }
  return compteur-1;
}

void Init_Anneau(ANNEAU *anneau,STATION *station ){
  anneau->anneau.fill(-1);
}

void Decale_Anneau(ANNEAU * Anneau){
  int tmp=Anneau->anneau.at(Anneau->anneau.size()-1);
  for (int i = Anneau->anneau.size()-2; i >= 0; i--) {
    Anneau->anneau.at(i+1)=Anneau->anneau.at(i);
  }
  Anneau->anneau.at(0)=tmp;

}
vector<double> Fct_Repart(ifstream &fichier){
  vector<double> fonction;
  string ligne;
  while (getline(fichier, ligne)){
    istringstream iss(ligne);
    string mot;
    iss >> mot;
    iss >> mot;
    if(fonction.empty()) fonction.push_back(stod(mot));
    else fonction.push_back(fonction.back()+stod(mot));
  }
  cout << fonction.back() << endl;
  return fonction;
}

void afficher_evenement(event e){
  switch (e.type_event) {
    case TIC:
        cout << "evenement : " << "TIC" << " " <<e.date << " " <<e.station << endl;
      break;
    case ARRIVEE_CONTENEUR:
      cout << "evenement : "<< "ARRIVEE_CONTENEUR" << " " <<e.date << " " <<e.station << endl;
      break;
    case ENTREE_CONTENEUR :
      cout << "evenement : "<< "ENTREE_CONTENEUR" << " " <<e.date << " " <<e.station << endl;
      break;
    case SORTIE_CONTENEUR :
      cout << "evenement : "<< "SORTIE_CONTENEUR" << " " <<e.date << " " <<e.station << endl;
      break;
    default :
      break;
  }
}
void afficher_echeancier(echeancier *e){
  priority_queue<event,std::vector<event> ,Comparedate> *tmp =&e->echeancier ;
  while (!tmp->empty()) {
    afficher_evenement(tmp->top());
    tmp->pop();
  }
}

void Evenement_Tic(ANNEAU *a, Echeancier *e, event E, double valeur ,vector<double> fonction ){
  Decale_Anneau(a);
  event E1;
  for (int i = 0; i < K; i++) {
    E1.type_event=ARRIVEE_CONTENEUR;
    E1.date=horloge+Generer_Duree(fonction,valeur);
    E1.station=i;
    e->echeancier.push(E1);
    //afficher_evenement(E1);
    std::cout << "echeancier"<< '\n';
  }
  E1.type_event=TIC;
  E1.date=horloge+1;
  E1.station=-1;
  //afficher_evenement(E1);
  e->echeancier.push(E1);
  afficher_echeancier(e);
  std::cout << "echeancier taille " << e->echeancier.size()<< '\n';
}

int recherche_station(ANNEAU *anneau,int station){
  for (int i = 0; i < 150; i++) {
    if(anneau->anneau.at(i)==station) return i;
  }
  return -1;
}

int count_station(STATION *s , int station){
  int cmp=0;
  for (int i = 0; i < s->v.size(); i++) {
    if (s->v.at(i).num_paquet==station) {
      cmp++;
    }
  }
  return cmp;
}

void Arrivee_Conteneur(ANNEAU *anneau,STATION *s, int station,Echeancier *e){
  PAQUETS p1;
  event E1;
  E1.type_event=ENTREE_CONTENEUR;
  p1.num_paquet=station;
  E1.station=station;
  int find=recherche_station(anneau,station);
  if(find == -1){
    E1.date=horloge;
  }
  else{
    E1.date=horloge+(count_station(s,station)*DELTA);
  }
  s->v.push_back(p1);
  std::cout << "FILE CAPACITE : " <<s->v.size() << " TEMPS : " << horloge << '\n';
  paquets_totaux++;
  afficher_evenement(E1);
  e->echeancier.push(E1);
}

int supression_paquet(STATION *S , int station){
  for (int i = 0; i < S->v.size(); i++) {
    if(S->v.at(i).num_paquet==station) return i;
  }
  return -1;
}

void Entree_Conteneur(Anneau *a, STATION *s, Echeancier *e, int station){
  if(a->anneau.at((150/K)*station)!=-1){
    paquets_perdus++;
    //std::cout << "Paquets "<< station << " a été perdu" << '\n';
    //std::cout << "nombre de paquets perdus = " << paquets_perdus<< '\n';
  }
  else{
    paquets_traites++;
    a->anneau.at((150/K)*station)=station;
    Event E1;
    E1.type_event=SORTIE_CONTENEUR;
    E1.date=horloge+150;
    E1.station=station;
    //afficher_evenement(E1);
    e->echeancier.push(E1);
    //std::cout << "Paquets "<< station << " a été traité" << '\n';
    //std::cout << "nombre de paquets traités = " << paquets_traites<< '\n';
  }
  s->v.erase(s->v.begin()+supression_paquet(s,station));
}

void Sortie_Conteneur(Anneau *a,int station) {
  a->anneau.at((150/K)*station)=-1;
  //std::cout << "Paquets "<<station << " est sorti" << '\n';
}

void exec_evenement(echeancier *e,ANNEAU *anneau , STATION *station ,double valeur ,vector<double> fonction ){
  event E1 = e->echeancier.top();
  e->echeancier.pop();
  event E2;
  Evenement_Tic(anneau, e, E1 ,valeur , fonction);
  // while (E1.date==horloge) {
  //   //std::cout << "echeancier tailles : " <<e->echeancier.size() << '\n';
  //   switch (E1.type_event) {
  //     case TIC:
  //       E2=E1;
  //       break;
  //     case ARRIVEE_CONTENEUR:
  //       Arrivee_Conteneur(anneau,station,E1.station,e);
  //       break;
  //     case ENTREE_CONTENEUR :
  //       Entree_Conteneur(anneau, station,e, E1.station);
  //       break;
  //     case SORTIE_CONTENEUR :
  //       Sortie_Conteneur(anneau,E1.station);
  //       break;
  //     default :
  //       break;
  //   }
  // }
  horloge++;
}


void simulation(ANNEAU *anneau , STATION *station, echeancier* e){
  random_device dev;
  mt19937 rng(dev());
  uniform_real_distribution<> dist(0.0,1.0);
  Init_Anneau(anneau,station);
  ifstream fichier("message.txt");
  std::vector<double> inter_arrivees = Fct_Repart(fichier);
  while (horloge < 1) {
    //std::cout << "echeancier taille : " <<e->echeancier.size() << '\n';
    exec_evenement(e,anneau,station,dist(rng),inter_arrivees);
    //afficher_echeancier(e);

  }
  std::cout << "paquets_totaux : " <<paquets_totaux<< '\n';
  std::cout << "paquets_perdu : " <<paquets_perdus<< '\n';
  std::cout << "paquets_traites : " <<paquets_traites<< '\n';

}



int main(int argc, char const *argv[]) {
  ANNEAU anneau;
  STATION station;
  echeancier e;
  event e1;
  e1.type_event=TIC;
  e1.date=0;
  e1.station=-1;
  afficher_evenement(e1);
  e.echeancier.push(e1);
  simulation(&anneau,&station,&e);
}
