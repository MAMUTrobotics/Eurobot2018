#pragma once
#include"Navigation.h"

extern struct Kreuzung;

typedef struct Mission
{
	int ID;
	bool MissionErfuelltFlag;
	struct Kreuzung* Missionskreuzung;
	void(*MissionFunktion)(void);
	Mission* PreMission;
};



typedef struct SpielMissionen
{
	Mission BieneGruen;
	Mission Wuerfel1;
	Mission SchmutzwasserturmOrange;
	Mission SchmutzwasserspeicherOrange;
	Mission TrinkwasserGruen;
	Mission Wuerfel2;
	Mission Wuerfel3;
	Mission KonstruktionsareaGruen;
	Mission WasserspeicherGruen;
	Mission LichtschalterGruen;
	Mission Konstruktionsanleitung;
	Mission LichtschalterOrange;
	Mission KonstruktionsareaOrange;
	Mission WasserspeicherOrange;
	Mission Wuerfel4;
	Mission TrinkwasserOrgange;
	Mission Wuerfel5;
	Mission Wuerfel6;
	Mission SchmutzwasserturmGruen;
	Mission SchmutzwasserspeicherGruen;
	Mission BieneOrange;

	bool MissionAktiv;     //Muss auf 0 gesetzt werden wenn die aktive Mission blockiert ist
	Mission* AktiveMission;
	bool WegZuMissionGefunden;   // Wenn ausgewichen wurde muss dieser Wert auf 0 gesetzt werden

};

void initSpielMissionen(SpielMissionen* Spiel);

void initMission(Mission* Mission, int ID, Kreuzung* Missionskreuzung, void (*Missionsfunktion)(void), struct Mission* PreMission);

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