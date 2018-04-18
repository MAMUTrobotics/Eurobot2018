#include"Navigation.h"
#include<stdio.h>

extern Spielfeld feld;
extern SpielMissionen Spiel;
extern Mission* Missionsplan[2];
extern struct Kreuzung* Wegpunkte[15];
extern struct Strecke* Wegstrecke[15];
extern Fahrkommando Richtungsvorgaben[15];
extern int aktuelleAusrichtung;

void initMission(Mission* Mission, int ID, Kreuzung* Missionskreuzung, void (*Missionsfunktion)(void), struct Mission* PreMission)
{
	Mission->ID = ID;
	Mission->MissionErfuelltFlag = 0;
	Mission->Missionskreuzung = Missionskreuzung;
	Mission->MissionFunktion = Missionsfunktion;
	Mission->PreMission = PreMission;
}

void initSpielMissionen(SpielMissionen* Spiel)
{
	initMission(&Spiel->BieneGruen, 0, &feld.k_a, Mission_BieneGruen, NULL);
	initMission(&Spiel->Wuerfel1, 1, &feld.k_e, Mission_Wuerfel1, NULL);
	initMission(&Spiel->SchmutzwasserturmOrange, 2, &feld.k_c, Mission_SchmutzwasserturmOrange, NULL);
	initMission(&Spiel->SchmutzwasserspeicherOrange, 3, &feld.k_c, Mission_SchmutzwasserspeicherOrange, &Spiel->SchmutzwasserturmOrange);
	initMission(&Spiel->TrinkwasserGruen, 4, &feld.k_e, Mission_TrinkwasserGruen, NULL);
	initMission(&Spiel->Wuerfel2, 5, &feld.k_f, Mission_Wuerfel2, NULL);
	initMission(&Spiel->Wuerfel3, 6, &feld.k_b, Mission_Wuerfel3, NULL);
	initMission(&Spiel->KonstruktionsareaGruen, 7, &feld.k_g, Mission_KonstruktionsareaGruen, &Spiel->Wuerfel1);
	initMission(&Spiel->WasserspeicherGruen, 8, &feld.k_h, Mission_WasserspeicherGruen, &Spiel->TrinkwasserGruen);
	initMission(&Spiel->LichtschalterGruen, 9, &feld.k_j, Mission_LichtschalterGruen, NULL);
	initMission(&Spiel->Konstruktionsanleitung, 10, &feld.k_l, Mission_Konstruktionsanleitung, NULL);
	initMission(&Spiel->LichtschalterOrange, 11, &feld.k_o, Mission_LichtschalterOrange, NULL);
	initMission(&Spiel->KonstruktionsareaOrange, 12, &feld.k_p, Mission_KonstruktionsareaOrange, &Spiel->Wuerfel5);
	initMission(&Spiel->WasserspeicherOrange, 13, &feld.k_q, Mission_WasserspeicherOrange, &Spiel->TrinkwasserOrgange);
	initMission(&Spiel->Wuerfel4, 14, &feld.k_u, Mission_Wuerfel4, NULL);
	initMission(&Spiel->TrinkwasserOrgange, 15, &feld.k_t, Mission_TrinkwasserOrange, NULL);
	initMission(&Spiel->Wuerfel5, 16, &feld.k_s, Mission_Wuerfel5, NULL);
	initMission(&Spiel->Wuerfel6, 17, &feld.k_t, Mission_Wuerfel6, NULL);
	initMission(&Spiel->SchmutzwasserturmGruen, 18, &feld.k_v, Mission_SchmutzwasserturmGruen, NULL);
	initMission(&Spiel->SchmutzwasserspeicherGruen, 19, &feld.k_v, Mission_SchmutzwasserspeicherGruen, &Spiel->SchmutzwasserturmGruen);
	initMission(&Spiel->BieneOrange, 20, &feld.k_w, Mission_BieneOrange, NULL);

	Spiel->MissionAktiv = 0;
	Spiel->WegZuMissionGefunden = 0;
}

void Mission_BieneGruen(void)
{
	printf("Mission Biene Gruen \n");
	// Hier wird die Mission abgearbeitet


	// Mission wurde erfüllt Mission erfüllt Flag setzen und Mission aktiv zurücksetzen
	Spiel.BieneGruen.MissionErfuelltFlag = 1;   
	Spiel.MissionAktiv = 0;
	Spiel.WegZuMissionGefunden = 0;
}

void Mission_Wuerfel1(void)
{

}

void Mission_SchmutzwasserturmOrange(void)
{

}

void Mission_SchmutzwasserspeicherOrange(void)
{

}

void Mission_TrinkwasserGruen(void)
{
	printf("Mission Trinkwasser Gruen \n");
	// Hier wird die Mission abgearbeitet


	// Mission wurde erfüllt Mission erfüllt Flag setzen und Mission aktiv zurücksetzen
	Spiel.TrinkwasserGruen.MissionErfuelltFlag = 1;
	Spiel.MissionAktiv = 0;
	Spiel.WegZuMissionGefunden = 0;
}

void Mission_Wuerfel2(void)
{

}

void Mission_Wuerfel3(void)
{

}

void Mission_KonstruktionsareaGruen(void)
{

}

void Mission_WasserspeicherGruen(void)
{

}

void Mission_LichtschalterGruen(void)
{

}

void Mission_Konstruktionsanleitung(void)
{

}

void Mission_LichtschalterOrange(void)
{

}

void Mission_KonstruktionsareaOrange(void)
{

}

void Mission_WasserspeicherOrange(void)
{

}

void Mission_Wuerfel4(void)
{

}

void Mission_TrinkwasserOrange(void)
{

}

void Mission_Wuerfel5(void)
{

}

void Mission_Wuerfel6(void)
{

}

void Mission_SchmutzwasserturmGruen(void)
{

}

void Mission_SchmutzwasserspeicherGruen(void)
{

}

void Mission_BieneOrange(void)
{

}

/* CarryOutMission wird zyklisch aufgerufen
*/

void CarryOutMission(void)
{
	static int count = 0;
	if (Spiel.MissionAktiv)    // Wenn eine Mission aktiv ist
	{
		if (feld.AktuelleKreuzung == Spiel.AktiveMission->Missionskreuzung)
		{
			Spiel.AktiveMission->MissionFunktion();
		}
		else
		{
			if (Spiel.WegZuMissionGefunden == 0)   // Nach Ausweichmanöver muss Weg neu berechnet werden
			{
				
				if (Wegfindung(NULL, feld.AktuelleKreuzung, Spiel.AktiveMission->Missionskreuzung))
				{
					Spiel.WegZuMissionGefunden = 1;
					printf("Weg gefunden \n");
					generiereRichtungswechsel(Richtungsvorgaben, aktuelleAusrichtung);
					printf("Richtungen generiert \n");
				}
			}
		}
		
		
	}

	else
	{
		if (Missionsplan[count] != NULL)
		{
			if (Missionsplan[count]->PreMission == NULL)
			{
				if (Missionsplan[count]->MissionErfuelltFlag == 0)
				{
					Spiel.AktiveMission = Missionsplan[count];
					Spiel.MissionAktiv = 1;
					
				}
			}
			else 
			{
				if (Missionsplan[count]->MissionErfuelltFlag == 0 && Missionsplan[count]->PreMission->MissionErfuelltFlag ==1)
				{
					Spiel.AktiveMission = Missionsplan[count];
					Spiel.MissionAktiv = 1;
					
				}
			}
			count++;
		}
		else
		{
			count = 0;      // n auf 0 setzen um Missionen erneut durchzuschauen
			printf("keine weitere Mission vorhanden \n");
		}
	}
}