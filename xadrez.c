/*
 * =============================================================
 * PROJETO: Xadrez Simplificado em C
 * OBJETIVO: Foco didático para iniciantes, mostrando a lógica
 * de um jogo de tabuleiro.
 * REGRAS IMPLEMENTADAS: Movimento da Torre e do Bispo.
 * =============================================================
 */

// --- 1. Bibliotecas Essenciais ---

// stdio.h:    Para funções de entrada e saída (printf, scanf, fgets)
#include <stdio.h> 
// stdlib.h:   Para funções utilitárias (system, abs)
#include <stdlib.h> 
// string.h:   Para manipular strings (strcmp, strcspn)
#include <string.h> 
// ctype.h:    Para checar tipos de caracteres (tolower)
#include <ctype.h> 

// --- 2. Variáveis Globais e Constantes ---

// Nosso tabuleiro. É uma matriz 8x8 de caracteres.
char tabuleiro[8][8];

/*
 * =============================================================
 * FUNÇÃO: inicializarTabuleiro
 * DESCRIÇÃO: Preenche a matriz 'tabuleiro' com a posição
 * inicial de todas as peças.
 * =============================================================
 */
void inicializarTabuleiro() {
    // 1. Peças Pretas (letras minúsculas)
    tabuleiro[0][0] = 'r'; // Torre
    tabuleiro[0][1] = 'n'; // Cavalo (Knight)
    tabuleiro[0][2] = 'b'; // Bispo
    tabuleiro[0][3] = 'q'; // Rainha (Queen)
    tabuleiro[0][4] = 'k'; // Rei (King)
    tabuleiro[0][5] = 'b';
    tabuleiro[0][6] = 'n';
    tabuleiro[0][7] = 'r';
    // Peões Pretos
    for (int j = 0; j < 8; j++) {
        tabuleiro[1][j] = 'p'; // Peão (Pawn)
    }

    // 2. Casas Vazias
    // Loop 'for' aninhado para passar por todas as casas do "meio"
    for (int i = 2; i < 6; i++) { // Linhas 2, 3, 4, 5
        for (int j = 0; j < 8; j++) { // Colunas 0 a 7
            tabuleiro[i][j] = ' '; // ' ' (espaço) representa casa vazia
        }
    }

    // 3. Peças Brancas (letras maiúsculas)
    // Peões Brancos
    for (int j = 0; j < 8; j++) {
        tabuleiro[6][j] = 'P';
    }
    // Peças Principais Brancas
    tabuleiro[7][0] = 'R';
    tabuleiro[7][1] = 'N';
    tabuleiro[7][2] = 'B';
    tabuleiro[7][3] = 'Q';
    tabuleiro[7][4] = 'K';
    tabuleiro[7][5] = 'B';
    tabuleiro[7][6] = 'N';
    tabuleiro[7][7] = 'R';
}

/*
 * =============================================================
 * FUNÇÃO: imprimirTabuleiro
 * DESCRIÇÃO: "Desenha" o estado atual do tabuleiro no console.
 * =============================================================
 */
void imprimirTabuleiro() {
    // system("clear") no Linux/Mac, system("cls") no Windows.
    // Limpa a tela antes de desenhar, para dar sensação de animação.
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    printf("\n  +---+---+---+---+---+---+---+---+\n"); // Topo
    
    // Loop pelas LINHAS (i)
    for (int i = 0; i < 8; i++) {
        // Imprime o número da linha (de 8 a 1)
        printf("%d |", 8 - i); 
        
        // Loop pelas COLUNAS (j)
        for (int j = 0; j < 8; j++) {
            // Imprime a peça que está na célula [i][j]
            printf(" %c |", tabuleiro[i][j]);
        }
        
        printf("\n  +---+---+---+---+---+---+---+---+\n"); // Divisor
    }
    
    // Imprime as letras das colunas
    printf("    a   b   c   d   e   f   g   h\n\n");
}

/*
 * =============================================================
 * FUNÇÃO: eMovimentoValido
 * DESCRIÇÃO: O CÉREBRO DO JOGO. Verifica se um movimento é legal.
 * PARÂMETROS:
 * lO, cO: linha/coluna de ORIGEM
 * lD, cD: linha/coluna de DESTINO
 * turno:  0 para Brancas (Maiúsculas), 1 para Pretas (Minúsculas)
 * RETORNA:
 * 1 se o movimento for VÁLIDO
 * 0 se o movimento for INVÁLIDO
 * =============================================================
 */
int eMovimentoValido(int lO, int cO, int lD, int cD, int turno) {
    
    // Pega a peça na casa de origem
    char peca = tabuleiro[lO][cO];
    // Pega o que está na casa de destino
    char destino = tabuleiro[lD][cD];

    // --- 1. Validações Básicas (servem para todas as peças) ---

    // 1.1: Não pode mover uma casa vazia
    if (peca == ' ') {
        printf("ERRO: Voce esta tentando mover uma casa vazia.\n");
        return 0; // 0 = Inválido
    }

    // 1.2: Checar o turno. Brancas só movem Maiúsculas, Pretas só Minúsculas.
    if (turno == 0) { // Turno das Brancas
        if (peca >= 'a' && peca <= 'z') { // Se a peça for minúscula
            printf("ERRO: Turno das Brancas, mas voce moveu uma peca preta.\n");
            return 0;
        }
    } else { // Turno das Pretas
        if (peca >= 'A' && peca <= 'Z') { // Se a peça for maiúscula
            printf("ERRO: Turno das Pretas, mas voce moveu uma peca branca.\n");
            return 0;
        }
    }

    // 1.3: Não pode capturar uma peça da MESMA COR
    if (destino != ' ') { // Se o destino NÃO está vazio (é uma captura)
        if (turno == 0) { // Turno das Brancas
            if (destino >= 'A' && destino <= 'Z') {
                printf("ERRO: Brancas nao podem capturar Brancas.\n");
                return 0;
            }
        } else { // Turno das Pretas
            if (destino >= 'a' && destino <= 'z') {
                printf("ERRO: Pretas nao podem capturar Pretas.\n");
                return 0;
            }
        }
    }

    // --- 2. Lógica Específica de Cada Peça ---
    
    // Usamos tolower() para tratar 'R' e 'r' da mesma forma
    switch (tolower(peca)) {
        
        // --- LÓGICA DA TORRE (ROOK) ---
        case 'r':
            // A Torre se move em linha reta.
            // Isso significa que OU a linha de origem e destino é a mesma,
            // OU a coluna de origem e destino é a mesma.
            if (lO == lD || cO == cD) {
                // NOTA: Um jogo real precisaria checar se o CAMINHO está livre.
                // Para um iniciante, vamos pular essa verificação complexa.
                return 1; // 1 = Válido
            }
            printf("ERRO: A Torre so se move em linhas retas (vertical/horizontal).\n");
            return 0; // Se não for linha reta, é inválido

        // --- LÓGICA DO BISPO (BISHOP) ---
        case 'b':
            // O Bispo se move na diagonal.
            // A "mágica" da diagonal é que a diferença entre as linhas
            // deve ser IGUAL à diferença entre as colunas.
            // Usamos abs() (valor absoluto) para ignorar a direção.
            if (abs(lO - lD) == abs(cO - cD)) {
                // NOTA: Um jogo real também checaria o caminho do Bispo.
                return 1; // 1 = Válido
            }
            printf("ERRO: O Bispo so se move em diagonais.\n");
            return 0;

        /*
         * --- DESAFIO PARA VOCÊ ---
         * Tente implementar as outras peças aqui!
         * case 'p': // Peão
         * case 'n': // Cavalo
         * case 'q': // Rainha (dica: é a lógica da Torre + Bispo)
         * case 'k': // Rei
         */

        default:
            // Se a peça não for 'r' ou 'b', dizemos que não foi implementada.
            printf("ERRO: A logica para a peca '%c' ainda nao foi implementada.\n", peca);
            return 0;
    }
}

/*
 * =============================================================
 * FUNÇÃO: fazerMovimento
 * DESCRIÇÃO: Atualiza a matriz 'tabuleiro' com o novo movimento.
 * =============================================================
 */
void fazerMovimento(int lO, int cO, int lD, int cD) {
    // 1. Copia a peça da origem para o destino
    tabuleiro[lD][cD] = tabuleiro[lO][cO];
    
    // 2. Limpa a casa de origem, marcando-a como vazia
    tabuleiro[lO][cO] = ' ';
}

/*
 * =============================================================
 * FUNÇÃO: pausarTela
 * DESCRIÇÃO: Pausa o jogo até o usuário apertar Enter.
 * Útil para ler mensagens de erro.
 * =============================================================
 */
void pausarTela() {
    printf("Pressione [Enter] para continuar...");
    // A mágica de esperar pelo Enter:
    // getchar() lê o '\n' que sobrou do último 'fgets'
    getchar(); 
}


/*
 * =============================================================
 * =============================================================
 * * FUNÇÃO PRINCIPAL: main (Onde o jogo acontece)
 * * =============================================================
 * =============================================================
 */
int main() {
    // 1. Prepara o jogo
    inicializarTabuleiro();
    int turno = 0; // 0 = Brancas, 1 = Pretas
    char entrada[10]; // String para guardar o input (ex: "e2e4")

    // 2. O "Game Loop" - Roda indefinidamente até o jogador sair
    while (1) {
        // 2.1. Desenha o tabuleiro
        imprimirTabuleiro();

        // 2.2. Pergunta ao jogador qual seu movimento
        if (turno == 0) {
            printf("--- TURNO DAS BRANCAS ---\n");
        } else {
            printf("--- TURNO DAS PRETAS ---\n");
        }
        printf("Digite seu movimento (ex: e2e4) ou 'sair' para fechar:\n> ");

        // 2.3. Lê a entrada do usuário de forma segura
        // Usamos 'fgets' em vez de 'scanf' para evitar problemas de buffer
        if (!fgets(entrada, sizeof(entrada), stdin)) {
            break; // Se houver um erro de leitura, sai do loop
        }
        
        // Remove o '\n' (Enter) que o 'fgets' captura no final
        entrada[strcspn(entrada, "\n")] = 0;

        // 2.4. Checa se o jogador quer sair
        // 'strcmp' compara duas strings. Retorna 0 se forem iguais.
        if (strcmp(entrada, "sair") == 0) {
            printf("Obrigado por jogar! Saindo...\n");
            break; // Quebra o 'while(1)' e encerra o programa
        }

        // 2.5. Validar o formato da entrada (deve ter 4 caracteres, ex: "e2e4")
        if (strlen(entrada) != 4) {
            printf("ERRO: Formato invalido! Use 4 caracteres (ex: e2e4).\n");
            pausarTela();
            continue; // Pula para a próxima iteração do loop 'while'
        }

        /*
         * 2.6. A "MÁGICA" DA CONVERSÃO:
         * O usuário digita "e2e4", mas a matriz entende [6][4] e [4][4].
         * Precisamos "traduzir" as coordenadas.
         *
         * COLUNAS ('a' a 'h'):
         * 'a' - 'a' = 0
         * 'b' - 'a' = 1
         * ...
         * 'h' - 'a' = 7
         *
         * LINHAS ('1' a '8'):
         * '8' - '8' = 0  (Linha 8 do xadrez é a linha 0 da matriz)
         * '8' - '7' = 1  (Linha 7 do xadrez é a linha 1 da matriz)
         * ...
         * '8' - '1' = 7  (Linha 1 do xadrez é a linha 7 da matriz)
         */
        
        // Origem
        int cO = entrada[0] - 'a'; // Coluna Origem
        int lO = '8' - entrada[1]; // Linha Origem
        
        // Destino
        int cD = entrada[2] - 'a'; // Coluna Destino
        int lD = '8' - entrada[3]; // Linha Destino
        
        // 2.7. Validar o movimento chamando nossa função principal
        if (eMovimentoValido(lO, cO, lD, cD, turno)) {
            // Se for válido...
            
            // 2.7.1. Faz o movimento
            fazerMovimento(lO, cO, lD, cD);
            
            // 2.7.2. Alterna o turno
            // Esta é uma forma "inteligente" de alternar 0 e 1:
            // Se turno = 0, vira 1 - 0 = 1
            // Se turno = 1, vira 1 - 1 = 0
            turno = 1 - turno;
            
        } else {
            // Se for inválido...
            // A própria função eMovimentoValido() já imprimiu o erro.
            // Nós apenas pausamos a tela para o usuário ler.
            pausarTela();
        }
        
    } // Fim do 'while(1)'

    // 3. Fim do programa
    return 0;
}
