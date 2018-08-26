/* ================================================================== *
	Universidade Federal de Sao Carlos - UFSCar, Sorocaba

	Disciplina: Estruturas de Dados 1
	Prof. Tiago A. Almeida

	Exercício 18 - Árvore AVL (Adelson-Velskii e Landis)

	Instrucoes
	----------

	Este arquivo contem o codigo que auxiliara no desenvolvimento do
	exercicio. Voce precisara completar as funcoes requisitadas.

	Voce nao podera criar nenhuma outra funcao. Complete apenas as
	rotinas	fornecidas.

	Nome: Michele Argolo Carvalho
 * ================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 70
#define TRUE 1
#define FALSE 0

// Frase exibida quando houver erro nas operacoes
#define FRASE_ERROALOCACAO	"Erro de alocacao!\n"
#define FRASE_OPCAO_INVAL	"Opcao invalida!\n"
#define FRASE_ERRO_AVL		"Erro: a arvore nao possui estrutura de AVL!!!\n"

// Definicao do elemento da árvore
typedef struct {
    int ra;
    char nome[MAX];
} Aluno;

// Definicao do nó
typedef struct no {
    Aluno info;
    struct no *esq, *dir;
    short int bal;
} No;

// Definicao da Arvore
typedef No *Arvore;


typedef struct queue {
    int N, max_size; 			//qtd de elementos da fila
    Arvore arvore;
    struct queue *next;
} queue;

/* ================================================================== */
/* ===================== PREAMBULO DAS FUNCOES ====================== */
/* ================================================================== */

/* Verifica se a árvore informada tem a estrutura de uma árvore AVL.
 * NÃO ALTERAR!!! */
int verificaAVL(Arvore t, int *alt);

/* Inicializa a arvore. */
void inicArvore(Arvore *t);

/* Caso exista um aluno com o RA informado, returne TRUE e atribua
 * em 'a' os dados do aluno. Caso contrário retorne FALSE. */
int consultaAluno(Arvore t, int ra, Aluno *a);

/* Imprime a arvore sob a forma de uma árvore 'deitada'. */
void imprimeArvore(Arvore t, int nivel);

/* Devolve o número de nós da arvore 't'. */
int numeroNosArvore(Arvore t);

/* Devolve a altura da arvore 't'. */
int alturaArvore(Arvore t);

/* Executa um percurso inordem na arvore, imprimindo todos os elementos. */
void percorreArvore(Arvore t);

/* Libera todos os nós da arvore apontada por 't', bem
como todas as cadeias que guardam os nomes. */
void liberaArvore(Arvore *t);

/** Rotinas para rotação na árvore **/

/* Rotação simples RR - Rotação Direira */
void RR(Arvore *t);

/* Rotação simples LL - Rotação Esquerda */
void LL(Arvore *t);

/* Rotação dupla RL - Rotação Direita-Esquerda*/
void RL(Arvore *t);

/* Rotação dupla LR - Rotação Esquerda-Direita */
void LR(Arvore *t);

int altura_AVL(Arvore t);
/** Rotinas auxiliares para inserção. */

/* Faz os ajustes quando a sub-árvore esquerda cresceu. */
void SubArvoreEsquerdaCresceu(Arvore *t, int *cresceu);

/* Faz os ajustes quando a sub-árvore direita cresceu. */
void SubArvoreDireitaCresceu(Arvore *t, int *cresceu);

/* Insere o aluno informado como parâmetro caso não exista aluno
 * com o mesmo RA e devolve TRUE. Caso contrário devolve FALSE.
 * A variável 'cresceu' recebe TRUE se a há aumento da altura da
 * árvore e FALSE, caso contrário. */
int insereAlunoAVL(Arvore *t, Aluno a, int *cresceu);

/* Cria um novo nó, com registro do aluno 'a'. */
Arvore novoNo(Aluno a);

void largura(Arvore t);

int max_bal(Arvore t);

int removeAlunoAVL(Arvore *t, int ra, int *mudouAltura);
void rotLLremove(Arvore *p, int *mudouAltura);
void rotLRremove(Arvore *p, int *mudouAltura);


/* ================================================================== */
/* ======================== ROTINA PRINCIPAL ======================== */
/* ================================================================== */

int main() {
    int info, ra, nivel;
    Aluno aluno;
    Arvore t;
    char acao;
    int cresceu, mudouAltura;
    int altura, max;

    inicArvore(&t);

    //Enquanto opcao for diferente de SAIR
    do {

        //Pergunta a opcao que o usuario deseja realizar
        scanf("\n%c", &acao);

        //Verifica qual opcao foi escolhida.
        switch (acao) {

            case 'i':
                scanf("%d", &aluno.ra);

                info = insereAlunoAVL(&t, aluno, &cresceu);
                if (info == FALSE)
                    printf("RA %d ja estava presente na arvore de dados.\n", aluno.ra);

                else if(info == -1){
                    printf(FRASE_ERROALOCACAO);
                    exit(0);
                }
                break;

            case 'c':
                scanf("%d",&ra);
                if (consultaAluno(t, ra, &aluno))
                    printf("%d, %s\n", aluno.ra, aluno.nome);
                else
                    printf("RA %d nao encontrado na arvore de dados.\n", ra);
                break;

            case 'd':
                nivel = 0;
                imprimeArvore(t, nivel);
                break;

            case 'n':
                printf("Numero de nos na arvore de dados: %d.\n", numeroNosArvore(t));
                break;

            case 'a':
                printf("Altura da arvore de dados: %d.\n", alturaArvore(t));
                break;

            case 'p':
                percorreArvore(t);
                break;

            case 'x':
                if (!verificaAVL(t, &altura))
                    printf(FRASE_ERRO_AVL);

                liberaArvore(&t);
                break;

            case 'y':
                largura(t);
                break;

            case 'b':
                max = max_bal(t);
                printf("O val de bal é: %d\n", max);
                break;

            case 'r':
                scanf("%d", &ra);

                info = removeAlunoAVL(&t, ra, &mudouAltura);
                if (info == -1)
                    printf("RA %d nao estava presente na arvore de dados.\n", ra);

                else {
                    printf("RA %d removido com sucesso.\n", ra);
                }
                break;

            case 'w':
                altura = altura_AVL(t);
                printf("%d\n", altura);

                break;


            default:
                printf(FRASE_OPCAO_INVAL);
                break;
        }

    } while (acao != 'x');

    printf("Processamento finalizado\n");
    return 0;
}

/* ================================================================== */
/* ========== FUNCOES QUE DETERMINAM AS OPERACOES POSSIVEIS ========= */
/* ================================================================== */

/* Verifica se a árvore informada tem a estrutura de uma árvore AVL.
 * NÃO ALTERAR!!! */
int verificaAVL(Arvore t, int *alt) {
    int altEsq, altDir, bal, resEsq, resDir;

    if (t == NULL) {
        *alt = 0;
        return TRUE;
    }

    resEsq = verificaAVL(t->esq, &altEsq);
    resDir = verificaAVL(t->dir, &altDir);
    bal = t->bal;

    *alt = ((altEsq<altDir) ? altDir : altEsq) + 1;

    return ((bal >= -1) && (bal <= +1)
            && (bal == (altDir-altEsq))
            && resEsq && resDir);
}

/* Inicializa a arvore. */
void inicArvore(Arvore *t) {
    *t = NULL;
}

/* Caso exista um aluno com o RA informado, returne TRUE e atribua
 * em 'a' os dados do aluno. Caso contrário retorne FALSE. */
int consultaAluno(Arvore t, int ra, Aluno *a) {
    if(t == NULL)
        return FALSE;
    else if(ra < t->info.ra)
        consultaAluno(t->esq, ra, a);
    else if(ra > t->info.ra)
        consultaAluno(t->dir, ra, a);
    else if(ra == t->info.ra){
        *a = t->info;
        return TRUE;
    }

}

/* Imprime a arvore sob a forma de uma árvore 'deitada'. */
void imprimeArvore(Arvore t, int nivel){
    /* COMPLETAR AQUI */
}

/* Devolve o número de nós da arvore 't'. */
int numeroNosArvore(Arvore t){
    if(t == NULL) return 0;
    int ne = numeroNosArvore(t->esq);
    int nd = numeroNosArvore(t->dir);
    return (ne + nd + 1);
}

/* Devolve a altura da arvore 't'. */
int alturaArvore(Arvore t){
    if(t == NULL) return -1;
    else {
        int he = alturaArvore(t->esq);
        int hd = alturaArvore(t->dir);
        if (he > hd)
            return (he + 1);
        else
            return (hd + 1);
    }
}

/* Executa um percurso inordem na arvore, imprimindo todos os elementos. */
void percorreArvore(Arvore t){ //inordem = ERD
    if(t == NULL) return;
    percorreArvore(t->esq);
    printf("%d %s\n", t->info.ra, t->info.nome);
    percorreArvore(t->dir);
}

/* Libera todos os nós da arvore apontada por 't', bem
como todas as cadeias que guardam os nomes. */
void liberaArvore(Arvore *t){
    if(*t != NULL){
        liberaArvore(&(*t)->esq);
        liberaArvore(&(*t)->dir);
        free(*t);
    }
}

/** Rotinas para rotação na árvore **/

/* Rotação simples RR - Rotação Direira */
void RR(Arvore *t) {
    Arvore B = (*t)->dir;
    (*t)->dir = B->esq;
    B->esq = (*t);
    (*t)->bal = 0;
    B->bal  = 0;
    *t = B;
}

/* Rotação simples LL - Rotação Esquerda */
void LL(Arvore *t) {
    Arvore B = (*t)->esq;
    (*t)->esq = B->dir;
    B->dir = (*t);
    (*t)->bal = 0;
    B->bal  = 0;
    *t = B;
}

/* Rotação dupla RL - Rotação Direita-Esquerda*/
void RL(Arvore *t) {
    Arvore B = (*t)->dir;
    Arvore C = B->esq;
    B->esq = C->dir;
    C->dir = B;
    (*t)->dir = C->esq;
    C->esq = (*t);
    if (C->bal == -1) B->bal = +1; else B->bal = 0;
    if (C->bal == +1) (*t)->bal = -1; else (*t)->bal = 0;
    C->bal = 0;
    *t = C;
}

/* Rotação dupla LR - Rotação Esquerda-Direita*/
void LR(Arvore *t) {
    Arvore B = (*t)->esq;
    Arvore C = B->dir;
    B->dir = C->esq;
    C->esq = B;
    (*t)->esq = C->dir;
    C->dir = (*t);
    if (C->bal == +1) B->bal = -1; else B->bal = 0;
    if (C->bal == -1) (*t)->bal = +1; else (*t)->bal = 0;
    C->bal = 0;
    *t = C;
}

/** Rotinas auxiliares para inserção. */

/* Cria um novo nó, com registro do aluno 'a'. */
Arvore novoNo(Aluno a) {
    Arvore novo = (Arvore)malloc(sizeof(No));
    novo->info = a;
    novo->bal = 0;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}


/* Insere o aluno informado como parâmetro caso não exista aluno
 * com o mesmo RA e devolve TRUE. Caso contrário devolve FALSE.
 * A variável 'cresceu' recebe TRUE se a há aumento da altura da
 * árvore e FALSE, caso contrário. */
int insereAlunoAVL(Arvore *t, Aluno a, int *cresceu) {
    if(*t == NULL){
        *t = novoNo(a);
        *cresceu = TRUE;
        return TRUE;
    }
    else if((*t)->info.ra == a.ra){
        *cresceu = FALSE;
        return FALSE;
    } else if( a.ra < (*t)->info.ra ){
        if(insereAlunoAVL(&(*t)->esq, a, cresceu)){
            if(*cresceu){       //inseriu na esquerda, vamos verificar balanceamento dos nós
                switch((*t)->bal){
                    case -1:
                        if (t->esq->bal== -1){ // sinais iguais – pendem para mesmo lado
                            LL(t);
                            *cresceu = 0;
                        }else if (t->esq->bal == 1){
                            LR(t);
                            *cresceu = 0;
                        }else{
                            LL(t); // retorna balanceado
                            *cresceu = 1;
                        }
                        break;
                    case 0:
                        (*t)->bal = -1; // ficou maior à esq.
                        *cresceu = 1; // propaga verificação
                        break;
                    case 1: // era maior à direita
                        (*t)->bal = 0; // balanceou com ins. esq
                        *cresceu = 0; //interrompe propagação
                        break;
                }
            }
            return 1;
        }
        else
            return 0;
    } else if(a.ra > (*t)->info.ra ){
        if(insereAlunoAVL(&(*t)->dir, a, cresceu)){
            if(*cresceu){       //inseriu na direita, vamos verificar balanceamento dos nós
                switch((*t)->bal){
                    case -1:
                        (*t)->bal = 0; // balanceou com ins. esq
                        *cresceu = 0; //interrompe propagação
                        break;
                    case 0:
                        (*t)->bal = 1; // ficou maior à dir
                        *cresceu = 1; // propaga verificação
                        break;
                    case 1: // era maior à direita
                        if (t->dir->bal == 1){ // sinais iguais – pendem para mesmo lado
                            RR(t);
                            *cresceu = 0;
                        }else if (t->dir->bal == -1){
                            RL(t); // retorna balanceado
                            *cresceu = 0;
                        }else{
                            RR(t);
                            *cresceu = 1;
                        }break;
                }
            }
            return 1;
        }
        else
            return 0;
    }


}


void create(queue *q, Arvore t){
    q->next = NULL;
    q->N = 0;
    q->max_size = 100;
}

int empty(queue *q){
    if(q->N == 0)
        return 1;
    return 0;
}

void put(queue *q, Arvore t){
    if(q->N < q->max_size){
        queue *atual = q->next;
        queue *elemento = (queue *)malloc(sizeof(queue));
        elemento->arvore = t;
        elemento->next = NULL;

        if(q->next == NULL){
            q->next = elemento;
        } else {		//insere no fim
            while(atual->next != NULL)
                atual = atual->next;    //atual tem ultimo elemento

            atual->next = elemento;
        }
        q->N++;
    }
}

Arvore get(queue *q){
    if(q->N > 0){
        q->N--;
        Arvore a = q->next->arvore;
        queue *primeiro = q->next;

        q->next = primeiro->next;
        free(primeiro);
        return a;
    }
    return NULL;
}

void largura(Arvore t){
    queue fila;
    create(&fila, t);

    put(&fila, t);

    while(!empty(&fila)){
        Arvore e = get(&fila);
        printf("%d %d ", e->info.ra, e->bal);
        if(e->esq != NULL)
            put(&fila, e->esq);
        if(e->dir != NULL)
            put(&fila, e->dir);
    }
    printf("\n");
}

int max_bal(Arvore t){
    if(t == NULL) return 0;
    else{
        max_bal(t->esq);
        int maxe = (t->bal);
        max_bal(t->dir);
        int maxd = (t->bal);
        if(maxe > maxd)
            return maxe;
        else
            return maxd;
    }
}


void rotLLremove(Arvore *p, int *mudouAltura) {
    Arvore A = (*p)->esq;
    (*p)->esq = A->dir;
    A->dir = *p;
    if(A->bal == 0) {
        A->bal = 1;
        (*p)->bal = -1;
        *mudouAltura = 0;
    } else {
        A->bal = 0;
        (*p)->bal = 0;
        *mudouAltura = 1;
    }
    *p = A;
}

void rotRRremove(Arvore *p, int *mudouAltura) {
    Arvore B = (*p)->dir;
    (*p)->dir = B->esq;
    B->esq = *p;
    if (B->bal == 0) {
        B->bal = -1;
        (*p)->bal = 1;
        *mudouAltura = 0;
    } else {
        B->bal = 0;
        (*p)->bal = 0;
        *mudouAltura = 1;
    }
    *p = B;
}

void rotLRremove(Arvore *p, int *mudouAltura) {
    *mudouAltura = 1;
    return LR(p);
}

void rotRLremove(Arvore *p, int *mudouAltura) {
    *mudouAltura = 0;
    return RL(p);
}

int sucessor(Arvore *atual, Arvore *proximo, int *mudouAltura){
    if((*proximo)->dir != NULL){
        if(sucessor(atual, &(*proximo)->dir, mudouAltura)){
            switch((*proximo)->bal){
                case -1:
                if ((*proximo)->esq->bal== -1) // sinais iguais – pendem para mesmo lado
                    rotLLremove(proximo, mudouAltura);
                else rotLRremove(proximo, mudouAltura); // retorna balanceado
                break;

                case 0:
                    (*proximo)->bal = -1;
                    *mudouAltura = 0;
                    break;

                case 1:
                    (*proximo)->bal = 0;
                    *mudouAltura = 1;
                    break;
            }
        }
        return (FALSE);
    } else {
        (*atual)->info = (*proximo)->info;
        (*atual) = *proximo;
        *proximo = (*proximo)->esq;
        return(TRUE);
    }
}

int removeAlunoAVL(Arvore *t, int ra, int *mudouAltura) {
    if(*t == NULL){     //Apenas retorna falso quando não encontra nenhum elemento
        return -1;
    } else if( ra < (*t)->info.ra ){
        if(removeAlunoAVL(&(*t)->esq, ra, mudouAltura)){
            if(*mudouAltura){   //Vamos analisar o que acontece
                switch((*t)->bal) {
                    case -1:
                        (*t)->bal = 0;
                        *mudouAltura = 1;
                        break;

                    case 0:
                        (*t)->bal = 1;
                        *mudouAltura = 0;
                        break;

                    case 1: //sub árvore direita cresceu
                        //oooops, precisa balancear
                        //Houve remoção da esquerda então balanceia a direita
                        if ((*t)->dir->bal == 1) // sinais iguais – pendem para mesmo lado
                            rotRRremove(t, mudouAltura);
                        else if ((*t)->dir->bal == -1)
                            rotRLremove(t, mudouAltura); // retorna balanceado
                        else
                            rotRRremove(t, mudouAltura);
                        break;

                }
            }
        }
    } else if(ra > (*t)->info.ra ){
        if(removeAlunoAVL(&(*t)->dir, ra, mudouAltura)){
            if(*mudouAltura){
                switch((*t)->bal) {
                    case -1:        //ops, precisa balancear
                        //Houve remoção da direita então balenceia a esquerda
                        if ((*t)->esq->bal== -1) // sinais iguais – pendem para mesmo lado
                            rotLLremove(t, mudouAltura);
                        else if ((*t)->esq->bal == 1)
                            rotLRremove(t, mudouAltura);
                        else
                            rotLLremove(t, mudouAltura); // retorna balanceado
                        break;

                    case 0:
                        (*t)->bal = -1;
                        *mudouAltura = 0;
                        break;

                    case 1:
                        (*t)->bal = 0;
                        *mudouAltura = 1;
                        break;

                }
            }
        }

    } else { //Encontrou o elemento
        Arvore aux;
        aux = *t;

        if(aux->esq == NULL){
            *t = aux->dir;
            *mudouAltura = 1;
            return 1;
        } else if (aux->dir == NULL){
            *t = aux->esq;
            *mudouAltura = 1;
            return 1;
        } else {
            if(sucessor(&aux, &aux->esq, mudouAltura)) {
                switch ((*t)->bal) {
                    case -1:
                        (*t)->bal = 0;
                        *mudouAltura = 1;
                        break;

                    case 0:
                        (*t)->bal = 1;
                        *mudouAltura = 0;
                        break;

                    case 1: //sub árvore direita cresceu
                        //oooops, precisa balancear
                        //Houve remoção da esquerda então balanceia a direita
                        if ((*t)->dir->bal == 1) // sinais iguais – pendem para mesmo lado
                            rotRRremove(t, mudouAltura);
                        else if ((*t)->dir->bal == -1)
                            rotRLremove(t, mudouAltura); // retorna balanceado
                        else
                            rotRRremove(t, mudouAltura);
                        break;

                }
            }
        }
        free(aux);
        return 1;
    }
}

int altura_AVL(Arvore t){
    if(t == NULL) return -1;
    else{
        if(t->bal == 1)
            return 1 + altura_AVL(t->dir);
        else
            return 1 + altura_AVL(t->esq);
    }
}
