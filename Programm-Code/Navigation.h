#pragma once
#include"Mission.h"
#include"CarryOutMission.h"

# define NULL nullptr

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




typedef struct Strecke {

	int Streckenlänge;
	Streckentyp Typ;
	struct Kreuzung* nextKreuzungNO;   // Strecken zwischen Kreuzungen
	struct Kreuzung* nextKreuzungSW;
	//Kreuzung* nextKreuzungS;
	//Kreuzung* nextKreuzungW;
};



typedef struct Kreuzung {
	char Name;
	bool Mission;
	int MissionN;	// statt int vll Zeiger auf eine Mission? todo
	int MissionO;
	int MissionS;
	int MissionW;
	int KoordinateX;
	int KoordinateY;
	Strecke* nextStreckeN;   // Strecken zwischen Kreuzungen
	Strecke* nextStreckeO;
	Strecke* nextStreckeS;
	Strecke* nextStreckeW;
};



typedef struct Spielfeld {
	Kreuzung k_a;	// Biene Grün
	Kreuzung k_b;	// Würfel
	Kreuzung k_c;	// Schmutzwasserturm Orange  Schmutzwasserspeicher Orange
	Kreuzung k_d;
	Kreuzung k_e;	// Trinkwasser Grün       Würfel
	Kreuzung k_f;	// Würfel
	Kreuzung k_g;	// Konstruktionsarea Grün
	Kreuzung k_h;	// Startzone Grün   Wasserspeicher Grün
	Kreuzung k_i;
	Kreuzung k_j;	// Lichtschalter Grün
	Kreuzung k_k;
	Kreuzung k_l;	// Konstruktionsanleitung
	Kreuzung k_m;
	Kreuzung k_n;
	Kreuzung k_o;	// Lichtschalter Orange
	Kreuzung k_p;	// Konstruktionsarea Orange
	Kreuzung k_q;	// Startpunkt Orange		Wasserspeicher Orange
	Kreuzung k_r;
	Kreuzung k_s;	// Würfel
	Kreuzung k_t;	// Trinkwasser Orange      Würfel
	Kreuzung k_u;	// Würfel
	Kreuzung k_v;	// Schmutzwasser Grün     Schmutzwasserspeicher Grün
	Kreuzung k_w;	// Biene Orange

	Strecke  s_ab;
	Strecke  s_bc;
	Strecke  s_bd;
	Strecke  s_de;
	Strecke  s_df;
	Strecke  s_dg;
	Strecke  s_gh;
	Strecke  s_gi;
	Strecke  s_ij;
	Strecke  s_ik;
	Strecke  s_kl;
	Strecke  s_km;
	Strecke  s_kn;
	Strecke  s_no;
	Strecke  s_np;
	Strecke  s_pq;
	Strecke  s_pr;
	Strecke  s_rt;
	Strecke  s_rs;
	Strecke  s_ru;
	Strecke  s_uv;
	Strecke  s_uw;


	Kreuzung* AktuelleKreuzung;
	Strecke*  AktuelleStrecke;

};

bool Wegfindung(Strecke* vorherigeStrecke, Kreuzung* Start, Kreuzung* Ziel);
void generiereRichtungswechsel(Fahrkommando Richtungsvorgaben[], int aktuelleAusrichtung);


