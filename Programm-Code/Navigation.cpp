#include<stdio.h>
#include"Navigation.h"

int Spielfarbe = GRUEN;
Spielfeld feld;
SpielMissionen Spiel;
Mission* Missionsplan[] = { &Spiel.BieneGruen, &Spiel.TrinkwasserGruen, NULL };    // Missionsplan in welcher Reihenfolge Missionen ausgeführt werden sollen
Kreuzung* Wegpunkte[15] = { NULL };
Strecke* Wegstrecken[15] = { NULL };
Fahrkommando Richtungsvorgaben[15] = { LEER };
int aktuelleAusrichtung;

bool initStrecke(Strecke *strecke, Kreuzung * kreuzungNO, Kreuzung* kreuzungSW)  
{
	strecke->nextKreuzungNO = kreuzungNO;
	strecke->nextKreuzungSW = kreuzungSW;
	return 1;
}

bool initKreuzung(Kreuzung *kreuzung, Strecke *streckeN, Strecke *streckeO, Strecke *streckeS, Strecke *streckeW, char Name)   // todo Koordinaten und Mission
{
	kreuzung->nextStreckeN = streckeN;
	kreuzung->nextStreckeO = streckeO;
	kreuzung->nextStreckeS = streckeS;
	kreuzung->nextStreckeW = streckeW;
	kreuzung->Name = Name;
	return 1;
}


bool initSpielfeld(Spielfeld *feld) {

	initKreuzung(&feld->k_a, &feld->s_ab, NULL, NULL, NULL, 'a');
	initKreuzung(&feld->k_b, &feld->s_bd, NULL, &feld->s_bc, &feld->s_ab, 'b');
	initKreuzung(&feld->k_c, &feld->s_bc, NULL, NULL, NULL, 'c');
	initKreuzung(&feld->k_d, &feld->s_dg, &feld->s_df, &feld->s_bd, &feld->s_de, 'd');
	initKreuzung(&feld->k_e, NULL, &feld->s_de, NULL, NULL, 'e');
	initKreuzung(&feld->k_f, NULL, NULL, NULL, &feld->s_df, 'f');
	initKreuzung(&feld->k_g, NULL, &feld->s_gi, &feld->s_dg, &feld->s_gh, 'g');
	initKreuzung(&feld->k_h, NULL, &feld->s_gh, NULL, NULL, 'h');
	initKreuzung(&feld->k_i, &feld->s_ij, &feld->s_ik, NULL, &feld->s_gi, 'i');
	initKreuzung(&feld->k_j, NULL, NULL, &feld->s_ij, NULL, 'j');
	initKreuzung(&feld->k_k, &feld->s_kl, &feld->s_kn, &feld->s_km, &feld->s_ik, 'k');
	initKreuzung(&feld->k_l, NULL, NULL, &feld->s_kl, NULL, 'l');
	initKreuzung(&feld->k_m, &feld->s_km, NULL, NULL, NULL, 'm');
	initKreuzung(&feld->k_n, &feld->s_no, &feld->s_np, NULL, &feld->s_kn, 'n');
	initKreuzung(&feld->k_o, NULL, NULL, &feld->s_no, NULL, 'o');
	initKreuzung(&feld->k_p, NULL, &feld->s_pq, &feld->s_pr, &feld->s_np, 'p');
	initKreuzung(&feld->k_q, NULL, NULL, NULL, &feld->s_pq, 'q');
	initKreuzung(&feld->k_r, &feld->s_pr, &feld->s_rt, &feld->s_ru, &feld->s_rs, 'r');
	initKreuzung(&feld->k_s, NULL, &feld->s_rs, NULL, NULL, 's');
	initKreuzung(&feld->k_t, NULL, NULL, NULL, &feld->s_rt, 't');
	initKreuzung(&feld->k_u, &feld->s_ru, &feld->s_uw, &feld->s_uv, NULL, 'u');
	initKreuzung(&feld->k_v, &feld->s_uv, NULL, NULL, NULL, 'v');
	initKreuzung(&feld->k_w, &feld->s_uw, NULL, NULL, NULL, 'w');
	// .............

	initStrecke(&feld->s_ab, &feld->k_b, &feld->k_a);
	initStrecke(&feld->s_bc, &feld->k_b, &feld->k_c);
	initStrecke(&feld->s_bd, &feld->k_d, &feld->k_b);
	initStrecke(&feld->s_de, &feld->k_d, &feld->k_e);
	initStrecke(&feld->s_df, &feld->k_f, &feld->k_d);
	initStrecke(&feld->s_dg, &feld->k_g, &feld->k_d);
	initStrecke(&feld->s_gh, &feld->k_g, &feld->k_h);
	initStrecke(&feld->s_gi, &feld->k_i, &feld->k_g);
	initStrecke(&feld->s_ij, &feld->k_j, &feld->k_i);
	initStrecke(&feld->s_ik, &feld->k_k, &feld->k_i);
	initStrecke(&feld->s_kl, &feld->k_l, &feld->k_k);
	initStrecke(&feld->s_km, &feld->k_k, &feld->k_m);
	initStrecke(&feld->s_kn, &feld->k_n, &feld->k_k);
	initStrecke(&feld->s_no, &feld->k_o, &feld->k_n);
	initStrecke(&feld->s_np, &feld->k_p, &feld->k_n);
	initStrecke(&feld->s_pq, &feld->k_q, &feld->k_p);
	initStrecke(&feld->s_pr, &feld->k_p, &feld->k_r);
	initStrecke(&feld->s_rs, &feld->k_r, &feld->k_s);
	initStrecke(&feld->s_rt, &feld->k_t, &feld->k_r);
	initStrecke(&feld->s_ru, &feld->k_r, &feld->k_u);
	initStrecke(&feld->s_uv, &feld->k_u, &feld->k_v);
	initStrecke(&feld->s_uw, &feld->k_w, &feld->k_u);
	// ..............

	

	return 1;

}

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


void main(void)
{
	initSpielfeld(&feld);
	initSpielMissionen(&Spiel);

	if (Spielfarbe == GRUEN)
	{
		feld.AktuelleKreuzung = &feld.k_h;
		aktuelleAusrichtung = OST;
	}
	else
	{
		feld.AktuelleKreuzung = &feld.k_q;
		aktuelleAusrichtung = WEST;
	}

	int n = 0;
	while (1)
	{
		CarryOutMission();
		//feld.AktuelleKreuzung = Spiel.AktiveMission->Missionskreuzung;
		n++;
		if (n == 20)
		{
			feld.AktuelleKreuzung = Spiel.AktiveMission->Missionskreuzung;
			aktuelleAusrichtung = SUED;
		}
		if(n==40)
		{
			feld.AktuelleKreuzung = Spiel.AktiveMission->Missionskreuzung;
		}
	}





	/*if (Wegfindung(NULL, feld.AktuelleKreuzung, &feld.k_w, Wegpunkte, Wegstrecke))
	{
		generiereRichtungswechsel(Wegpunkte, Wegstrecke, Richtungsvorgaben, aktuelleAusrichtung);
		printf("Weg gefunden\n");
		Spiel.BieneGruen.MissionFunktion();
		
	}
	else 
	{
		printf("Kein Weg gefunden");
	}
	if (Wegfindung(NULL, &feld.k_h, &feld.k_f, Wegpunkte, Wegstrecke))
	{
		generiereRichtungswechsel(Wegpunkte, Wegstrecke, Richtungsvorgaben, aktuelleAusrichtung);
		printf("Weg gefunden\n");
	}
	else
	{
		printf("Kein Weg gefunden");
	}*/
}