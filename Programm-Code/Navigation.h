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

	int Streckenl�nge;
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
	Kreuzung k_a;	// Biene Gr�n
	Kreuzung k_b;	// W�rfel
	Kreuzung k_c;	// Schmutzwasserturm Orange  Schmutzwasserspeicher Orange
	Kreuzung k_d;
	Kreuzung k_e;	// Trinkwasser Gr�n       W�rfel
	Kreuzung k_f;	// W�rfel
	Kreuzung k_g;	// Konstruktionsarea Gr�n
	Kreuzung k_h;	// Startzone Gr�n   Wasserspeicher Gr�n
	Kreuzung k_i;
	Kreuzung k_j;	// Lichtschalter Gr�n
	Kreuzung k_k;
	Kreuzung k_l;	// Konstruktionsanleitung
	Kreuzung k_m;
	Kreuzung k_n;
	Kreuzung k_o;	// Lichtschalter Orange
	Kreuzung k_p;	// Konstruktionsarea Orange
	Kreuzung k_q;	// Startpunkt Orange		Wasserspeicher Orange
	Kreuzung k_r;
	Kreuzung k_s;	// W�rfel
	Kreuzung k_t;	// Trinkwasser Orange      W�rfel
	Kreuzung k_u;	// W�rfel
	Kreuzung k_v;	// Schmutzwasser Gr�n     Schmutzwasserspeicher Gr�n
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


