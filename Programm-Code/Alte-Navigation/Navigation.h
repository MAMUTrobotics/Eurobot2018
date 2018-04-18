#pragma once
#include "stdbool.h"
#include"Mission.h"

#define GRUEN 0
#define ORANGE 1


enum Fahrkommando
{
	LEER,
	GERADEAUS,
	LINKS,
	RECHTS,
	DREHEN
};

enum Streckentyp
{
	GERADE,
	GEBOGEN
};


enum Himmelsrichtung {
	NORD,
	OST,
	SUED,
	WEST
};




struct Strecke {

	int Streckenlaenge;
	enum Streckentyp Typ;
	struct Kreuzung* nextKreuzungNO;   // Strecken zwischen Kreuzungen
	struct Kreuzung* nextKreuzungSW;
	//Kreuzung* nextKreuzungS;
	//Kreuzung* nextKreuzungW;
};



struct Kreuzung {
	char Name;
	bool Mission;
	int MissionN;	// statt int vll Zeiger auf eine Mission? todo
	int MissionO;
	int MissionS;
	int MissionW;
	int KoordinateX;
	int KoordinateY;
	struct Strecke* nextStreckeN;   // Strecken zwischen Kreuzungen
	struct Strecke* nextStreckeO;
	struct Strecke* nextStreckeS;
	struct Strecke* nextStreckeW;
};



struct Spielfeld {
	struct Kreuzung k_a;	// Biene Grün
	struct Kreuzung k_b;	// Würfel
	struct Kreuzung k_c;	// Schmutzwasserturm Orange  Schmutzwasserspeicher Orange
	struct Kreuzung k_d;
	struct Kreuzung k_e;	// Trinkwasser Grün       Würfel
	struct Kreuzung k_f;	// Würfel
	struct Kreuzung k_g;	// Konstruktionsarea Grün
	struct Kreuzung k_h;	// Startzone Grün   Wasserspeicher Grün
	struct Kreuzung k_i;
	struct Kreuzung k_j;	// Lichtschalter Grün
	struct Kreuzung k_k;
	struct Kreuzung k_l;	// Konstruktionsanleitung
	struct Kreuzung k_m;
	struct Kreuzung k_n;
	struct Kreuzung k_o;	// Lichtschalter Orange
	struct Kreuzung k_p;	// Konstruktionsarea Orange
	struct Kreuzung k_q;	// Startpunkt Orange		Wasserspeicher Orange
	struct Kreuzung k_r;
	struct Kreuzung k_s;	// Würfel
	struct Kreuzung k_t;	// Trinkwasser Orange      Würfel
	struct Kreuzung k_u;	// Würfel
	struct Kreuzung k_v;	// Schmutzwasser Grün     Schmutzwasserspeicher Grün
	struct Kreuzung k_w;	// Biene Orange

	struct Strecke  s_ab;
	struct Strecke  s_bc;
	struct Strecke  s_bd;
	struct Strecke  s_de;
	struct Strecke  s_df;
	struct Strecke  s_dg;
	struct Strecke  s_gh;
	struct Strecke  s_gi;
	struct Strecke  s_ij;
	struct Strecke  s_ik;
	struct Strecke  s_kl;
	struct Strecke  s_km;
	struct Strecke  s_kn;
	struct Strecke  s_no;
	struct Strecke  s_np;
	struct Strecke  s_pq;
	struct Strecke  s_pr;
	struct Strecke  s_rt;
	struct Strecke  s_rs;
	struct Strecke  s_ru;
	struct Strecke  s_uv;
	struct Strecke  s_uw;


	struct Kreuzung* AktuelleKreuzung;
	struct Strecke*  AktuelleStrecke;

};


struct Spielfeld feld;

bool Wegfindung( struct Strecke* vorherigeStrecke, struct Kreuzung* Start, struct Kreuzung* Ziel);
void generiereRichtungswechsel( enum Fahrkommando Richtungsvorgaben[], int aktuelleAusrichtung);

bool initStrecke( struct Strecke *strecke, struct Kreuzung * kreuzungNO, struct Kreuzung* kreuzungSW)  
{
	strecke->nextKreuzungNO = kreuzungNO;
	strecke->nextKreuzungSW = kreuzungSW;
	return true;
}

bool initKreuzung( struct Kreuzung *kreuzung,
                   struct Strecke *streckeN,
                   struct Strecke *streckeO,
                   struct Strecke *streckeS,
                   struct Strecke *streckeW,
                   char Name)   // todo Koordinaten und Mission
{
	kreuzung->nextStreckeN = streckeN;
	kreuzung->nextStreckeO = streckeO;
	kreuzung->nextStreckeS = streckeS;
	kreuzung->nextStreckeW = streckeW;
	kreuzung->Name = Name;
	return 1;
}
bool initSpielfeld( struct Spielfeld *feldTemp) {

	initKreuzung(&feldTemp->k_a, &feldTemp->s_ab, NULL, NULL, NULL, 'a');
	initKreuzung(&feldTemp->k_b, &feldTemp->s_bd, NULL, &feldTemp->s_bc, &feldTemp->s_ab, 'b');
	initKreuzung(&feldTemp->k_c, &feldTemp->s_bc, NULL, NULL, NULL, 'c');
	initKreuzung(&feldTemp->k_d, &feldTemp->s_dg, &feldTemp->s_df, &feldTemp->s_bd, &feldTemp->s_de, 'd');
	initKreuzung(&feldTemp->k_e, NULL, &feldTemp->s_de, NULL, NULL, 'e');
	initKreuzung(&feldTemp->k_f, NULL, NULL, NULL, &feldTemp->s_df, 'f');
	initKreuzung(&feldTemp->k_g, NULL, &feldTemp->s_gi, &feldTemp->s_dg, &feldTemp->s_gh, 'g');
	initKreuzung(&feldTemp->k_h, NULL, &feldTemp->s_gh, NULL, NULL, 'h');
	initKreuzung(&feldTemp->k_i, &feldTemp->s_ij, &feldTemp->s_ik, NULL, &feldTemp->s_gi, 'i');
	initKreuzung(&feldTemp->k_j, NULL, NULL, &feldTemp->s_ij, NULL, 'j');
	initKreuzung(&feldTemp->k_k, &feldTemp->s_kl, &feldTemp->s_kn, &feldTemp->s_km, &feldTemp->s_ik, 'k');
	initKreuzung(&feldTemp->k_l, NULL, NULL, &feldTemp->s_kl, NULL, 'l');
	initKreuzung(&feldTemp->k_m, &feldTemp->s_km, NULL, NULL, NULL, 'm');
	initKreuzung(&feldTemp->k_n, &feldTemp->s_no, &feldTemp->s_np, NULL, &feldTemp->s_kn, 'n');
	initKreuzung(&feldTemp->k_o, NULL, NULL, &feldTemp->s_no, NULL, 'o');
	initKreuzung(&feldTemp->k_p, NULL, &feldTemp->s_pq, &feldTemp->s_pr, &feldTemp->s_np, 'p');
	initKreuzung(&feldTemp->k_q, NULL, NULL, NULL, &feldTemp->s_pq, 'q');
	initKreuzung(&feldTemp->k_r, &feldTemp->s_pr, &feldTemp->s_rt, &feldTemp->s_ru, &feldTemp->s_rs, 'r');
	initKreuzung(&feldTemp->k_s, NULL, &feldTemp->s_rs, NULL, NULL, 's');
	initKreuzung(&feldTemp->k_t, NULL, NULL, NULL, &feldTemp->s_rt, 't');
	initKreuzung(&feldTemp->k_u, &feldTemp->s_ru, &feldTemp->s_uw, &feldTemp->s_uv, NULL, 'u');
	initKreuzung(&feldTemp->k_v, &feldTemp->s_uv, NULL, NULL, NULL, 'v');
	initKreuzung(&feldTemp->k_w, &feldTemp->s_uw, NULL, NULL, NULL, 'w');
	// .............

	initStrecke(&feldTemp->s_ab, &feldTemp->k_b, &feldTemp->k_a);
	initStrecke(&feldTemp->s_bc, &feldTemp->k_b, &feldTemp->k_c);
	initStrecke(&feldTemp->s_bd, &feldTemp->k_d, &feldTemp->k_b);
	initStrecke(&feldTemp->s_de, &feldTemp->k_d, &feldTemp->k_e);
	initStrecke(&feldTemp->s_df, &feldTemp->k_f, &feldTemp->k_d);
	initStrecke(&feldTemp->s_dg, &feldTemp->k_g, &feldTemp->k_d);
	initStrecke(&feldTemp->s_gh, &feldTemp->k_g, &feldTemp->k_h);
	initStrecke(&feldTemp->s_gi, &feldTemp->k_i, &feldTemp->k_g);
	initStrecke(&feldTemp->s_ij, &feldTemp->k_j, &feldTemp->k_i);
	initStrecke(&feldTemp->s_ik, &feldTemp->k_k, &feldTemp->k_i);
	initStrecke(&feldTemp->s_kl, &feldTemp->k_l, &feldTemp->k_k);
	initStrecke(&feldTemp->s_km, &feldTemp->k_k, &feldTemp->k_m);
	initStrecke(&feldTemp->s_kn, &feldTemp->k_n, &feldTemp->k_k);
	initStrecke(&feldTemp->s_no, &feldTemp->k_o, &feldTemp->k_n);
	initStrecke(&feldTemp->s_np, &feldTemp->k_p, &feldTemp->k_n);
	initStrecke(&feldTemp->s_pq, &feldTemp->k_q, &feldTemp->k_p);
	initStrecke(&feldTemp->s_pr, &feldTemp->k_p, &feldTemp->k_r);
	initStrecke(&feldTemp->s_rs, &feldTemp->k_r, &feldTemp->k_s);
	initStrecke(&feldTemp->s_rt, &feldTemp->k_t, &feldTemp->k_r);
	initStrecke(&feldTemp->s_ru, &feldTemp->k_r, &feldTemp->k_u);
	initStrecke(&feldTemp->s_uv, &feldTemp->k_u, &feldTemp->k_v);
	initStrecke(&feldTemp->s_uw, &feldTemp->k_w, &feldTemp->k_u);
	// ..............

	return 1;
}

int Spielfarbe;
