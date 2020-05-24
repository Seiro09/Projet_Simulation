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

#define K 20
#define DELTA 10

random_device dev;
mt19937 rng(dev());
uniform_real_distribution<> dist(0.0,1.0);

double horloge=0;
int paquets_totaux=0;
int paquets_traites=0;

typedef struct donneAnneau{
  vector<int> remplissageAnneau;
  vector<int> AttenteStation1;
  vector<int> AttenteStation10;
}AnneauDonne;

enum type {
  SORTIE_CONTENEUR,
  ARRIVEE_CONTENEUR,
  ENTREE_CONTENEUR,
  TIC
 };

typedef struct Event{
  type type_event;
  double date;
  int station;
  double tmpArrivee;
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

int remplissageAnneau(ANNEAU *anneau){
  int cmp=0;
  for (int i = 0; i < 150; i++) {
    if (anneau->anneau.at(i)==-1) {
      cmp++;
    }
  }
  return 150-cmp;
}

int Generer_Duree(vector<double> fonction, double valeur){
  int compteur=0;
  while(fonction.at(compteur)<valeur){
    compteur++;
  }
  return compteur-1;
}

void Decale_Anneau(ANNEAU * Anneau){
  int tmp=Anneau->anneau.at(Anneau->anneau.size()-1);
  for (int i = Anneau->anneau.size()-2; i >= 0; i--) {
    Anneau->anneau.at(i+1)=Anneau->anneau.at(i);
  }
  Anneau->anneau.at(0)=tmp;

}

void Init_Anneau(ANNEAU *anneau,STATION *station ){
  anneau->anneau.fill(-1);
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
        // cout << "evenement : " << "TIC" << " " <<e.date << " " <<e.station << endl;
      break;
    case ARRIVEE_CONTENEUR:
      // cout << "evenement : "<< "ARRIVEE_CONTENEUR" << " " <<e.date << " " <<e.station << endl;
      break;
    case ENTREE_CONTENEUR :
      // cout << "evenement : "<< "ENTREE_CONTENEUR" << " " <<e.date << " " <<e.station << endl;
      break;
    case SORTIE_CONTENEUR :
      // cout << "evenement : "<< "SORTIE_CONTENEUR" << " " <<e.date << " " <<e.station << endl;
      break;
    default :
      break;
  }
}

void init_echeancier(Echeancier *e,double valeur ,vector<double> fonction, int station){
  event E1;
  E1.type_event=ARRIVEE_CONTENEUR;
  E1.date=horloge+Generer_Duree(fonction,valeur);
  E1.station=station;
  e->echeancier.push(E1);

}


void Evenement_Tic(ANNEAU *a, Echeancier *e, event E){
  Decale_Anneau(a);
  event E1;
  E1.type_event=TIC;
  E1.date=horloge+1;
  E1.station=-1;
  e->echeancier.push(E1);
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

int dernier_paquet(ANNEAU *A , int station){
  for (int i = ((150/K)*station); i < (150/K)*station+10; i++) {
    if (A->anneau.at(i%150)==station) {
      if (i-(150/K)*station <10) {
        return i-(150/K)*station;
      }
      else{
        return DELTA;
      }
    }
  }
  return DELTA;
}

void Arrivee_Conteneur(ANNEAU *anneau, STATION *s, int station, Echeancier *e, double valeur, vector<double> fonction ) {
  PAQUETS p1;
  event E1;
  //Creation de l'evenement Entree Conteneur
  E1.type_event=ENTREE_CONTENEUR;
  p1.num_paquet=station;
  E1.station=station;

  // Calcul le temps d'attente a partir de l'anneau
  int attenteA = (DELTA-dernier_paquet(anneau,station));

  // Calcul le nombre de paquets present pour une station donnée
  int nb_station = count_station(s,station);
  int attenteS;
  if (nb_station!=0) {
     attenteS =(count_station(s,station)-1)*DELTA;
  }
  else{
     attenteS=0;
  }
  //int attenteS =(count_station(s,station)-1)*DELTA;
  if(E1.station == 0){
    // std::cout << "attente : " << attenteA+attenteS << '\n';
    // std::cout << "attenteA : " << attenteA << '\n';
    // std::cout << "attenteS : " << attenteS << '\n';
  }
  // Calcul de la date
  E1.date=horloge+attenteA+attenteS;
  E1.tmpArrivee=horloge;
  if(E1.station == 0){
    cout << "Date : " << E1.date << " || Temps arrivée : " << E1.tmpArrivee << "|| Paquets : " << attenteA << endl;
  }
  s->v.push_back(p1);
  paquets_totaux++;
  e->echeancier.push(E1);

  // Creation prochaine Arrivée conteneur .
  E1.type_event=ARRIVEE_CONTENEUR;
  E1.station=station;
  E1.date=horloge+Generer_Duree(fonction,dist(rng));

  e->echeancier.push(E1);
}

int supression_paquet(STATION *S , int station){
  for (int i = 0; i < S->v.size(); i++) {
    if(S->v.at(i).num_paquet==station) return i;
  }
  return -1;
}

void Ecrire_Fichier(string fichierSortie, vector<int> v){
  std::ofstream os(fichierSortie);
  if(os.is_open()){
    for (int i = 0; i < v.size(); i++) {
      os << i << "    " << v.at(i) << endl;
    }
  }
  os.close();
}

void Entree_Conteneur(Anneau *a, STATION *s, Echeancier *e, int station,donneAnneau *da,event E){

  if(a->anneau.at((150/K)*station)!=-1){
    Event E1;
    E1.type_event=ENTREE_CONTENEUR;
    E1.date=horloge+1;
    E1.station=station;
    E1.tmpArrivee = E.tmpArrivee;
    e->echeancier.push(E1);
  }
  else{
    paquets_traites++;
    a->anneau.at((150/K)*station)=station;
    Event E1;
    E1.type_event=SORTIE_CONTENEUR;
    E1.date=horloge+150;
    E1.station=station;
    e->echeancier.push(E1);
    if(E1.station == 0) {
      da->AttenteStation1.push_back(E.date-E.tmpArrivee);

      //std::cout << "attente : " << E.date-E.tmpArrivee << '\n';
    }
    if(E1.station == 9) da->AttenteStation10.push_back(E.date-E.tmpArrivee);
    // std::cout << "Paquets "<< station << " a été traité" << '\n';
    // std::cout << "nombre de paquets traités = " << paquets_traites<< '\n';
    s->v.erase(s->v.begin()+supression_paquet(s,station));
  }
}

void Sortie_Conteneur(Anneau *a,int station) {
  // std::cout << "je suis sortie_conteneur" << '\n';
  a->anneau.at((150/K)*station)=-1;
  //std::cout << "Paquets "<<station << " est sorti" << '\n';
}

void exec_evenement(echeancier *e,ANNEAU *anneau , STATION *station ,double valeur ,vector<double> fonction ,AnneauDonne *AD){
  event E1;
  //std::cout << "taille : "<<e->echeancier.size() << '\n';
  while (e->echeancier.top().date==horloge) {
    afficher_evenement(e->echeancier.top());
    //std::cout << "echeancier tailles : " <<e->echeancier.size() << '\n';
    switch (e->echeancier.top().type_event) {
      case TIC:
        E1=e->echeancier.top();
        break;
      case ARRIVEE_CONTENEUR:
        Arrivee_Conteneur(anneau,station,e->echeancier.top().station,e,valeur,fonction);
        break;
      case ENTREE_CONTENEUR :
        Entree_Conteneur(anneau, station,e, e->echeancier.top().station,AD, e->echeancier.top());
        break;
      case SORTIE_CONTENEUR :
        Sortie_Conteneur(anneau,e->echeancier.top().station);
        break;
      default :
        break;
    }
    e->echeancier.pop();
    if(e->echeancier.empty()) break;
  }
  Evenement_Tic(anneau, e, E1 );
  AD->remplissageAnneau.push_back(remplissageAnneau(anneau));
  horloge++;
}


void simulation(ANNEAU *anneau , STATION *station, echeancier* e){
  AnneauDonne AD;
  Init_Anneau(anneau,station);
  ifstream fichier("message.txt");
  std::vector<double> inter_arrivees = Fct_Repart(fichier);
  event E1;
  E1.type_event=TIC;
  E1.date=0;
  E1.station=-1;
  e->echeancier.push(E1);
  for (int i = 0; i < K; i++) {
    init_echeancier(e,dist(rng),inter_arrivees,i);
  }
  while (horloge < 3500) {
    //std::cout << "TEMPS" << horloge << '\n' << '\n';
    exec_evenement(e,anneau,station,dist(rng),inter_arrivees,&AD);
  }
  Ecrire_Fichier("remplissageAnneau",AD.remplissageAnneau);
  Ecrire_Fichier("Station1",AD.AttenteStation1);
  Ecrire_Fichier("Station10",AD.AttenteStation10);
  std::cout << "taille echeancier : "<<e->echeancier.size() << '\n';
  std::cout << "paquets_totaux : " <<paquets_totaux<< '\n';
  std::cout << "paquets_traites : " <<paquets_traites<< '\n';

}



int main(int argc, char const *argv[]) {
  ANNEAU anneau;
  STATION station;
  echeancier e;
  simulation(&anneau,&station,&e);
}
