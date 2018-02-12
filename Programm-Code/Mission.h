#pragma once
#include "stdbool.h"
#include"Navigation.h"

struct Mission
{
	int ID;
	bool MissionErfuelltFlag;
	struct Kreuzung* Missionskreuzung;
	void(*MissionFunktion)(void);
	struct Mission* PreMission;
};


extern struct SpielMissionen Spiel;
uint8_t aktuelleAusrichtung;

struct SpielMissionen
{
	struct Mission BieneGruen;
	struct Mission Wuerfel1;
	struct Mission SchmutzwasserturmOrange;
	struct Mission SchmutzwasserspeicherOrange;
	struct Mission TrinkwasserGruen;
	struct Mission Wuerfel2;
	struct Mission Wuerfel3;
	struct Mission KonstruktionsareaGruen;
	struct Mission WasserspeicherGruen;
	struct Mission LichtschalterGruen;
	struct Mission Konstruktionsanleitung;
	struct Mission LichtschalterOrange;
	struct Mission KonstruktionsareaOrange;
	struct Mission WasserspeicherOrange;
	struct Mission Wuerfel4;
	struct Mission TrinkwasserOrgange;
	struct Mission Wuerfel5;
	struct Mission Wuerfel6;
	struct Mission SchmutzwasserturmGruen;
	struct Mission SchmutzwasserspeicherGruen;
	struct Mission BieneOrange;

	bool MissionAktiv;     //Muss auf 0 gesetzt werden wenn die aktive Mission blockiert ist
	struct Mission* AktiveMission;
	bool WegZuMissionGefunden;   // Wenn ausgewichen wurde muss dieser Wert auf 0 gesetzt werden
}Spiel;

void initMission(struct Mission* Mission,
                 int ID,
                 struct Kreuzung* Missionskreuzung,
                 void (*Missionsfunktion)(void),
                 struct Mission* PreMission)
{
	Mission->ID = ID;
	Mission->MissionErfuelltFlag = 0;
	Mission->Missionskreuzung = Missionskreuzung;
	Mission->MissionFunktion = Missionsfunktion;
	Mission->PreMission = PreMission;
}
void Mission_BieneGruen(void);
void Mission_Wuerfel1(void);
void Mission_SchmutzwasserturmOrange(void);
void Mission_SchmutzwasserspeicherOrange(void);
void Mission_TrinkwasserGruen(void);
void Mission_Wuerfel2(void);
void Mission_Wuerfel3(void);
void Mission_KonstruktionsareaGruen(void);
void Mission_WasserspeicherGruen(void);
void Mission_LichtschalterGruen(void);
void Mission_Konstruktionsanleitung(void);
void Mission_LichtschalterOrange(void);
void Mission_KonstruktionsareaOrange(void);
void Mission_WasserspeicherOrange(void);
void Mission_Wuerfel4(void);
void Mission_TrinkwasserOrange(void);
void Mission_Wuerfel5(void);
void Mission_Wuerfel6(void);
void Mission_SchmutzwasserturmGruen(void);
void Mission_SchmutzwasserspeicherGruen(void);
void Mission_BieneOrange(void);


void initSpielMissionen( struct SpielMissionen* SpielM)
{
	//initMission(&SpielM->BieneGruen, 0, feld.k_a, Mission_BieneGruen, NULL);
	//initMission(&SpielM->Wuerfel1, 1, &feld.k_e, Mission_Wuerfel1, NULL);
	//initMission(&SpielM->SchmutzwasserturmOrange, 2, &feld.k_c, Mission_SchmutzwasserturmOrange, NULL);
	//initMission(&SpielM->SchmutzwasserspeicherOrange, 3, &feld.k_c, Mission_SchmutzwasserspeicherOrange, &Spiel->SchmutzwasserturmOrange);
	//initMission(&SpielM->TrinkwasserGruen, 4, &feld.k_e, Mission_TrinkwasserGruen, NULL);
	//initMission(&SpielM->Wuerfel2, 5, &feld.k_f, Mission_Wuerfel2, NULL);
	//initMission(&SpielM->Wuerfel3, 6, &feld.k_b, Mission_Wuerfel3, NULL);
	//initMission(&SpielM->KonstruktionsareaGruen, 7, &feld.k_g, Mission_KonstruktionsareaGruen, &Spiel->Wuerfel1);
	//initMission(&SpielM->WasserspeicherGruen, 8, &feld.k_h, Mission_WasserspeicherGruen, &Spiel->TrinkwasserGruen);
	//initMission(&SpielM->LichtschalterGruen, 9, &feld.k_j, Mission_LichtschalterGruen, NULL);
	//initMission(&SpielM->Konstruktionsanleitung, 10, &feld.k_l, Mission_Konstruktionsanleitung, NULL);
	//initMission(&SpielM->LichtschalterOrange, 11, &feld.k_o, Mission_LichtschalterOrange, NULL);
	//initMission(&SpielM->KonstruktionsareaOrange, 12, &feld.k_p, Mission_KonstruktionsareaOrange, &Spiel->Wuerfel5);
	//initMission(&SpielM->WasserspeicherOrange, 13, &feld.k_q, Mission_WasserspeicherOrange, &Spiel->TrinkwasserOrgange);
	//initMission(&SpielM->Wuerfel4, 14, &feld.k_u, Mission_Wuerfel4, NULL);
	//initMission(&SpielM->TrinkwasserOrgange, 15, &feld.k_t, Mission_TrinkwasserOrange, NULL);
	//initMission(&SpielM->Wuerfel5, 16, &feld.k_s, Mission_Wuerfel5, NULL);
	//initMission(&SpielM->Wuerfel6, 17, &feld.k_t, Mission_Wuerfel6, NULL);
	//initMission(&SpielM->SchmutzwasserturmGruen, 18, &feld.k_v, Mission_SchmutzwasserturmGruen, NULL);
	//initMission(&SpielM->SchmutzwasserspeicherGruen, 19, &feld.k_v, Mission_SchmutzwasserspeicherGruen, &Spiel->SchmutzwasserturmGruen);
	//initMission(&SpielM->BieneOrange, 20, &feld.k_w, Mission_BieneOrange, NULL);

	SpielM->MissionAktiv = 0;
	SpielM->WegZuMissionGefunden = 0;
}

void Mission_BieneGruen(void);
void Mission_Wuerfel1(void);
void Mission_SchmutzwasserturmOrange(void);
void Mission_SchmutzwasserspeicherOrange(void);
void Mission_TrinkwasserGruen(void);
void Mission_Wuerfel2(void);
void Mission_Wuerfel3(void);
void Mission_KonstruktionsareaGruen(void);
void Mission_WasserspeicherGruen(void);
void Mission_LichtschalterGruen(void);
void Mission_Konstruktionsanleitung(void);
void Mission_LichtschalterOrange(void);
void Mission_KonstruktionsareaOrange(void);
void Mission_WasserspeicherOrange(void);
void Mission_Wuerfel4(void);
void Mission_TrinkwasserOrange(void);
void Mission_Wuerfel5(void);
void Mission_Wuerfel6(void);
void Mission_SchmutzwasserturmGruen(void);
void Mission_SchmutzwasserspeicherGruen(void);
void Mission_BieneOrange(void);

void CarryOutMission(void);
