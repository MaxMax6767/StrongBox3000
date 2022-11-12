//####################################
//###### Def Variables Globales ######
//####################################

//Liste des lettres d'agent (Constante)
static char Nom[16] = {'A', 'B', 'C', 'D', 'E', 'F', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q'};

char nomAgent;

String authFailed = "Authentification non reussie"; //String authentification ratée
String authSuccess = "Authentification reussie ! \n"; //String authentification réussie

String authPrompt = "Veuillez entrer votre nom d'agent : "; //String entrer nom d'agent


//###############
//##### MA1 #####
//###############

bool MA1() {
    int juste = 0;

    // Def Questions & Réponses
    const String Q[3][4] = {{"Si 1+1=3, 1+2=?",                                "1",                                      "4",                                                "69"},
                            {"Quel est notre methode d'assassinat preferee ?", "Tireur d'elite",                         "Paves explosifs",                                  "Missile Balistique Intercontinental"},
                            {"Quel est notre slogan?",                         "Tuer d'abord, poser les question apres", "On espionne tout le monde, c'est plus egalitaire", "On en a pas, on est un service secret"}};
    int a[3] = {2, 3, 3};

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

        if (Serial.parseInt() == a[i]) {
            ++juste;
        }
        for (int j = 0; j < 10; ++j) {
            Serial.println("");
        }
    }
    if (juste == 3) {
        Serial.println(authSuccess);
        return true;
    }
    Serial.println(authFailed);
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
        if (a == 0 or n == 1){
            r = 0;
        } else {
            r = 1;
            while (e > 0){
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
    static int publicKeys[16] = {601, 619, 631, 641, 647, 653, 661, 673, 691, 701, 733, 739, 751, 797, 809, 811};
    int i = 0;
    for (i = 0; i < 16; i++) {
        if (Nom[i] == x) {
            return publicKeys[i];
        }
    }
    Serial.println("Erreur: Nom d'agent non reconnu");
    return -1;
}

bool MA2() {
    int Mp;
    int M = random(2281);
    Serial.read();
    Serial.println(authPrompt);
    while (Serial.available() == 0) {
        // attente que la console série soit utilisée
    }
    char temp = Serial.read();
    if (nomAgent != NULL) {
        if (nomAgent != temp) {
            return false;
        }
    } else {
        nomAgent = temp;
    }
    int essais = 3; //Nombre d'essais restants

    if (cleAgent(nomAgent) == -1) {
        return false;
    }
    int C = chiffrer(M, cleAgent(nomAgent));
    Serial.println("Message a dechiffrer : ");
    Serial.println(C);

    while (essais > 0) {
        if (Serial.available() > 0) {
            Mp = Serial.parseInt();
            if (M == Mp) {
                Serial.println(authSuccess);
                return true;
            } else {
                Serial.println("\nValeur incorrecte !\n");
                essais--;
                if (essais == 0) {
                    Serial.println(authFailed);
                    return false;
                }
                Serial.print("Il vous reste ");
                Serial.print(essais);
                Serial.println(" essais\n");
            }
        }
    }
    Serial.println(authFailed);
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
    if (Serial.read() == 75) {
        Serial.println(authSuccess);
        return true;
    }
    Serial.println(authFailed);
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
    if (Serial.read() == 75) {
        Serial.println(authSuccess);
        return true;
    }
    Serial.println(authFailed);
    return false;
}

//###############
//##### MA5 #####
//###############

bool MA5() {
    int cardId[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

    Serial.read();
    Serial.println(authPrompt);
    while (Serial.available() == 0) {
        // attente que la console série soit utilisée
    }
    char temp = Serial.read();
    if (nomAgent != NULL) {
        if (nomAgent != temp) {
            Serial.println(authFailed);
            return false;
        }
    } else {
        nomAgent = temp;
    }

    Serial.println("Merci de renseingner votre CardID");
    while (Serial.available() == 0) {
        // attente que la console série soit utilisée
    }
    int cId = Serial.parseInt();
    for (int i = 0; i < 16; i++) {
        if (Nom[i] == nomAgent) {
            if (cardId[i] == cId) {
                Serial.println(authSuccess);
                return true;
            }
        }
    }
    Serial.println(authFailed);
    return false;
}

int authModele() {
    int val = analogRead(A0);
    while (!(176 <= val && val <= 198 || 306 <= val && val <= 328 || 388 <= val && val <= 410 ||
             470 <= val && val <= 492 || 513 <= val && val <= 535 || 550 <= val && val <= 572 ||
             574 <= val && val <= 596 || 636 <= val && val <= 658)) {
        val = analogRead(A0);
    }
    if (176 <= val && val <= 198) {
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

bool secu(int modele) {
    if (modele == 1) {
        return ((MA1() && MA3()) == true);
    } else if (modele == 2) {
        return ((MA1() && MA4()) == true);
    } else if (modele == 3 || modele == 4) {
        return ((MA2() && MA5()) == true);
    } else if (modele == 5 || modele == 6 || modele == 7) {
        return ((MA2() && MA3() && MA5()) == true);
    } else if (modele == 8) {
        return ((MA1() && MA2() && MA3() && MA5()) == true);
    } else {
        return false;
    }
}

void setup() {
    pinMode(A0, INPUT);
    Serial.begin(9600);
    Serial.println("StrongBox 3000\n");
}

void loop() {

    if ((secu(authModele())) == true) {
        Serial.println("Bienvenue");
    } else {
        Serial.println("Acces refuse");
    }

}