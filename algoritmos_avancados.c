#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Constantes ---
#define TAMANHO_TABELA_HASH 13 // Um número primo para a tabela hash

// --- Estruturas ---

// Estrutura para um cômodo da mansão (nó da árvore binária de navegação)
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
    int pista_encontrada; // Flag para saber se a pista já foi coletada
} Sala;

// Estrutura para um nó da árvore de busca binária de pistas
typedef struct NoPista {
    char pista[100];
    struct NoPista *esquerda;
    struct NoPista *direita;
} NoPista;

// Estrutura para uma entrada na tabela hash (pista -> suspeito)
typedef struct EntradaHash {
    char pista[100];
    char suspeito[50];
    struct EntradaHash *proximo; // Para tratamento de colisões (encadeamento)
} EntradaHash;

// Estrutura da Tabela Hash
typedef struct TabelaHash {
    EntradaHash* tabela[TAMANHO_TABELA_HASH];
} TabelaHash;

// --- Protótipos das Funções ---

// Funções da Mansão (Árvore Binária)
Sala* criarSala(const char* nome);
void explorarSalas(Sala* sala_atual, NoPista** raizPistas);
void liberarMemoriaSalas(Sala* sala_atual);

// Funções das Pistas (Árvore de Busca Binária)
NoPista* inserirPista(NoPista* raiz, const char* novaPista);
void mostrarPistasEmOrdem(NoPista* raiz);
void liberarMemoriaPistas(NoPista* raiz);

// Funções da Tabela Hash
unsigned int hash(const char* chave);
void inicializarTabelaHash(TabelaHash* th);
void inserirNaHash(TabelaHash* th, const char* pista, const char* suspeito);
const char* buscarNaHash(TabelaHash* th, const char* pista);
void exibirTabelaHash(TabelaHash* th);
void liberarMemoriaHash(TabelaHash* th);

// Funções de Análise
void analisarSuspeitos(TabelaHash* th, NoPista* pistasColetadas);
void contarSuspeitos(NoPista* noPista, TabelaHash* th, int* contadores, char** nomesSuspeitos, int numSuspeitos);


// --- Função Principal ---
int main() {
    // 1. Construção da Árvore Binária (Mapa da Mansão)
    Sala* hall = criarSala("Hall de Entrada");
    hall->esquerda = criarSala("Biblioteca");
    hall->direita = criarSala("Sala de Jantar");
    
    hall->esquerda->esquerda = criarSala("Escritorio");
    hall->esquerda->direita = criarSala("Quarto Principal");
    
    hall->direita->esquerda = criarSala("Cozinha");
    hall->direita->direita = criarSala("Jardim de Inverno");

    // Raiz da Árvore de Busca Binária de Pistas
    NoPista* arvoreDePistas = NULL;

    // Tabela Hash de Suspeitos
    TabelaHash tabelaDeSuspeitos;
    inicializarTabelaHash(&tabelaDeSuspeitos);

    // Preencher a Tabela Hash com associações Pista -> Suspeito
    inserirNaHash(&tabelaDeSuspeitos, "Bilhete rasgado com data antiga", "Mordomo");
    inserirNaHash(&tabelaDeSuspeitos, "Chave dourada com brasao", "Jardineiro");
    inserirNaHash(&tabelaDeSuspeitos, "Diario com anotacoes estranhas", "Dona da Casa");
    inserirNaHash(&tabelaDeSuspeitos, "Copo quebrado", "Mordomo");
    inserirNaHash(&tabelaDeSuspeitos, "Pegadas na lama", "Jardineiro");
    inserirNaHash(&tabelaDeSuspeitos, "Carta de amor", "Dona da Casa");


    printf("--- Bem-vindo ao Detective Quest - Nível Mestre ---\n");
    printf("Você está no Hall de Entrada da mansão. Explore, encontre pistas e desvende o mistério!\n");

    // 2. Inicia a exploração a partir do Hall de Entrada
    explorarSalas(hall, &arvoreDePistas);

    // 3. Após a exploração, exibe as pistas coletadas
    printf("\n--- Pistas Coletadas (em ordem alfabética) ---\n");
    if (arvoreDePistas == NULL) {
        printf("Nenhuma pista foi encontrada.\n");
    } else {
        mostrarPistasEmOrdem(arvoreDePistas);
    }
    printf("------------------------------------------------\n");

    // 4. Exibe as associações Pista -> Suspeito na Tabela Hash
    printf("\n--- Associações Pista -> Suspeito (Tabela Hash) ---\n");
    exibirTabelaHash(&tabelaDeSuspeitos);
    printf("---------------------------------------------------\n");

    // 5. Analisa as pistas coletadas para determinar o suspeito mais citado
    analisarSuspeitos(&tabelaDeSuspeitos, arvoreDePistas);

    // 6. Libera toda a memória alocada
    liberarMemoriaSalas(hall);
    liberarMemoriaPistas(arvoreDePistas);
    liberarMemoriaHash(&tabelaDeSuspeitos);
    printf("\nVocê saiu da mansão. O jogo terminou.\n");

    return 0;
}

// --- Implementações das Funções da Mansão ---

Sala* criarSala(const char* nome) {
    Sala* nova_sala = (Sala*) malloc(sizeof(Sala));
    if (nova_sala == NULL) { printf("Erro: Falha ao alocar memória para a sala.\n"); exit(1); }
    strcpy(nova_sala->nome, nome);
    nova_sala->esquerda = NULL;
    nova_sala->direita = NULL;
    nova_sala->pista_encontrada = 0;
    return nova_sala;
}

void explorarSalas(Sala* sala_atual, NoPista** raizPistas) {
    char escolha;

    while (sala_atual != NULL) {
        printf("\n----------------------------------------\n");
        printf("Você está em: %s\n", sala_atual->nome);

        // Lógica para encontrar pistas em cômodos específicos
        if (strcmp(sala_atual->nome, "Biblioteca") == 0 && !sala_atual->pista_encontrada) {
            printf("Você encontrou uma pista na Biblioteca: 'Bilhete rasgado com data antiga'.\n");
            *raizPistas = inserirPista(*raizPistas, "Bilhete rasgado com data antiga");
            sala_atual->pista_encontrada = 1;
        } else if (strcmp(sala_atual->nome, "Escritorio") == 0 && !sala_atual->pista_encontrada) {
            printf("Você encontrou uma pista no Escritório: 'Chave dourada com brasao'.\n");
            *raizPistas = inserirPista(*raizPistas, "Chave dourada com brasao");
            sala_atual->pista_encontrada = 1;
        } else if (strcmp(sala_atual->nome, "Quarto Principal") == 0 && !sala_atual->pista_encontrada) {
            printf("Você encontrou uma pista no Quarto Principal: 'Diario com anotacoes estranhas'.\n");
            *raizPistas = inserirPista(*raizPistas, "Diario com anotacoes estranhas");
            sala_atual->pista_encontrada = 1;
        } else if (strcmp(sala_atual->nome, "Cozinha") == 0 && !sala_atual->pista_encontrada) {
            printf("Você encontrou uma pista na Cozinha: 'Copo quebrado'.\n");
            *raizPistas = inserirPista(*raizPistas, "Copo quebrado");
            sala_atual->pista_encontrada = 1;
        } else if (strcmp(sala_atual->nome, "Jardim de Inverno") == 0 && !sala_atual->pista_encontrada) {
            printf("Você encontrou uma pista no Jardim de Inverno: 'Pegadas na lama'.\n");
            *raizPistas = inserirPista(*raizPistas, "Pegadas na lama");
            sala_atual->pista_encontrada = 1;
        } // Adicione mais cômodos e pistas conforme necessário para testar a hash

        // Verifica se chegou a um beco sem saída (nó-folha)
        if (sala_atual->esquerda == NULL && sala_atual->direita == NULL) {
            printf("Este cômodo não tem outras saídas. Fim do caminho.\n");
            break;
        }

        // Mostra as opções de caminho
        printf("Para onde você quer ir?\n");
        if (sala_atual->esquerda != NULL) { printf("(e) - Caminho da Esquerda (%s)\n", sala_atual->esquerda->nome); }
        if (sala_atual->direita != NULL) { printf("(d) - Caminho da Direita (%s)\n", sala_atual->direita->nome); }
        printf("(s) - Sair da exploração\n");
        printf("Escolha: ");

        scanf(" %c", &escolha);

        if (escolha == 'e' || escolha == 'E') {
            if (sala_atual->esquerda != NULL) sala_atual = sala_atual->esquerda;
            else printf("Não há caminho para a esquerda aqui.\n");
        } else if (escolha == 'd' || escolha == 'D') {
            if (sala_atual->direita != NULL) sala_atual = sala_atual->direita;
            else printf("Não há caminho para a direita aqui.\n");
        } else if (escolha == 's' || escolha == 'S') {
            printf("Você decidiu parar a exploração por enquanto.\n");
            break;
        } else {
            printf("Opção inválida. Tente novamente.\n");
        }
    }
}

void liberarMemoriaSalas(Sala* sala_atual) {
    if (sala_atual == NULL) return;
    liberarMemoriaSalas(sala_atual->esquerda);
    liberarMemoriaSalas(sala_atual->direita);
    free(sala_atual);
}

// --- Implementações das Funções das Pistas (BST) ---

NoPista* inserirPista(NoPista* raiz, const char* novaPista) {
    if (raiz == NULL) {
        NoPista* novo_no = (NoPista*) malloc(sizeof(NoPista));
        if (novo_no == NULL) { printf("Erro: Falha ao alocar memória para a pista.\n"); exit(1); }
        strcpy(novo_no->pista, novaPista);
        novo_no->esquerda = NULL;
        novo_no->direita = NULL;
        return novo_no;
    }

    int comparacao = strcmp(novaPista, raiz->pista);
    if (comparacao < 0) raiz->esquerda = inserirPista(raiz->esquerda, novaPista);
    else if (comparacao > 0) raiz->direita = inserirPista(raiz->direita, novaPista);
    return raiz;
}

void mostrarPistasEmOrdem(NoPista* raiz) {
    if (raiz == NULL) return;
    mostrarPistasEmOrdem(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    mostrarPistasEmOrdem(raiz->direita);
}

void liberarMemoriaPistas(NoPista* raiz) {
    if (raiz == NULL) return;
    liberarMemoriaPistas(raiz->esquerda);
    liberarMemoriaPistas(raiz->direita);
    free(raiz);
}

// --- Implementações das Funções da Tabela Hash ---

unsigned int hash(const char* chave) {
    unsigned int valor_hash = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        valor_hash = (valor_hash * 31) + chave[i]; // Multiplicar por um primo e somar
    }
    return valor_hash % TAMANHO_TABELA_HASH;
}

void inicializarTabelaHash(TabelaHash* th) {
    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        th->tabela[i] = NULL;
    }
}

void inserirNaHash(TabelaHash* th, const char* pista, const char* suspeito) {
    unsigned int indice = hash(pista);
    
    // Verifica se a pista já existe na lista encadeada para evitar duplicatas
    EntradaHash* atual = th->tabela[indice];
    while(atual != NULL) {
        if(strcmp(atual->pista, pista) == 0) {
            // Pista já existe, apenas atualiza o suspeito (ou não faz nada se for o mesmo)
            strcpy(atual->suspeito, suspeito);
            return;
        }
        atual = atual->proximo;
    }

    // Se não encontrou, cria uma nova entrada
    EntradaHash* nova_entrada = (EntradaHash*) malloc(sizeof(EntradaHash));
    if (nova_entrada == NULL) { printf("Erro: Falha ao alocar memória para entrada hash.\n"); exit(1); }
    strcpy(nova_entrada->pista, pista);
    strcpy(nova_entrada->suspeito, suspeito);
    nova_entrada->proximo = th->tabela[indice]; // Adiciona no início da lista
    th->tabela[indice] = nova_entrada;
}

const char* buscarNaHash(TabelaHash* th, const char* pista) {
    unsigned int indice = hash(pista);
    EntradaHash* atual = th->tabela[indice];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return NULL; // Pista não encontrada
}

void exibirTabelaHash(TabelaHash* th) {
    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        EntradaHash* atual = th->tabela[i];
        if (atual != NULL) {
            printf("Índice %d:\n", i);
            while (atual != NULL) {
                printf("  - Pista: '%s' -> Suspeito: '%s'\n", atual->pista, atual->suspeito);
                atual = atual->proximo;
            }
        }
    }
}

void liberarMemoriaHash(TabelaHash* th) {
    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        EntradaHash* atual = th->tabela[i];
        while (atual != NULL) {
            EntradaHash* proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }
        th->tabela[i] = NULL;
    }
}

// --- Implementações das Funções de Análise ---

// Função auxiliar para contar suspeitos (percorre a BST de pistas)
void contarSuspeitos(NoPista* noPista, TabelaHash* th, int* contadores, char** nomesSuspeitos, int numSuspeitos) {
    if (noPista == NULL) return;

    contarSuspeitos(noPista->esquerda, th, contadores, nomesSuspeitos, numSuspeitos);

    const char* suspeitoEncontrado = buscarNaHash(th, noPista->pista);
    if (suspeitoEncontrado != NULL) {
        for (int i = 0; i < numSuspeitos; i++) {
            if (strcmp(suspeitoEncontrado, nomesSuspeitos[i]) == 0) {
                contadores[i]++;
                break;
            }
        }
    }

    contarSuspeitos(noPista->direita, th, contadores, nomesSuspeitos, numSuspeitos);
}


void analisarSuspeitos(TabelaHash* th, NoPista* pistasColetadas) {
    // Lista de suspeitos conhecidos para contagem
    char* nomesSuspeitos[] = {"Mordomo", "Jardineiro", "Dona da Casa"};
    int numSuspeitos = sizeof(nomesSuspeitos) / sizeof(nomesSuspeitos[0]);
    int contadores[numSuspeitos]; // Contadores para cada suspeito

    for (int i = 0; i < numSuspeitos; i++) {
        contadores[i] = 0; // Inicializa contadores
    }

    // Percorre as pistas coletadas e conta as menções aos suspeitos
    contarSuspeitos(pistasColetadas, th, contadores, nomesSuspeitos, numSuspeitos);

    printf("\n--- Análise de Suspeitos ---\n");
    int maxMencoes = -1;
    const char* suspeitoMaisCitado = "Nenhum";

    for (int i = 0; i < numSuspeitos; i++) {
        printf("%s: %d menções\n", nomesSuspeitos[i], contadores[i]);
        if (contadores[i] > maxMencoes) {
            maxMencoes = contadores[i];
            suspeitoMaisCitado = nomesSuspeitos[i];
        }
    }

    printf("\n>>> O SUSPEITO MAIS CITADO É: %s (com %d menções)\n", suspeitoMaisCitado, maxMencoes);
    printf("----------------------------\n");
}