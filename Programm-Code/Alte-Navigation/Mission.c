#include"Navigation.h"
#include<stdio.h>

extern Mission* Missionsplan[2];
extern struct Kreuzung* Wegpunkte[15];
extern struct Strecke* Wegstrecke[15];
extern Fahrkommando Richtungsvorgaben[15];

extern struct Spielfeld feld;

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