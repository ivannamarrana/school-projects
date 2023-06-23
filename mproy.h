#ifndef MPROY_H_INCLUDED
#define MPROY_H_INCLUDED

#define NEG 1
#define CONJ 2
#define DISY 3
#define IMPL 4
#define DIMPL 5

typedef struct {
    int size;
    char *cVec;
}   CharVec;

typedef struct {
    int size;
    int *iVec;
}   IntVec;

typedef struct {
    int size;
    int nombre;
    int conector;
    int *truthValues;
    int prop1, indxP1;
    int prop2, indxP2;
    int evaluado;
}   Prop;

typedef struct {
    int size;
    Prop* props;
}   VectorProp;

void createCharVec(CharVec *cv, int size){
    cv->size = size;
    cv->cVec = (char *) malloc(cv->size * sizeof(char));
    if(!cv->cVec){
        printf("\nERROR NO HAY MEMORIA\n");
        cv->size = 0;
        return;
    }
    return;
}

void freeCharVec(CharVec *cv){
    free(cv->cVec);
    return;
}

int cuentaCuantasLetrasHay(char *prop){
    int contador = 0;
    int size = (int) strlen(prop);
    for(int i = 0; i < size; i++){//contamos a lo más cuántas proposiciones simples hay
        if(isalpha(prop[i]) == 2){
            contador ++;
        }
    }
    return contador;
}

int encuentraCaracterEnCadena(CharVec* cv, char c){
    for(int i = 0; i < cv->size; i++){
        if(cv->cVec[i] == c){
            return 1;
        }
    }
    return 0;
}

int indicePrimerEspacio(CharVec* cv){
    for(int i = 0; i < cv->size; i++){
        if(cv->cVec[i] == 32){
            return i;
        }
    }
}

int cuentaLosQueNoSonEspacios(CharVec* cv){
    int contador = 0;
    for(int i = 0; i < cv->size; i++){
        if(cv->cVec[i] != 32){
            contador ++;
        }
    }
    return contador;
}

int cuentaProposicionesSimples(char *prop){
    CharVec propaux;
    createCharVec(&propaux, cuentaCuantasLetrasHay(prop));//creamos un vector con memoria dinámica donde estarán las proposiciones simples sin repetirse
    for(int i = 0; i < propaux.size; i++){//llenamos el vector auxiliar de espacios
        propaux.cVec[i] = 32;
    }
    for(int i = 0; i < (int) strlen(prop); i++){
        if(isalpha(prop[i]) == 2){
            if(encuentraCaracterEnCadena(&propaux, prop[i]) == 0){
                propaux.cVec[indicePrimerEspacio(&propaux)] = prop[i];
            }
        }
    }
    int numprop = cuentaLosQueNoSonEspacios(&propaux);
    freeCharVec(&propaux);
    return numprop;
}

int cuentaProposicionesCompuestas(char *prop){
    int size = (int) strlen(prop);
    int contador = 0;
    for(int i = 0; i < size; i++){
        if(prop[i] == 40){
            contador ++;
        }
    }
    return contador;
}

int cuentaProposicionesSimplesYCompuestas(char* prop){
    return cuentaProposicionesCompuestas(prop) + cuentaProposicionesSimples(prop);
}

int longitudDeVectoresProp(char* prop){
    return pow(2, cuentaProposicionesSimples(prop));
}

void createProp(Prop *iv, int size){
    iv->size = size;
    iv->truthValues = (int *) malloc(iv->size * sizeof(int));
    if(!iv->truthValues){
        printf("\nERROR NO HAY MEMORIA\n");
        iv->size = 0;
        return;
    }
    return;
}

void createVectorProp(VectorProp* vp, int sizeVectorProp, int sizeEachVector){
    vp->size = sizeVectorProp;
    vp->props = (Prop*) malloc(vp->size * sizeof(Prop));
    if(!vp->props){
        vp->size = 0;
        printf("\nERROR NO HAY MEMORIA\n");
        return;
    }
    for(int i = 0; i < vp->size; i++){
        createProp(&vp->props[i], sizeEachVector);
    }
    return;
}

void freeProp(Prop *iv){
    free(iv->truthValues);
    return;
}

void freeVectorProp(VectorProp* vp){
    for(int i = 0; i < vp->size; i++){
        freeProp(&vp->props[i]);
    }
    free(vp->props);
    return;
}

void inicializaPropSimples(VectorProp* vp, int nPropSimples){
    int divDelVector = 2;
    int valor = 1;
    int contador = 0;
    for(int i = 0; i < nPropSimples; i++){
        for(int k = 0, j = 0; k < vp->props[i].size; k++, j++){
            if((j % ((vp->props[i].size) / divDelVector)) == 0){
                valor = (valor + 1) % 2;
            }
            vp->props[i].truthValues[k] = valor;
        }
        divDelVector *= 2;
    }
    return;
}

int tieneAlgunoEsteNombre(VectorProp* vp, int nombre, int numPropSimples){
    for(int i = 0; i < numPropSimples; i++){
        if(vp->props[i].nombre == nombre){
            return 1;
        }
    }
    return 0;
}

void nombraProposiciones(char* prop, VectorProp* vp, int PropTotales, int PropSimples){
    for(int i = 0; i < PropTotales; i++){//inicializamos todos los nombres a 0
        vp->props[i].nombre = 0;
    }
    char* ptr = prop;//creamos un apuntador a caracter que nos servirá como auxiliar
    int propsize = strlen(prop);
    int aux;//un entero que también nos servirá como auxiliar
    int indxSimple = 0; //el índice de las proposiciones simples que vamos a nombrar
    int indxCompuesto = 1;// el índice de las proposiciones compuestas
    int aux1 = PropSimples;//auxiliar dos que nos servirá para nombrar a las proposiciones compuestas
    int aux2 = 0;
    while(aux2 != propsize){//mientras no terminemos de leer toda la proposición ingresada, le seguimos.
        aux = *ptr;//El entero va a ser el contenido de a donde apunta ptr
        if(aux >= 97 && aux <= 122){//si aux está en este intervalo es que es una proposición simple lo que apunta
            if(tieneAlgunoEsteNombre(vp, aux, PropSimples) == 0){//si hay una proposición simple que es nueva, es decir, que no tiene nombre, le ponemos nombre, en específico, el número que le corresponde en la tabla ascii
                vp->props[indxSimple].nombre = aux;
                indxSimple++;
            }
        }
        else if(aux == 40){
           vp->props[aux1].nombre = indxCompuesto;
            aux1 ++;
            indxCompuesto ++;
        }
        ptr++;
        aux2++;
    }
    return;
}

int cuantasProposicionesTiene(char* propFragment){
    char* ptr = propFragment;
    int trash = 0;
    int aux;
    int parentesisAbiertos = 0;
    int parentesisCerrados = 0;
    int diferencia;
    int proposiciones = 0;
    do{
        aux = *ptr;
        if(aux == 40){
            parentesisAbiertos++;
            diferencia = parentesisAbiertos - parentesisCerrados;
            if(diferencia == 1){
            proposiciones++;
            }
        }
        else if(aux == 41){
            parentesisCerrados++;
            diferencia = parentesisAbiertos - parentesisCerrados;
            if(diferencia == 1){
            proposiciones++;
            }
        }
        else if(aux <= 122 && aux >= 97){
            parentesisAbiertos++;
            parentesisCerrados++;
            diferencia = parentesisAbiertos - parentesisCerrados;
            if(diferencia == 1){
            proposiciones++;
            }
        }
        ptr++;
    }while(diferencia != 0);
    return proposiciones - 1;
}

char* absorveLoQueNoImporta(char* prop){//va adelantando a prop hasta que encuentra algo que es de interés, en este caso, lo que nos interesa es o un paréntesis abierto o una proposición simple
    prop++;
    while(1){
        if((*prop) == 40){
            break;
        }
        else if((*prop) >= 97 && (*prop) <= 122){
            break;
        }
        prop++;
    }
    return prop;//y se queda prop apuntando a donde está algo que nos interesa
}

int encuentraElNombreDeEsteParentesis(char* prop, char* subProp){
    char* ptr;
    int parentesisAbiertos = 0;
    for(ptr = prop; ptr != subProp; ptr++){
        if(*ptr == 40){
            parentesisAbiertos ++;
        }
    }
    return parentesisAbiertos + 1;
}

char* ponmeEnDondeTermina(char* prop){
    int parentesisAbiertos = 0;
    int parentesisCerrados = 0;
    do{
        if(*prop == 40){
            parentesisAbiertos++;
        }
        else if(*prop == 41){
            parentesisCerrados++;
        }
        prop++;
    }while((parentesisAbiertos - parentesisCerrados) != 0);
    return prop - 1;
}

int encuentraConnector(char* conectorFinder){
    while(1){
        if((*conectorFinder) == 33){
            return CONJ;
        }
        else if((*conectorFinder) == 58){
            return DISY;
        }
        else if((*conectorFinder) == 62){
            return IMPL;
        }
        else if((*conectorFinder) == 60){
            return DIMPL;
        }
        conectorFinder++;
    }
}

int encuentraIndxPropSimplePorNombre(VectorProp* vp, int propSimples, int nombre){
    for(int i = 0; i < propSimples; i++){
        if(nombre == vp->props[i].nombre){
            return i;
        }
    }
    return 0;
}

void creaConeccionesEntreProposicionesRecursive(char* prop, char* estamosEn, VectorProp* vp, int propSimples, int propTotales){//prop apunta al inicio del array, estamosEn apunta a ya sea, un paréntesis abierto o a una proposición simple
    int numeroDeProps = cuantasProposicionesTiene(estamosEn);
    char* estamosEnCpy = estamosEn;
    char* conector;
    switch (numeroDeProps){
    case 1:
        estamosEnCpy = absorveLoQueNoImporta(estamosEnCpy);
        if((*estamosEnCpy) == 40){
            vp->props[(propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEn)].prop1 = encuentraElNombreDeEsteParentesis(prop, estamosEnCpy);
            vp->props[(propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEn)].indxP1 = (propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEnCpy);
            vp->props[(propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEn)].conector = NEG;
            creaConeccionesEntreProposicionesRecursive(prop, estamosEnCpy, vp, propSimples, propTotales);
            return;
        }
        else if((*estamosEnCpy) >= 97 && (*estamosEnCpy) <= 122){
            vp->props[(propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEn)].prop1 = (*estamosEnCpy);
            vp->props[(propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEn)].indxP1 = encuentraIndxPropSimplePorNombre(vp, propSimples, *estamosEnCpy);
            vp->props[(propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEn)].conector = NEG;
            return;
        }
        break;
    case 2:
        estamosEnCpy = absorveLoQueNoImporta(estamosEnCpy);
        if((*estamosEnCpy) == 40){
            vp->props[(propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEn)].prop1 = encuentraElNombreDeEsteParentesis(prop, estamosEnCpy);
            vp->props[(propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEn)].indxP1 = (propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEnCpy);
            creaConeccionesEntreProposicionesRecursive(prop, estamosEnCpy, vp, propSimples, propTotales);
            estamosEnCpy = ponmeEnDondeTermina(estamosEnCpy);
            conector = estamosEnCpy;
            vp->props[(propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEn)].conector = encuentraConnector(conector);
            estamosEnCpy = absorveLoQueNoImporta(estamosEnCpy);
            if((*estamosEnCpy) == 40){
                vp->props[(propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEn)].prop2 = encuentraElNombreDeEsteParentesis(prop, estamosEnCpy);
                vp->props[(propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEn)].indxP2 = (propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEnCpy);
                creaConeccionesEntreProposicionesRecursive(prop, estamosEnCpy, vp, propSimples, propTotales);
                return;
            }
            else if((*estamosEnCpy) >= 97 && (*estamosEnCpy) <= 122){
                vp->props[(propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEn)].prop2 = (*estamosEnCpy);
                vp->props[(propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEn)].indxP2 = encuentraIndxPropSimplePorNombre(vp, propSimples, *estamosEnCpy);
                return;
            }
        }
        else if((*estamosEnCpy) >= 97 && (*estamosEnCpy) <= 122){
            vp->props[(propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEn)].prop1 = *estamosEnCpy;
            vp->props[(propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEn)].indxP1 = encuentraIndxPropSimplePorNombre(vp, propSimples, *estamosEnCpy);
            conector = estamosEnCpy;
            vp->props[(propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEn)].conector = encuentraConnector(conector);
            estamosEnCpy = absorveLoQueNoImporta(estamosEnCpy);
            if((*estamosEnCpy) == 40){
                vp->props[(propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEn)].prop2 = encuentraElNombreDeEsteParentesis(prop, estamosEnCpy);
                vp->props[(propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEn)].indxP2 = (propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEnCpy);
                creaConeccionesEntreProposicionesRecursive(prop, estamosEnCpy, vp, propSimples, propTotales);
                return;
            }
            else if((*estamosEnCpy) >= 97 && (*estamosEnCpy) <= 122){
                vp->props[(propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEn)].prop2 = *estamosEnCpy;
                vp->props[(propSimples - 1) + encuentraElNombreDeEsteParentesis(prop, estamosEn)].indxP2 = encuentraIndxPropSimplePorNombre(vp, propSimples, *estamosEnCpy);
                return;
            }
        }
        break;
    } 
}

void inicializaProposicionesInternas(VectorProp* vp, int propTotales){
    for(int i = 0; i < propTotales; i++){
        vp->props[i].prop1 = 0;
        vp->props[i].prop2 = 0;
    }
    return;
}

void inicializaConectores(VectorProp* vp, int propTotales){
    for(int i = 0; i < propTotales; i++){
        vp->props[i].conector = 0;
    }
    return;
}

void inicializaIndxProp(VectorProp* vp, int propTotales){
    for(int i = 0; i < propTotales; i++){
        vp->props[i].indxP1 = 0;
        vp->props[i].indxP2 = 0;
    }
    return;
}

void conjuncion(int* prop1, int* prop2, int* propres, int size){
    for(int i = 0; i < size; i++){
        if(prop1[i] == 1 && prop2[i] == 1){
            propres[i] = 1;
        }
        else{
            propres[i] = 0;
        }
    }
    return;
}

void negacion(int* prop1, int* propres, int size){
    for(int i = 0; i < size; i++){
        if(prop1[i] == 1){
            propres[i] = 0;
        }
        else{
            propres[i] = 1;
        }
    }
    return;
}

void disyuncion(int* prop1, int* prop2, int* propres, int size){
    for(int i = 0; i < size; i++){
        if(prop1[i] == 1){
            propres[i] = 1;
        }
        else if(prop2[i] == 1){
            propres[i] = 1;
        }
        else{
            propres[i] = 0;
        }
    }
    return;
}

void implicacion(int* prop1, int* prop2, int* propres, int size){
    for(int i = 0; i < size; i++){
        if(prop1[i] == 1 && prop2[i] == 0){
            propres[i] = 0;
        }
        else{
            propres[i] = 1;
        }
    }
    return;
}

void dobleimplicacion(int* prop1, int* prop2, int* propres, int size){
    for(int i = 0; i < size; i++){
        if(prop1[i] == 1 && prop2[i] == 1){
            propres[i] = 1;
        }
        else if(prop1[i] == 0 && prop2[i] == 0){
            propres[i] = 1;
        }
        else{
            propres[i] = 0;
        }
    }
    return;
}

void inicializaPropCompuestas(VectorProp* vp, int propSimples, int propCompuestas, int propTotales){
    for(int i = 1; i <= propCompuestas; i++){
        vp->props[(propSimples - 1) + i].evaluado = 0;
    }
    for(int i = 0; i < propSimples; i++){
        vp->props[i].evaluado = 1;
    }
    for(int i = 1; i <= propCompuestas; i++){
        for(int k = 0; k < vp->props[i].size; k++){
            vp->props[(propSimples - 1) + i].truthValues[k] = 0;
        }
    }
    return;
}

void evaluaLasProposiciones(VectorProp* vp, int indx){
    if(vp->props[indx].evaluado == 1){
        return;
    }
    else{
        switch (vp->props[indx].conector){
        case NEG:
            if(vp->props[vp->props[indx].indxP1].evaluado == 0){
                evaluaLasProposiciones(vp, vp->props[indx].indxP1);
            }
            negacion(vp->props[vp->props[indx].indxP1].truthValues, vp->props[indx].truthValues, vp->props[indx].size);
            vp->props[indx].evaluado = 1;
            return;
            break;
        case CONJ:
            if(vp->props[vp->props[indx].indxP1].evaluado == 0){
                evaluaLasProposiciones(vp, vp->props[indx].indxP1);
            }
            if(vp->props[vp->props[indx].indxP2].evaluado == 0){
                evaluaLasProposiciones(vp, vp->props[indx].indxP2);
            }
            conjuncion(vp->props[vp->props[indx].indxP1].truthValues, vp->props[vp->props[indx].indxP2].truthValues, vp->props[indx].truthValues, vp->props[indx].size);
            vp->props[indx].evaluado = 1;
            return;
            break;
        case DISY:
            if(vp->props[vp->props[indx].indxP1].evaluado == 0){
                evaluaLasProposiciones(vp, vp->props[indx].indxP1);
            }
            if(vp->props[vp->props[indx].indxP2].evaluado == 0){
                evaluaLasProposiciones(vp, vp->props[indx].indxP2);
            }
            disyuncion(vp->props[vp->props[indx].indxP1].truthValues, vp->props[vp->props[indx].indxP2].truthValues, vp->props[indx].truthValues, vp->props[indx].size);
            vp->props[indx].evaluado = 1;
            return;
            break;
        case IMPL:
            if(vp->props[vp->props[indx].indxP1].evaluado == 0){
                evaluaLasProposiciones(vp, vp->props[indx].indxP1);
            }
            if(vp->props[vp->props[indx].indxP2].evaluado == 0){
                evaluaLasProposiciones(vp, vp->props[indx].indxP2);
            }
            implicacion(vp->props[vp->props[indx].indxP1].truthValues, vp->props[vp->props[indx].indxP2].truthValues, vp->props[indx].truthValues, vp->props[indx].size);
            vp->props[indx].evaluado = 1;
            return;
            break;
        case DIMPL:
            if(vp->props[vp->props[indx].indxP1].evaluado == 0){
                evaluaLasProposiciones(vp, vp->props[indx].indxP1);
            }
            if(vp->props[vp->props[indx].indxP2].evaluado == 0){
                evaluaLasProposiciones(vp, vp->props[indx].indxP2);
            }
            dobleimplicacion(vp->props[vp->props[indx].indxP1].truthValues, vp->props[vp->props[indx].indxP2].truthValues, vp->props[indx].truthValues, vp->props[indx].size);
            vp->props[indx].evaluado = 1;
            return;
            break;
        }
    }
}

/*Crea un vector prop, hay que liberar.*/
void leeYEvaluaProp(char* prop, VectorProp* vp){
    int propSimples = cuentaProposicionesSimples(prop);
    int propCompuestas = cuentaProposicionesCompuestas(prop);
    int propTotales = propSimples + propCompuestas;
    createVectorProp(vp, propTotales, longitudDeVectoresProp(prop));
    inicializaPropSimples(vp, propSimples);
    nombraProposiciones(prop, vp, propTotales, propSimples);
    inicializaProposicionesInternas(vp, propTotales);
    inicializaIndxProp(vp, propTotales);
    inicializaConectores(vp, propTotales);
    creaConeccionesEntreProposicionesRecursive(prop, prop, vp, propSimples, propTotales);
    inicializaPropCompuestas(vp, propSimples, propCompuestas, propTotales);
    evaluaLasProposiciones(vp, propSimples);
    return;
}

void imprimeLinea(char* prop, VectorProp* vp, int indx, int propSimples){
    int longitud = strlen(prop);
    char* aux = prop;
    int parentesisAbiertos = 0;
    for(int i = 0; i < longitud; i++){
        if((*aux) == 40){
            parentesisAbiertos++;
            printf("%d", vp->props[(propSimples - 1) + parentesisAbiertos].truthValues[indx]);
        }
        else if((*aux) >= 97 && (*aux) <= 122){

            printf("%d", vp->props[encuentraIndxPropSimplePorNombre(vp, propSimples, (*aux))].truthValues[indx]);
        }
        else{
            printf(" ");
        }
        aux++;
    }
}

void imprimeTablaDeVerdad(char* prop, VectorProp* vp){
    int propSimples = cuentaProposicionesSimples(prop);
    printf("%s\n", prop);
    for(int i = 0; i < vp->props[0].size; i++){
        imprimeLinea(prop, vp, i, propSimples);
        printf("\n");
    }
    return;
}


#endif
