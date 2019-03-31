//declarando as variaveis globais, estruturas e etc

bool teclas[256];

//Janela
const int LARGURA_JANELA = 1600;
const int ALTURA_JANELA = 900;

//Telas
const int LADO_IMAGENS = 830;

//Passaro
const int RESPAWN_X_PASSARO = 50;
const int RESPAWN_Y_PASSARO = 900;
const int LARGURA_PASSARO = 75;
const int ALTURA_PASSARO = 50;

//Plataforma (ninho)
const int RESPAWN_Y_PLATAFORMA = 125;
const int LARGURA_PLATAFORMA = 120;
const int ALTURA_PLATAFORMA = 83;

//Chao
const int ALTURA_CHAO = 125;

//Barra de Energia
const int LARGURA_BARRA = 185;
const int ALTURA_BARRA = 19;
bool temEnergia;

//Coracoes do numero de vidas
const int LADO_CORACAO = 20;

//Coracao quando perde vida
const int LARGURA_CORACAO = 50;
const int ALTURA_CORACAO = 47;

//Estrela quando passa de nivel
const int LARGURA_ESTRELA = 50;
const int ALTURA_ESTRELA = 47;
const int OFFSET_ALTURA = 40;
bool apareccer_estrela = false;

//Linha da barra de energia
const int LARGURA_LINHA = 5;
const int ALTURA_LINHA = 28;

//Fumaca quando destrói
const int LARGURA_FUMACA = 75;
const int ALTURA_FUMACA = 75;
bool explosao = false;

//Nuvens
const int LARGURA_NUVEM = 250;
const int ALTURA_NUVEM = 141;
const int QUANTIDADE_NUVENS = 6;
float velocidadeNuvem[QUANTIDADE_NUVENS];

//Constantes e variaveis para movimentos
const float ACELERACAO_GRAVIDADE = -0.0005;
const float ACELERACAO_MOTOR = 0.0015;
int tempoGravidade=0;
int tempoMotor=0;

//Outras constantes
const int VIDAS_INICIAIS = 3;
const int COINS_INICIAIS = 1;
const double PI = 3.14159265358979323846;
const int DESLOCAMENTO = 5;
const int OFFSET_FUMACA = 30;
const int TOLERANCIA_X_PLATAFORMA = 5;
const int OFFSET_Y_PLATAFORMA = 40;
const int ANGULO_DE_TORELANCIA = 40;

//Tempo
int tempoTotalSegundos = 0;
int tempoTotalMinutos = 0;
int tempoDaFase;

//Contadores
int contadorTempo = 0; 
int contadorFrames = 0;

//musicas
const int TAMANHO_MUSICA_MENU = 2160;
const int TAMANHO_MUSICA_JOGO = 480;
const int TAMANHO_MUSICA_LEVEL = 36;
const int TAMANHO_MUSICA_AW = 25;
const int TAMANHO_MUSICA_PERDEU = 70;
int contadorFramesGrito = 0;
int contadorFramesMenu = 0;
int contadorFramesJogo = 0;
int contadorFramesLevel = 0;
int contadorFramesAw = 0;
int contadorFramesPerdeu = 0;

//Pontuacao
int score = 0;

enum TELA {INTRO, MENU, TUTORIAL, PAUSE, JOGO, PERDEU};
TELA tela_atual = INTRO;

//Texturas - Objetos
GLuint texturaPassaro;
GLuint texturaVida;
GLuint texturaPlataforma;
GLuint texturaChao;
GLuint texturaFundo;
GLuint texturaFumaca;
GLuint texturaNuvem;
GLuint texturaBarra;
GLuint texturaLinha;
GLuint texturaEstrela;
GLuint texturaCoracao;
//Texturas - Telas
GLuint texturaIntroducao;
GLuint texturaMenu;
GLuint texturaTutorial;
GLuint texturaPause;
GLuint texturaPerdeu;
GLuint texturaNULL;


//Structs
typedef struct {
    float x;
    float y;
} COORDENADAS;

typedef struct {
    int totalFiguras;
    int figuraAtual;
    int contadorFiguras;
    int totalLinhas;
    int linhaAtual;
    int contadorLinhas;
} SPRITESHEET;

typedef struct {
    int largura;
    int altura;
    int angulo;
    COORDENADAS posicao;
    COORDENADAS velocidade;
    COORDENADAS aceleracao;
    int nivel;
    int vidas;
    GLuint textura;
    SPRITESHEET sprite;
} OBJETO;

//Objetos globais
OBJETO passaro;
OBJETO plataforma;
OBJETO barra;
OBJETO linha;
OBJETO chao;
OBJETO fundo;
OBJETO fumaca;
OBJETO nuvem[QUANTIDADE_NUVENS];
OBJETO imagem;
OBJETO vida, vida2, vida3;
OBJETO estrela;
OBJETO coracao;

sf::Music musicGRITO, musicJOGO, musicMENU, musicLEVEL, musicAW, musicPERDEU;

void sounds();

float inteiroAleatorio(int alcance, int comeco);

double grauParaRadiano(double angulo);

void setarObjeto(OBJETO *obj, float x, float y, int largura, int altura, int vidas, int coins, float aceleracao, GLuint textura, int totalFiguras, int totalLinhas);

void designSpritesheet(OBJETO *obj, GLuint textura);

void designObjeto(OBJETO obj, GLuint textura);

void resetarCenario(int vidas, int coins);

void setarEstrela(int idx);

void setarExplosao(int idx);

void desenharPassaro();

void desenharPlataforma();

void desenharNuvem();

void desenharExplosao();

void desenharCoracao();

void desenharEstrela();

int pontuacaoPorTempo();

void musicaGrito();

void musicaJogo();

void musicaMenu();

void musicaLevel();

void musicaAw();

void musicaPerdeu();

void checarChao();

void checarPlataforma();

void aceleracaoNave();

void gravidade();

void voar();

void energia(OBJETO *obj);

void imprimir(int x, int y, int z, char *string);

void hud();

void desenharCena(void);

void atualiza(int idx);

void inicializa(void);

void redimensiona(int w, int h);

void keyUp (unsigned char key, int x, int y);

void teclado(unsigned char key, int x, int y);