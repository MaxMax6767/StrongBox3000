//Liste des lettres d'agent
char Agents[16] = {'A', 'B', 'C', 'D', 'E', 'F', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q'};

//Agent variables
char lettreAgent; //Lettre d'identification envoyé par l'agent
int numAgent; //Numéro de l'agent, dérivé de la lettre
bool agentValide = false; //Es-ce que la lettre d'agent est valide?

//Bool authentification
bool authentifie = false;

//Bool disable, activé si authentification ratee
bool disable = false;

bool secLevel[5][5] = {{true, false, true, false, false}, //Sec Level 1
                       {true, false, false, true, false}, //Sec Level 2
                       {false, true, false, false, true}, //Sec Level 3
                       {false, true, true, true, false}, //Sec Level 4
                       {true, true, true, false, true}}; //Sec Level 5


int secValue = 3; //Valeur de sécurité de l'agent

String launchOf = "Lancement de "; //String lancement de
String authFailed = "Authentification ratee, vous aveu echoue "; //String authentification ratée


//############################################
//####### MA1 Methode d'identification #######
//############################################


bool MA1(int numAgent) {
    String Q[3][4] = {{"Si 1+1=3, 1+2=?", "1", "4", "69"},
                      {"Quel est notre methode d'assasination preferee?", "Tireur d'elite", "Paves explosifs", "Missile Balistique Intercontinental"},
                      {"Quel est notre slogan?", "Tuer d'abord, poser les question apres", "On espionne tout le monde, c'est plus egalitaire", "On en a pas, on est un service secret"}};

    int a[3] = {2, 3, 3};

    Serial.println("Repondez aux questions afin de reussir l'authentification");


    for (int i = 0; i < 3; i++) {
        Serial.println(Q[i][0]);

        for (int j = 1; j < 4; j++) {
            Serial.print(j);
            Serial.print(" : ");
            Serial.println(Q[i][j]);
        }

        Serial.println("Votre reponse: ");

        while (Serial.available() == 0){

        }
        if (Serial.available() > 0) {
            int reponse = Serial.parseInt();
            if (reponse == a[i]) {
                Serial.println("Reponse correcte");
                for (int j = 0; j < 10; ++j) {
                    Serial.println("");
                }
            }
            else {
                Serial.println("Reponse incorrecte");
                return false;
            }
        }
    }
    return true;
}

//############################################
//####### MA2 Methode d'identification #######
//############################################

//Declare module function
int Modexp(int a, int e, int n) {
    long r;
    if (e < 0) {
        Serial.println("Unhandled Case");
        return -1;
    }
    else {
        if (a == 0 or n == 1){
            r = 0;
        }
        else {
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
int chiffrer (int M, int e) {
    int n = 43*67;
    return Modexp(M, e, n);
}

bool MA2(int numAgent) {

    randomSeed(analogRead(A0)); //Seed random number generator

    int cleAgent; //Clé publique de l'agent
    int M = random(2281); //Message non crypté
    int Mp; //Proposition de message décrypté, envoyé par l'utilisateur
    int C; //Message crypté

    int essais = 3; //Nombre d'essais restants

    //Clés publiques
    int publicKeys[16] = {601, 619, 631, 641, 647, 653, 661, 673, 691, 701, 733, 739, 751, 797, 809, 811};

    //Clés privés
    int privateKeys[16] = {1273, 103, 883, 653, 467, 641, 1321, 589, 1039, 953, 745, 919, 1159, 713, 233, 1063};

    cleAgent = publicKeys[numAgent];
    C = chiffrer(M, cleAgent);

    Serial.print("(Message decrypte: ");
    Serial.print(M);
    Serial.println(")");

    Serial.println("Veuillez decrypter le code suivant avec votre cle privee: ");
    Serial.println(C);

    while (essais > 0) {
        if (Serial.available() > 0) {
            Mp = Serial.parseInt(SKIP_ALL);
            if (M == Mp) {
                Serial.println("Authentification reussie!");
                return true;
            }
            else {
                Serial.println("\nValeur incorrecte!\n");
                essais--;
                if (essais == 0) {
                    Serial.println("Vous avez epuise vos essais!\n");
                    return false;
                }
                Serial.print("Il vous reste ");
                Serial.print(essais);
                Serial.println(" essais\n");
            }
        }
    }
    return false;
}

//############################################
//####### MA3 Methode d'identification #######
//############################################

bool MA3(int numAgent) {
    Serial.println("Scan retinien en cours...\n");
    Serial.println("Scan retinien reussi!\n");
    return true;
}

//############################################
//####### MA4 Methode d'identification #######
//############################################

bool MA4(int numAgent) {
    Serial.println("Scan digital en cours...\n");
    Serial.println("Scan digital reussi!\n");
    return true;
}

//############################################
//####### MA5 Methode d'identification #######
//############################################

bool MA5(int numAgent) {
    int pw[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

    Serial.read();
    Serial.println("Veuillez entrer votre mot de passe: ");


    while (Serial.available() == 0) {
        if (Serial.available() > 0) {
            int reponse = Serial.parseInt(SKIP_ALL);
            if (reponse == pw[numAgent]) {
                Serial.println("Mot de passe juste!");
                return true;
            }
            else {
                Serial.println("Mot de passe incorrect!");
                return false;
            }
        }
    }
}

void setup() {
    Serial.begin(9600);
    Serial.println("Identifiez vous: \n");
}

void loop() {
    if (authentifie == false && disable == false) {
        if (agentValide == false) {
            if (Serial.available() > 0) {
                lettreAgent = Serial.read();

                //Look for input letter in Agents array
                for (int i = 0; i <= sizeof(Agents); ++i) {
                    if (lettreAgent == Agents[i]) {
                        numAgent = i;
                        agentValide = true;
                        break;
                    }
                }

                if (agentValide == false) {
                    Serial.println("Lettre d'agent invalide!");
                }
            }
        }

        else {
            if (secLevel[secValue][1] == true){
                Serial.print(launchOf);
                Serial.println("MA1 \n");
                if (MA1(numAgent) == false){

                    Serial.print(authFailed);
                    Serial.println("MA1!");

                    disable = true;
                    return;
                }
            }
            if (secLevel[secValue][2] == true){
                Serial.print(launchOf);
                Serial.println("MA2 \n");
                if (MA2(numAgent) == false){

                    Serial.print(authFailed);
                    Serial.println("MA2!");

                    disable = true;
                    return;
                }
            }
            if (secLevel[secValue][3] == true){
                Serial.print(launchOf);
                Serial.println("MA3 \n");
                if (MA3(numAgent) == false){

                    Serial.print(authFailed);
                    Serial.println("MA3!");

                    disable = true;
                    return;
                }
            }
            if (secLevel[secValue][4] == true){
                Serial.print(launchOf);
                Serial.println("MA4 \n");
                if (MA4(numAgent) == false){

                    Serial.print(authFailed);
                    Serial.println("MA4!");

                    disable = true;
                    return;
                }
            }
            if (secLevel[secValue][5] == true){
                Serial.print(launchOf);
                Serial.println("MA5 \n");
                if (MA5(numAgent) == false){

                    Serial.print(authFailed);
                    Serial.println("MA5!");

                    disable = true;
                    return;
                }
            }
            authentifie = true;
            Serial.println("Vous etes authentifie!");
        }
    }
}