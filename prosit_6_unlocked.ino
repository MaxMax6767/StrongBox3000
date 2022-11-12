//####################################
//###### Def Variables Globales ######
//####################################

//Liste des lettres d'agent (Constante)
static char Nom[16] = {'A', 'B', 'C', 'D', 'E', 'F', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q'};

//Nom de l'agent, utilisé par les fonctions MA2 et MA5
char nomAgent;

// Variables de phrases utilisées dans plusieurs fonctions
String authFailed = "Authentification non reussie"; //Phrase authentification ratée
String authSuccess = "Authentification reussie ! \n"; //Phrase authentification réussie
String authPrompt = "Veuillez entrer votre nom d'agent : "; //Phrase de demande d'authentification

//###############
//##### MA1 #####
//###############

bool MA1() {
    int juste = 0;  // Compteur de réponses justes (0 à 3)

    // Def Questions & Réponses (Constante)
    const String Q[3][4] = {{"Si 1+1=3, 1+2=?",                                "1",                                      "4",                                                "69"},
                            {"Quel est notre methode d'assassinat preferee ?", "Tireur d'elite",                         "Paves explosifs",                                  "Missile Balistique Intercontinental"},
                            {"Quel est notre slogan?",                         "Tuer d'abord, poser les question apres", "On espionne tout le monde, c'est plus egalitaire", "On en a pas, on est un service secret"}};
    // Def Réponses Justes (Constante)
    const int a[3] = {2, 3, 3};

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
        Serial.println(authSuccess);
        return true;
    }
    // Sinon, on renvoie false
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
    // Def Cle publique Agent (Constante)
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
    int essais = 3; //Nombre d'essais restants
    int M = random(2281);

    Serial.println(authPrompt);  // Demande le nom de l'agent
    while (Serial.available() == 0) {
        // attente que la console série soit utilisée
    }
    char temp = Serial.read();  // Lit le nom de l'agent
    if (nomAgent != NULL) {  // Si le nom de l'agent est déjà connu, on vérifie qu'il est le même
        if (nomAgent != temp) {  // Si le nom de l'agent est différent, on renvoie false
            return false;
        }
    } else {  // Si le nom de l'agent n'est pas connu, on le stocke
        nomAgent = temp;
    }

    if (cleAgent(nomAgent) == -1) {  // Si le nom de l'agent n'est pas reconnu, on renvoie false
        return false;
    }

    int C = chiffrer(M, cleAgent(nomAgent));  // On chiffre M avec la clé publique de l'agent
    Serial.print("Message a dechiffrer : ");
    Serial.println(C);

    while (essais > 0) {  // Tant qu'il reste des essais, on demande la réponse
        if (Serial.available() > 0) {
            int Mp = Serial.parseInt();
            if (M == Mp) {  // Si la clé originale correspond à celle calculée avec la clé privée, on renvoie true
                Serial.println(authSuccess);
                return true;
            } else {  // Sinon, on décrémente le nombre d'essais restants et on affiche un message d'erreur & le nombre d'essais restants
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
    if (Serial.read() == 75) {  // Si K est entré, on renvoie true
        Serial.println(authSuccess);
        return true;
    }
    // Sinon, on renvoie false
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
    if (Serial.read() == 75) {  // Si K est entré, on renvoie true
        Serial.println(authSuccess);
        return true;
    }
    // Sinon, on renvoie false
    Serial.println(authFailed);
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
            Serial.println(authFailed);
            return false;
        }
    } else {  // Si le nom de l'agent n'est pas connu, on le stocke
        nomAgent = temp;
    }

    Serial.println("Merci de renseingner votre CardID");  // Demande du numéro de carte
    while (Serial.available() == 0) {
        // attente que la console série soit utilisée
    }
    int cId = Serial.parseInt();  // Lit le numéro de carte et le stocke temporairement
    int i = 0;  // Initialisation de l'index
    for (int i = 0; i < 16; i++) {
        if (Nom[i] == nomAgent) {  // Si le nom de l'agent est reconnu, on vérifie que le numéro de carte correspond
            if (cardId[i] == cId) {  // Si le numéro de carte correspond, on renvoie true
                Serial.println(authSuccess);
                return true;
            }
        }
    }
    // Sinon, on renvoie false
    Serial.println(authFailed);
    return false;
}

int authModele() {
    int val = analogRead(A0);  // Lecture de la valeur de l'entrée analogique A0
    while (!(176 <= val && val <= 198 || 306 <= val && val <= 328 ||
             388 <= val && val <= 410 || 470 <= val && val <= 492 ||
             513 <= val && val <= 535 || 550 <= val && val <= 572 ||
             574 <= val && val <= 596 || 636 <= val && val <= 658)) {  // Tant que la valeur n'est pas comprise entre les bornes, on relit la valeur
        val = analogRead(A0);
    }
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
        return false;
    }
}

void setup() {
    pinMode(A0, INPUT);  // Définition de l'entrée analogique A0 en entrée
    Serial.begin(9600);  // Définition de la vitesse de la console série à 9600 bauds
    Serial.println("StrongBox 3000\n");  // Affichage du nom du projet
}

void loop() {

    if ((secu(authModele())) == true) {
        Serial.println("Bienvenue");
    } else {
        Serial.println("Acces refuse");
    }

}