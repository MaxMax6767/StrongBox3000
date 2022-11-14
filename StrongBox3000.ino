// Projet StrongBox 3000 Groupe 6 | 2022 | Emma HODZIC, Maxime GUG, Kellian BECHTEL, Nicolas KANIAN

//####################################
//###### Def Variables Globales ######
//####################################

// Etat du coffre
bool open = false;
bool failed = false;

// Initialisation du code
int code = 0;

// Initialisation de l'état des boutons
int button1State = 0;
int button2State = 0;
int button3State = 0;
int button4State = 0;

//Liste des lettres d'agent (Constante)
static char Nom[16] = {'A', 'B', 'C', 'D', 'E', 'F', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q'};

//Nom de l'agent, utilisé par les fonctions MA2 et MA5
char nomAgent;

// Variables de phrases utilisées dans plusieurs fonctions
String authPrompt = "Veuillez entrer votre nom d'agent : "; // Phrase de demande d'authentification

//###############
//##### LED #####
//###############

void authFailed() {
    Serial.println("Authentification non reussie\n");  // Phrase authentification ratée
    for (int i = 0; i < 3; i++) {
        digitalWrite(11, HIGH);
        delay(500);
        digitalWrite(11, LOW);
        delay(500);
    }
}

void authSuccess() {
    Serial.println("Authentification reussie\n");  // Phrase authentification réussie
    for (int i = 0; i < 3; i++) {
        digitalWrite(12, HIGH);
        delay(500);
        digitalWrite(12, LOW);
        delay(500);
    }
}

void ErrorState() {
    // Blink 3x rouge
    for (int counter = 0; counter < 3; ++counter) {
        digitalWrite(7, HIGH);
        digitalWrite(6, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(4, HIGH);
        delay(500);
        digitalWrite(7, LOW);
        digitalWrite(6, LOW);
        digitalWrite(5, LOW);
        digitalWrite(4, LOW);
        delay(500);
    }
    code = 0;
}

//###############
//##### MA1 #####
//###############

bool MA1() {
    int juste = 0;  // Compteur de réponses justes (0 à 3)

    // Def Questions & Réponses (Constante)
    const String Q[3][4] = {{"Quel est le point cardinal manquant ? Nord / Est / Ouest ", "Nord", "Ouest", "Sud"},
                            {"De quelle valeur se rapproche le nombre π ?",               "11", "25",  "3,14"},
                            {"De quelle couleur est le ciel ?",                           "Bleu", "Vert",  "Jaune"}};
    // Def Réponses Justes (Constante)
    const int a[3] = {3, 3, 1};

    Serial.println("Repondez aux questions afin de reussir l'authentification");

    for (int i = 0; i < 3; i++) {  // Affiche la question
        Serial.println(Q[i][0]);
        for (int j = 1; j < 4; j++) {  // Affiche les réponses
            Serial.print(j);
            Serial.print(" : ");
            Serial.println(Q[i][j]);
        }

        Serial.println("Votre reponse: ");
        while (Serial.available() == 0) {
            // attente que la console série soit utilisée
        }

        if (Serial.parseInt() == a[i]) {  // Si la response est juste, on incrémente juste
            ++juste;
        }
        for (int j = 0; j < 3; ++j) {  // On sépare les questions
            Serial.println("");
        }
    }

    if (juste == 3) {  // Si toutes les réponses sont justes, on renvoie true
        authSuccess();
        return true;
    }
    // Sinon, on renvoie false
    authFailed();
    return false;
}

//###############
//##### MA2 #####
//###############

//Declare module function
int Modexp(int a, int e, int n) {
    long r;
    if (e < 0) {
        Serial.println("Unhandled Case");
        return -1;
    } else {
        if (a == 0 or n == 1) {
            r = 0;
        } else {
            r = 1;
            while (e > 0) {
                r = (r * (a % n)) % n;
                --e;
            }
        }
    }
    return r;
}

//Declare encryption function
int chiffrer(int M, int e) {
    int n = 43 * 67;
    return Modexp(M, e, n);
}

int cleAgent(char x) {
    // Def Cle publique Agent (Constante)
    static int publicKeys[16] = {601, 619, 631, 641, 647, 653, 661, 673, 691, 701, 733, 739, 751, 797, 809, 811};

    for (int i = 0; i < 16; i++) {
        if (Nom[i] == x) {
            return publicKeys[i];
        }
    }
    Serial.println("Erreur: Nom d'agent non reconnu");
    return -1;
}

bool MA2() {
    int essais = 3; //Nombre d'essais restants
    randomSeed(analogRead(A0));
    int M = random(2281);

    Serial.println(authPrompt);  // Demande le nom de l'agent
    while (Serial.available() == 0) {
        // attente que la console série soit utilisée
    }
    char temp = Serial.read();  // Lit le nom de l'agent
    if (nomAgent != NULL) {  // Si le nom de l'agent est déjà connu, on vérifie qu'il est le même
        if (nomAgent != temp) {  // Si le nom de l'agent est différent, on renvoie false
            authFailed();
            return false;
        }
    } else {  // Si le nom de l'agent n'est pas connu, on le stocke
        nomAgent = temp;
    }

    if (cleAgent(nomAgent) == -1) {  // Si le nom de l'agent n'est pas reconnu, on renvoie false
        authFailed();
        return false;
    }

    int C = chiffrer(M, cleAgent(nomAgent));  // On chiffre M avec la clé publique de l'agent
    Serial.print("Message a dechiffrer : ");
    Serial.println(C);

    while (essais > 0) {  // Tant qu'il reste des essais, on demande la réponse
        if (Serial.available() > 0) {
            int Mp = Serial.parseInt();
            if (M == Mp) {  // Si la clé originale correspond à celle calculée avec la clé privée, on renvoie true
                authSuccess();
                return true;
            } else {  // Sinon, on décrémente le nombre d'essais restants et on affiche un message d'erreur & le nombre d'essais restants
                Serial.println("\nValeur incorrecte !\n");
                essais--;
                if (essais == 0) {
                    authFailed();
                    return false;
                }
                Serial.print("Il vous reste ");
                Serial.print(essais);
                Serial.println(" essais\n");
            }
        }
    }
    authFailed();
    return false;
}

//###############
//##### MA3 #####
//###############

bool MA3() {
    Serial.println("Scan retinien en cours, merci d'entrer K");
    while (Serial.available() == 0) {
        // attente que la console série soit utilisée
    }
    if (Serial.read() == 75) {  // Si K est entré, on renvoie true
        authSuccess();
        return true;
    }
    // Sinon, on renvoie false
    authFailed();
    return false;
}

//###############
//##### MA4 #####
//###############

bool MA4() {
    Serial.println("Scan digital en cours, merci d'entrer K");
    while (Serial.available() == 0) {
        // attente que la console série soit utilisée
    }
    if (Serial.read() == 75) {  // Si K est entré, on renvoie true
        authSuccess();
        return true;
    }
    // Sinon, on renvoie false
    authFailed();
    return false;
}

//###############
//##### MA5 #####
//###############

bool MA5() {
    // Déclaration des numéros de carte (Constante)
    const int cardId[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

    Serial.println(authPrompt);  // Demande d'identification
    while (Serial.available() == 0) {
        // attente que la console série soit utilisée
    }
    char temp = Serial.read();  // Lit l'identification et le stocke temporairement
    if (nomAgent != NULL) {  // Si le nom de l'agent est déjà connu, on vérifie qu'il est le même
        if (nomAgent != temp) {  // Si le nom de l'agent est différent, on renvoie false
            authFailed();
            return false;
        }
    } else {  // Si le nom de l'agent n'est pas connu, on le stocke
        nomAgent = temp;
    }

    Serial.println("Merci de renseigner votre CardID");  // Demande du numéro de carte
    while (Serial.available() == 0) {
        // attente que la console série soit utilisée
    }
    int cId = Serial.parseInt();  // Lit le numéro de carte et le stocke temporairement

    for (int i = 0; i < 16; i++) {
        if (Nom[i] == nomAgent) {  // Si le nom de l'agent est reconnu, on vérifie que le numéro de carte correspond
            if (cardId[i] == cId) {  // Si le numéro de carte correspond, on renvoie true
                authSuccess();
                return true;
            }
        }
    }
    // Sinon, on renvoie false
    authFailed();
    return false;
}

int authModele() {
    int val = analogRead(A0);  // Lecture de la valeur de l'entrée analogique A0
    while (true) {  // Tant que la valeur n'est pas comprise entre les bornes, on relit la valeur
        val = analogRead(A0);
        if (176 <= val && val <= 198) {  // Si la valeur est comprise entre les bornes, on renvoie le numéro du modèle
            return 1;
        } else if (306 <= val && val <= 328) {
            return 2;
        } else if (388 <= val && val <= 410) {
            return 3;
        } else if (470 <= val && val <= 492) {
            return 4;
        } else if (513 <= val && val <= 535) {
            return 5;
        } else if (550 <= val && val <= 572) {
            return 6;
        } else if (574 <= val && val <= 596) {
            return 7;
        } else if (636 <= val && val <= 658) {
            return 8;
        }
    }
}

bool secu(int modele) {
    if (modele == 1) {  // Si le modèle est 1, on lance la fonction MA1 et MA3 et on renvoie si les 2 sont vraies ou non.
        return ((MA1() && MA3()) == true);
    } else if (modele == 2) {  // Si le modèle est 2, on lance la fonction MA1 et MA4 et on renvoie si les 2 sont vraies ou non.
        return ((MA1() && MA4()) == true);
    } else if (modele == 3 || modele == 4) {  // Si le modèle est 3 ou 4, on lance la fonction MA2 et MA5 et on renvoie si les 2 sont vraies ou non.
        return ((MA2() && MA5()) == true);
    } else if (modele == 5 || modele == 6 || modele == 7) {  // Si le modèle est 5, 6 ou 7, on lance la fonction MA2, MA3 et MA5 et on renvoie si les 3 sont vraies ou non.
        return ((MA2() && MA3() && MA5()) == true);
    } else if (modele == 8) {  // Si le modèle est 8, on lance la fonction MA1, MA2, MA3 et MA5 et on renvoie si les 4 sont vraies ou non.
        return ((MA1() && MA2() && MA3() && MA5()) == true);
    } else {  // Si le modèle n'est pas reconnu, on renvoie false
        authFailed();
        return false;
    }
}

void setup() {
    pinMode(A0, INPUT);  // Définition de l'entrée analogique A0 en entrée

    pinMode(13, OUTPUT);  // Sortie LED Verte
    pinMode(12, OUTPUT);  // Sortie LED Jaune
    pinMode(11, OUTPUT);  // Sortie LED Orange
    pinMode(10, OUTPUT);  // Sortie LED Rouge

    pinMode(8, OUTPUT);  // Sortie LED Verte
    pinMode(7, OUTPUT);  // Sortie LED Rouge
    pinMode(6, OUTPUT);  // Sortie LED Rouge
    pinMode(5, OUTPUT);  // Sortie LED Rouge
    pinMode(4, OUTPUT);  // Sortie LED Rouge

    pinMode(9, INPUT);  // Entrée du Bouton RESET

    pinMode(A4, INPUT);  // Entrée du Bouton code N°4
    pinMode(A3, INPUT);  // Entrée du Bouton code N°3
    pinMode(A2, INPUT);  // Entrée du Bouton code N°2
    pinMode(A1, INPUT);  // Entrée du Bouton code N°1

    Serial.begin(9600);  // Définition de la vitesse de la console série à 9600 bauds

    Serial.println("StrongBox 3000\n");  // Affichage du nom du projet
}

void loop() {
    // Update de l'état des boutons
    button1State = analogRead(A1);
    button2State = analogRead(A2);
    button3State = analogRead(A3);
    button4State = analogRead(A4);

    // Si un bouton est appuyé, on lance la fonction
    if (button1State != 0 or button2State != 0 or button3State != 0 or button4State != 0) {
        if (code == 3) {
            // Si bouton 4 pressé
            if (button1State == 0 and button2State == 0 and button3State == 0 and button4State != 0) {
                digitalWrite(7, HIGH);  // Allumage LED Rouge 4
                digitalWrite(8, HIGH);  // Allumage LED Verte
                code += 1;
                delay(100);
            } else {
                ErrorState();
            }
        } else if (code == 2) {
            // Si bouton 3 pressé
            if (button1State == 0 and button2State == 0 and button3State != 0 and button4State == 0) {
                digitalWrite(6, HIGH);  // Allumage LED Rouge 3
                code += 1;
                delay(100);
            } else {
                ErrorState();
            }
        } else if (code == 1) {
            // Si bouton 2 pressé
            if (button1State == 0 and button2State != 0 and button3State == 0 and button4State == 0) {
                digitalWrite(5, HIGH);  // Allumage LED Rouge 2
                code += 1;
                delay(100);
            } else {
                ErrorState();
            }
        } else if (code == 0) {
            // Si bouton 1 pressé
            if (button1State != 0 and button2State == 0 and button3State == 0 and button4State == 0) {  // Case 1 Valid
                digitalWrite(4, HIGH);  // Allumage LED Rouge 1
                code += 1;
                delay(100);
            } else {
                ErrorState();
            }
        }
    } else if (digitalRead(9) == HIGH) {  // Si le bouton RESET est appuyé, on réinitialise les variables
        nomAgent = NULL;
        open = false;
        failed = false;
        code = 0;
        digitalWrite(4, LOW);  // Extinction LED Rouge 1
        digitalWrite(5, LOW);  // Extinction LED Rouge 2
        digitalWrite(6, LOW);  // Extinction LED Rouge 3
        digitalWrite(7, LOW);  // Extinction LED Rouge 4
        digitalWrite(8, LOW);  // Extinction LED Verte
        Serial.println("RESET\n");
        delay(1000);
    } else if (open == true) {
        digitalWrite(13, HIGH);  // Allume la LED verte
        delay(500);
        digitalWrite(13, LOW);  // Eteint la LED verte
        delay(500);
    } else if (failed == true) {
        digitalWrite(10, HIGH);  // Allume la LED rouge
        delay(500);
        digitalWrite(10, LOW);  // Eteint la LED rouge
        delay(500);
    } else if (code == 4) {  // Si la boite n'est pas ouverte, on lance la fonction de sécurité
        if ((secu(authModele())) == true) {
            Serial.println("Bienvenue\n");
            open = true;
        } else {
            Serial.println("Acces refuse\n");
            failed = true;
        }
        delay(100);
    }
    delay(100);
}
