#include<stdio.h>
#include"Navigation.h"

Spielfarbe = GRUEN;


Mission* Missionsplan[] = { &Spiel.BieneGruen, &Spiel.TrinkwasserGruen, NULL };    // Missionsplan in welcher Reihenfolge Missionen ausgeführt werden sollen
Kreuzung* Wegpunkte[15] = { NULL };
Strecke* Wegstrecken[15] = { NULL };
Fahrkommando Richtungsvorgaben[15] = { LEER };



int findeWeg( Strecke* vorherigeStrecke, Kreuzung* Start, Kreuzung* Ziel, int n)
{
	
	if (Start == Ziel)
	{
		
		Wegpunkte[n] = Start;
		return 1;
	}
	else
	{
		
		if (Start->nextStreckeN != NULL && Start->nextStreckeN != vorherigeStrecke)
		{
			if (findeWeg(Start->nextStreckeN, Start->nextStreckeN->nextKreuzungNO, Ziel, n+1))
			{
				Wegpunkte[n] = Start;
				Wegstrecken[n] = Start->nextStreckeN;
				return 1;
			}
		}
		if (Start->nextStreckeO != NULL && Start->nextStreckeO != vorherigeStrecke)
		{
			if (findeWeg(Start->nextStreckeO, Start->nextStreckeO->nextKreuzungNO, Ziel, n+1))
			{
				Wegpunkte[n] = Start;
				Wegstrecken[n] = Start->nextStreckeO;
				return 1;
			}
		}
		
		if (Start->nextStreckeS != NULL && Start->nextStreckeS != vorherigeStrecke)
		{
			if (findeWeg(Start->nextStreckeS, Start->nextStreckeS->nextKreuzungSW, Ziel, n+1))
			{
				Wegpunkte[n] = Start;
				Wegstrecken[n] = Start->nextStreckeS;
				return 1;
			}
		}
		
		if (Start->nextStreckeW != NULL && Start->nextStreckeW != vorherigeStrecke)
		{
			if (findeWeg(Start->nextStreckeW, Start->nextStreckeW->nextKreuzungSW, Ziel, n+1))
			{
				Wegpunkte[n] = Start;
				Wegstrecken[n] = Start->nextStreckeW;
				return 1;
			}
		}
		
		//Weg[n - 1] = NULL;
		return 0;
		

	}

}


bool Wegfindung(Strecke* vorherigeStrecke, Kreuzung* Start, Kreuzung* Ziel)
{
	int n = 0;
	for (int i = 0; i < 15; i++)
	{
		Wegpunkte[i] = NULL;
		Wegstrecken[i] = NULL;
	}
	n = findeWeg(NULL, Start, Ziel, n);
	return n;
}

void generiereRichtungswechsel(Fahrkommando Richtungsvorgaben[],int aktuelleAusrichtung)
{
	int n = 0;
	int Ausrichtung = aktuelleAusrichtung;

	for (int i = 0; i < 15; i++)
	{
		Richtungsvorgaben[i] = LEER;
	}
	while (Wegpunkte[n] != NULL)
	{
		if (Wegstrecken[n] != NULL)
		{
			switch (Ausrichtung)
			{
			case NORD:
				if (Wegpunkte[n]->nextStreckeN == Wegstrecken[n])
				{
					if (Wegpunkte[n] == &feld.k_a)
					{
						Ausrichtung = OST;
						Richtungsvorgaben[n] = GERADEAUS;
					}
					else if (Wegpunkte[n] == &feld.k_w)
					{
						Ausrichtung = WEST;
						Richtungsvorgaben[n] = GERADEAUS;
					}
					else
					{
						Richtungsvorgaben[n] = GERADEAUS;
						Ausrichtung = NORD;
					}
					
				}
				if (Wegpunkte[n]->nextStreckeO == Wegstrecken[n])
				{
					Richtungsvorgaben[n] = RECHTS;
					Ausrichtung = OST;
				}
				if (Wegpunkte[n]->nextStreckeS == Wegstrecken[n])
				{
					Richtungsvorgaben[n] = DREHEN;
					Ausrichtung = SUED;
				}
				if (Wegpunkte[n]->nextStreckeW == Wegstrecken[n])
				{
					Richtungsvorgaben[n] = LINKS;
					Ausrichtung = WEST;
				}
				break;
			case OST:
				if (Wegpunkte[n]->nextStreckeN == Wegstrecken[n])
				{
					if (Wegpunkte[n] == &feld.k_a)
					{
						Ausrichtung = OST;
						Richtungsvorgaben[n] = LINKS;
					}
					else if (Wegpunkte[n] == &feld.k_w)
					{
						Ausrichtung = WEST;
						Richtungsvorgaben[n] = LINKS;
					}
					else 
					{
						Richtungsvorgaben[n] = LINKS;
						Ausrichtung = NORD;
					}
				}
				if (Wegpunkte[n]->nextStreckeO == Wegstrecken[n])
				{
					Richtungsvorgaben[n] = GERADEAUS;
					Ausrichtung = OST;
				}
				if (Wegpunkte[n]->nextStreckeS == Wegstrecken[n])
				{
					Richtungsvorgaben[n] = RECHTS;
					Ausrichtung = SUED;
				}
				if (Wegpunkte[n]->nextStreckeW == Wegstrecken[n])
				{
					Richtungsvorgaben[n] = DREHEN;
					Ausrichtung = WEST;
				}
				break;
			case SUED:
				if (Wegpunkte[n]->nextStreckeN == Wegstrecken[n])
				{
					if (Wegpunkte[n] == &feld.k_a)
					{
						Ausrichtung = OST;
						Richtungsvorgaben[n] = DREHEN;
					}
					else if (Wegpunkte[n] == &feld.k_w)
					{
						Ausrichtung = WEST;
						Richtungsvorgaben[n] = DREHEN;
					}
					else
					{
						Richtungsvorgaben[n] = DREHEN;
						Ausrichtung = NORD;
					}
				}
				if (Wegpunkte[n]->nextStreckeO == Wegstrecken[n])
				{
					Richtungsvorgaben[n] = LINKS;
					Ausrichtung = OST;
				}
				if (Wegpunkte[n]->nextStreckeS == Wegstrecken[n])
				{
					Richtungsvorgaben[n] = GERADEAUS;
					Ausrichtung = SUED;
				}
				if (Wegpunkte[n]->nextStreckeW == Wegstrecken[n])
				{
					Richtungsvorgaben[n] = RECHTS;
					Ausrichtung = WEST;
				}
				break;
			case WEST:
				if (Wegpunkte[n]->nextStreckeN == Wegstrecken[n])
				{
					if (Wegpunkte[n] == &feld.k_a)
					{
						Ausrichtung = OST;
						Richtungsvorgaben[n] = RECHTS;
					}
					else if (Wegpunkte[n] == &feld.k_w)
					{
						Ausrichtung = WEST;
						Richtungsvorgaben[n] = RECHTS;
					}
					else
					{
						Richtungsvorgaben[n] = RECHTS;
						Ausrichtung = NORD;
					}
				}
				if (Wegpunkte[n]->nextStreckeO == Wegstrecken[n])
				{
					Richtungsvorgaben[n] = DREHEN;
					Ausrichtung = OST;
				}
				if (Wegpunkte[n]->nextStreckeS == Wegstrecken[n])
				{
					Richtungsvorgaben[n] = LINKS;
					Ausrichtung = SUED;
				}
				if (Wegpunkte[n]->nextStreckeW == Wegstrecken[n])
				{
					Richtungsvorgaben[n] = GERADEAUS;
					Ausrichtung = WEST;
				}
				break;
			default:
				break;
			}
		}
		n++;
	}
}


// Nach dem ausweichen muss eine neue Linie gefunden werden und daraufhin die aktuelle Kreuzung korrigiert werden,
// ebenso muss im struct SpielMissionen "bool WegZuMissionGefunden" auf 0 gesetzt werden um einen neuen Weg zu finden

void KorrekturAktKreuzung(void)
{
	// Korrektur der Aktuellen Kreuzung
	Spiel.WegZuMissionGefunden = 0;     //CarryOutMission berechnet neuen Weg zum Ziel
}

//
//void main(void)
//{
//
//	int n = 0;
//	while (1)
//	{
//		CarryOutMission();
//		//feld.AktuelleKreuzung = Spiel.AktiveMission->Missionskreuzung;
//		n++;
//		if (n == 20)
//		{
//			feld.AktuelleKreuzung = Spiel.AktiveMission->Missionskreuzung;
//			aktuelleAusrichtung = SUED;
//		}
//		if(n==40)
//		{
//			feld.AktuelleKreuzung = Spiel.AktiveMission->Missionskreuzung;
//		}
//	}
//
//
//
//
//
//	/*if (Wegfindung(NULL, feld.AktuelleKreuzung, &feld.k_w, Wegpunkte, Wegstrecke))
//	{
//		generiereRichtungswechsel(Wegpunkte, Wegstrecke, Richtungsvorgaben, aktuelleAusrichtung);
//		printf("Weg gefunden\n");
//		Spiel.BieneGruen.MissionFunktion();
//		
//	}
//	else 
//	{
//		printf("Kein Weg gefunden");
//	}
//	if (Wegfindung(NULL, &feld.k_h, &feld.k_f, Wegpunkte, Wegstrecke))
//	{
//		generiereRichtungswechsel(Wegpunkte, Wegstrecke, Richtungsvorgaben, aktuelleAusrichtung);
//		printf("Weg gefunden\n");
//	}
//	else
//	{
//		printf("Kein Weg gefunden");
//	}*/
//}