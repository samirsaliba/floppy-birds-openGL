#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <SFML/Audio.hpp>
#include <iostream>
using namespace std;

#include "tp1.h"

//g++ tp1.cpp -o tp1 -lGL -lGLU -lglut -lSOIL -lsfml-audio

//setando parametros dos sons
void sounds(){
  musicGRITO.openFromFile("sounds/scream.wav");
  musicGRITO.setVolume(100);
  musicGRITO.setLoop(true);
  musicJOGO.openFromFile("sounds/jogo.wav");
  musicJOGO.setVolume(100);
  musicJOGO.setLoop(true);
  musicMENU.openFromFile("sounds/menu.wav");
  musicMENU.setVolume(100);
  musicMENU.setLoop(true);
  musicLEVEL.openFromFile("sounds/levelup.wav");
  musicLEVEL.setVolume(100);
  musicLEVEL.setLoop(true);
  musicAW.openFromFile("sounds/aw.wav");
  musicAW.setVolume(100);
  musicAW.setLoop(true);
  musicPERDEU.openFromFile("sounds/perdeu.wav");
  musicPERDEU.setVolume(100);
  musicPERDEU.setLoop(true);
}

//Gera um inteirio aleatorio, exceto 0
float inteiroAleatorio(int alcance, int comeco){
  float valor = rand() % alcance + comeco;
  if(valor == 0){
    valor = valor + 1;
  }
  return valor;
}

double grauParaRadiano(double angulo) {
  return angulo * (PI/180.0);
}

//Seta as propriedades do objeto
void setarObjeto(OBJETO *obj, float x, float y, int largura, int altura, int vidas, int nivel, float aceleracao, GLuint textura, int totalFiguras, int totalLinhas){
  obj->posicao.x = x;
  obj->posicao.y = y;
  obj->largura = largura;
  obj->altura = altura;
  obj->nivel = nivel;
  obj->vidas = vidas;
  obj->angulo = 0;
  obj->velocidade.x = 0;
  obj->velocidade.y = 0;
  obj->aceleracao.x = 0;
  obj->aceleracao.y = aceleracao;
  obj->textura = textura;
  obj->sprite.totalFiguras = totalFiguras - 1;
  obj->sprite.figuraAtual = 0;
  obj->sprite.contadorFiguras = 0;
  obj->sprite.totalLinhas = totalLinhas - 1;
  obj->sprite.linhaAtual = totalLinhas - 1;
  obj->sprite.contadorLinhas = totalLinhas - 1;
}

//Animacao dos sprite sheets
//O código aceita apenas sprites com numeros iguais de figuras em cada linha
void designSpritesheet(OBJETO *obj, GLuint textura){
  //Variaveis para calcular as figuras e os vertices a serem desenhados
  int figurasPorLinha = (obj->sprite.totalFiguras + 1)/(obj->sprite.totalLinhas + 1);
  float larguraSprite = 1/(float)figurasPorLinha;
  float alturaSprite = 1/(float)(obj->sprite.totalLinhas + 1);

  //Condicoes para setar corretamente qual figura de cada linha sera imprimida
  if((float)(obj->sprite.contadorFiguras + 1)/figurasPorLinha > 1){
    if(obj->sprite.contadorFiguras%figurasPorLinha == 0){
      obj->sprite.figuraAtual = 0;
      obj->sprite.contadorLinhas--;
    }
    obj->sprite.linhaAtual = obj->sprite.contadorLinhas;
  }else{
    if((obj->sprite.figuraAtual)%figurasPorLinha == 0){
      obj->sprite.figuraAtual = 0;
      obj->sprite.contadorLinhas = obj->sprite.totalLinhas;
    }
    obj->sprite.linhaAtual = obj->sprite.contadorLinhas;
  }

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textura);
  glBegin(GL_POLYGON);
    glTexCoord2f(larguraSprite * obj->sprite.figuraAtual, alturaSprite * obj->sprite.linhaAtual); glVertex3f(obj->posicao.x, obj->posicao.y, 0);
    glTexCoord2f(obj->sprite.figuraAtual * larguraSprite + larguraSprite, alturaSprite * obj->sprite.linhaAtual); glVertex3f(obj->posicao.x + obj->largura, obj->posicao.y, 0);
    glTexCoord2f(obj->sprite.figuraAtual * larguraSprite + larguraSprite, alturaSprite * obj->sprite.linhaAtual + alturaSprite); glVertex3f(obj->posicao.x + obj->largura, obj->posicao.y + obj->altura, 0);
    glTexCoord2f(larguraSprite * obj->sprite.figuraAtual, alturaSprite * obj->sprite.linhaAtual + alturaSprite); glVertex3f(obj->posicao.x, obj->posicao.y + obj->altura, 0);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  //Muda as variaveis para quando a funcao for chamada novamente mudar a figura a ser impressa
  obj->sprite.figuraAtual++;
  obj->sprite.contadorFiguras++;
  if(obj->sprite.contadorFiguras>obj->sprite.totalFiguras){
    obj->sprite.contadorFiguras = 0;
  } 
  if(obj->sprite.contadorLinhas<0){
    obj->sprite.contadorLinhas = obj->sprite.totalLinhas-1;
  }
}

//seta o design de Objetos (não sprites)
void designObjeto(OBJETO obj, GLuint textura){
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textura);
  glBegin(GL_POLYGON);
    glTexCoord2f(0, 0); glVertex3f(obj.posicao.x, obj.posicao.y, 0);
    glTexCoord2f(1, 0); glVertex3f(obj.posicao.x + obj.largura, obj.posicao.y, 0);
    glTexCoord2f(1, 1); glVertex3f(obj.posicao.x + obj.largura, obj.posicao.y + obj.altura, 0);
    glTexCoord2f(0, 1); glVertex3f(obj.posicao.x, obj.posicao.y + obj.altura, 0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void resetarCenario(int vidas, int nivel){
  setarObjeto(&passaro, inteiroAleatorio(1400, 50), inteiroAleatorio(600, 300), LARGURA_PASSARO, ALTURA_PASSARO, vidas, nivel, ACELERACAO_GRAVIDADE, texturaPassaro, 8, 2);
  setarObjeto(&plataforma, inteiroAleatorio(1400, 50), RESPAWN_Y_PLATAFORMA, LARGURA_PLATAFORMA, ALTURA_PLATAFORMA, 0, 0, 0, texturaPlataforma, 12, 3);
  setarObjeto(&linha, 1560, 716, LARGURA_LINHA, ALTURA_LINHA, 0, 0, 0, texturaLinha, 1, 1);
  for(int j=0; j<QUANTIDADE_NUVENS; j++){
    setarObjeto(&nuvem[j], inteiroAleatorio(300, -200 + j * 300), inteiroAleatorio(400, 550), LARGURA_NUVEM, ALTURA_NUVEM, 0, 0, 0, texturaNuvem, 1, 1);
    velocidadeNuvem[j] = 2+j*0.25;
  }
  temEnergia = true;
}

//ver desenharEstrela
void setarEstrela(int idx){
  apareccer_estrela = false;
}

//ver desenharExplosao
void setarExplosao(int idx){
  explosao = false;
}

//Desenha o passaro na tela
void desenharPassaro(){
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //Caso as coordenadas ou o angulo mudem, desenha a nave em um local diferente
  glPushMatrix();
    glTranslatef(passaro.posicao.x, passaro.posicao.y, 0); //translada a nave
    glRotatef(passaro.angulo, 0.0f, 0.0f, 1.0f); // rotacionar a nave pelo eixo z
    glTranslatef(-passaro.posicao.x-(passaro.largura/2), -passaro.posicao.y-(passaro.largura/2), 0); //necessario para rotacionar a nave em torno do centro (do quadrado)
    designSpritesheet(&passaro, texturaPassaro);
  glPopMatrix();
}

//desenha Plataforma
void desenharPlataforma(){
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glPushMatrix();
      designSpritesheet(&plataforma, texturaPlataforma);
  glPopMatrix();
}

//desenha as nuvens movimentando no céu
void desenharNuvem(){
  for(int j=0; j<QUANTIDADE_NUVENS; j++){
    //caso a nuvem saia da tela, muda a direcao de movimento da nuvem
    if(nuvem[j].posicao.x > LARGURA_JANELA){
      nuvem[j].posicao.x = -LARGURA_NUVEM;
    }
    //movimento da nuvem
    nuvem[j].posicao.x = nuvem[j].posicao.x + velocidadeNuvem[j];
    designObjeto(nuvem[j], texturaNuvem);
  }
}

//desenha a explosao quando o passaro bate no chao
void desenharExplosao(){
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glPushMatrix();
    designSpritesheet(&fumaca, texturaFumaca);
  glPopMatrix();
  //Chama a funcao setarExplosao apos a animacao da explosao terminar
  glutTimerFunc(210, setarExplosao, 0);
}

void desenharCoracao(){
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glPushMatrix();
    designSpritesheet(&coracao, texturaCoracao);
  glPopMatrix();
  //Chama a funcao setarExplosao apos a animacao da explosao terminar
  glutTimerFunc(210, setarExplosao, 0);
}

void desenharEstrela(){
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glPushMatrix();
    designSpritesheet(&estrela, texturaEstrela);
  glPopMatrix();
  //Chama a funcao setarEstrela apos a animacao da explosao terminar
  glutTimerFunc(210, setarEstrela, 0);
}

int pontuacaoPorTempo(){
  if(tempoDaFase==0) return 1000;
  return 1000/tempoDaFase;
}

void musicaGrito(){
  if(contadorFramesGrito==0){
    musicGRITO.play();
  }
}

void musicaJogo(){
  if(contadorFramesJogo==0 || contadorFrames%TAMANHO_MUSICA_JOGO == 0){
    musicJOGO.play();
  }
}

void musicaMenu(){
  if(contadorFramesMenu==0 || contadorFrames%TAMANHO_MUSICA_MENU == 0){
    musicMENU.play();
  }
}

void musicaLevel(){
  if(contadorFramesLevel==0){
    musicLEVEL.play();
  }
}

void musicaAw(){
  if(contadorFramesAw==0){
    musicAW.play();
  }
}

void musicaPerdeu(){
  if(contadorFramesPerdeu==0){
    musicPERDEU.play();
  }
}

//Checa se o passaro colidiu com o chao
void checarChao(){
  if(passaro.posicao.y - (passaro.largura/2) < chao.posicao.y+chao.altura){
    passaro.vidas--;
    musicGRITO.stop();
    contadorFramesGrito = 0;
    if(passaro.vidas == 0){
      contadorFramesPerdeu = 0;
      musicaPerdeu();
      score = score + 10*(linha.posicao.x - 1380) + pontuacaoPorTempo();
      tela_atual = PERDEU;
    }else{
      contadorFramesAw = 0;
      musicaAw();
      //Desenha apenas a explosao caso explosao = true, senao desenha o passaro e a plataforma (ver desenhaCena)
      explosao = true;
      //seta a fumaça, desenha a explosao e reseta o cenario
      setarObjeto(&fumaca, passaro.posicao.x - OFFSET_FUMACA, passaro.posicao.y - OFFSET_FUMACA, LARGURA_FUMACA, ALTURA_FUMACA, 0, 0, 0, texturaFumaca, 10, 1);
      setarObjeto(&coracao, passaro.posicao.x - OFFSET_FUMACA, passaro.posicao.y + 40, LARGURA_CORACAO, ALTURA_CORACAO, 0, 0, 0, texturaCoracao, 9, 1);
      resetarCenario(passaro.vidas, passaro.nivel);
      //iguala o tempo da gravidade a 0 para que o passaro comece a cair do repouso
      tempoGravidade = 0;
    }  
  }else{
    contadorFramesAw++;
    if(contadorFramesAw==TAMANHO_MUSICA_AW){
      musicAW.stop();
    }
  }
}

//Checa quando o passarinho colide com o ninho
void checarPlataforma(){
  if((passaro.posicao.x - (passaro.largura/2) >= (plataforma.posicao.x - TOLERANCIA_X_PLATAFORMA) && passaro.posicao.x + (passaro.largura/2) <= (plataforma.posicao.x + plataforma.largura + TOLERANCIA_X_PLATAFORMA))){ //tolerancia para o passaro poder cair um pouquinho errado no eixo x
    if(passaro.posicao.y -(passaro.altura/2) < plataforma.posicao.y + plataforma.altura - OFFSET_Y_PLATAFORMA){
      if(passaro.angulo < ANGULO_DE_TORELANCIA && passaro.angulo > -ANGULO_DE_TORELANCIA){
        contadorFramesLevel = 0;
        musicaLevel();
        contadorFramesLevel++;
        apareccer_estrela = true;
        tempoGravidade = 0;
        passaro.nivel = passaro.nivel + 1;
        setarObjeto(&estrela, plataforma.posicao.x, plataforma.posicao.y + OFFSET_ALTURA, LARGURA_ESTRELA, ALTURA_ESTRELA, 0, 0, 0, texturaEstrela, 10, 2);
        desenharEstrela();
        resetarCenario(passaro.vidas, passaro.nivel);
        score = score + 2000 + (linha.posicao.x - 1380) + pontuacaoPorTempo();
        tempoDaFase = 0;
      }
    } 
  }else{
    contadorFramesLevel++;
    if(contadorFramesLevel==TAMANHO_MUSICA_LEVEL){
      musicLEVEL.stop();
    }
  }
}

//Seta a aceleracao nos eixos x e y, atribuindo o valor de cada componente associado ao angulo por trigonometria
void aceleracaoNave(){
  passaro.posicao.x += (DESLOCAMENTO * sin(-grauParaRadiano(passaro.angulo))) + passaro.velocidade.x;
  passaro.posicao.y += (DESLOCAMENTO * cos(-grauParaRadiano(passaro.angulo))) + passaro.velocidade.y;
}

//Gravidade
void gravidade(){  
  //seta aceleracoes no eixo x e y
  passaro.aceleracao.x = 0;
  passaro.aceleracao.y = ACELERACAO_GRAVIDADE;
  //Aplicacao da fórmua: v = v0 + a*t
  passaro.velocidade.x = passaro.velocidade.x + passaro.aceleracao.x * tempoGravidade;
  passaro.velocidade.y = passaro.velocidade.y + passaro.aceleracao.y * tempoGravidade;
  //Aplicacao da fórmua: s = s0 + v0 * t + (1/2)*a*t²
  passaro.posicao.x = passaro.posicao.x + passaro.velocidade.x * tempoGravidade + (0.5 * passaro.aceleracao.x * pow(tempoGravidade, 2));
  passaro.posicao.y = passaro.posicao.y + passaro.velocidade.y * tempoGravidade + (0.5 * passaro.aceleracao.y * pow(tempoGravidade, 2));
  tempoGravidade++;
  /*
    Caso o tempo cresça muito, a aceleracao da nave ficara absurdamente alta quando segurado W (já que esta elevada ao quadrado)
    Assim, colocando um tempo máximo limite, a jogabilidade fica muito melhor
  */
  if(tempoGravidade > 25){
    tempoGravidade = 25;
  }
}

//Propusao do passaro. Quando voar é chamada, gravidade NÃO é chamada, e vice-versa
void voar(){
  //seta aceleracoes no eixo x e y, combinando gravidade e propulsao
  passaro.aceleracao.x =  ACELERACAO_MOTOR * sin(-grauParaRadiano(passaro.angulo));
  passaro.aceleracao.y =  ACELERACAO_GRAVIDADE + ACELERACAO_MOTOR * cos(-grauParaRadiano(passaro.angulo));
  //Aplicacao da fórmua: v = v0 + a*t
  passaro.velocidade.x =  passaro.velocidade.x + passaro.aceleracao.x * tempoMotor;
  passaro.velocidade.y =  passaro.velocidade.y + passaro.aceleracao.y * tempoMotor;
  //Aplicacao da fórmua: s = s0 + v0 * t + (1/2)*a*t²
  passaro.posicao.x = passaro.posicao.x + passaro.velocidade.x * tempoMotor + (0.5 * passaro.aceleracao.x * pow(tempoMotor, 2));
  passaro.posicao.y = passaro.posicao.y + passaro.velocidade.y * tempoMotor + (0.5 * passaro.aceleracao.y * pow(tempoMotor, 2));
  tempoMotor++;
  /*
    Caso o tempo cresça muito, a aceleracao da nave ficara absurdamente alta quando segurado W (já que esta elevada ao quadrado)
    Assim, colocando um tempo máximo limite, a jogabilidade fica muito melhor
  */
  if(tempoMotor > 25){
    tempoMotor = 25;
  }  
}

void energia(OBJETO *obj){
  obj->posicao.x = obj->posicao.x - 0.2 * passaro.nivel;
  if(obj->posicao.x <= 1380){
    obj->posicao.x = 1380;
    temEnergia = false;
  }
}

//Funcao de impressao que sera chamada pelo hud
void imprimir(int x, int y, int z, char *string){

  glColor3f(0, 0, 0);
  //seta a posicao do texto com as cordenadas passadas
  glRasterPos2f(x,y);
  //pega o comprimento da string pra ser mostrada
  int len = (int) strlen(string);
  //entra em loop pra escrever caractere por caractere
  for (int i = 0; i < len; i++){
      glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
  }
  glColor3f(1, 1, 1);
}

//Impressao na tela, chama imprimir 
void hud(){
  char buffer [50];
  int n;
  char *A;
  const int VALOR_PARA_APARECER_BONITO_NA_TELA = 100;

  //imprime o tempo, altura, energia
  n = sprintf (buffer, "Tempo: %d:%.d", tempoTotalMinutos, tempoTotalSegundos);
  A = (char*)buffer;
  imprimir(1380, 840, 2, A);

  n = sprintf (buffer, "Altura: %.1f", passaro.posicao.y - chao.altura - passaro.altura/2);
  A = (char*)buffer;
  imprimir(1380, 800, 2, A);

  n = sprintf (buffer, "Energia:");
  A = (char*)buffer;
  imprimir(1380, 760, 2, A);

  // imprime a velocidade e aceleracao
  n = sprintf (buffer, "Velocidade: %.1f", VALOR_PARA_APARECER_BONITO_NA_TELA*(pow(pow(passaro.velocidade.x, 2) + pow(passaro.velocidade.y, 2), 0.5)));
  A = (char*)buffer;
  imprimir(1380, 680, 2, A);

  n = sprintf (buffer, "Aceleracao: %.3f", VALOR_PARA_APARECER_BONITO_NA_TELA*(pow(pow(passaro.aceleracao.x, 2) + pow(passaro.aceleracao.y, 2), 0.5)));
  A = (char*)buffer;
  imprimir(1380, 640, 2, A);

  // imprime level, score e vidas
  n = sprintf (buffer, "Level %d", passaro.nivel);
  A = (char*)buffer;
  imprimir(50, 840, 2, A);

  n = sprintf (buffer, "Score: %d", score);
  A = (char*)buffer;
  imprimir(50, 800, 2, A);

  n = sprintf (buffer, "Vidas: ");
  A = (char*)buffer;
  imprimir(50, 760, 2, A);
      
  if(passaro.vidas == 1){
    designObjeto(vida, texturaVida);
  }

  if(passaro.vidas == 2){
    designObjeto(vida, texturaVida);
    designObjeto(vida2, texturaVida);
  }

  if(passaro.vidas == 3){
    designObjeto(vida, texturaVida);
    designObjeto(vida2, texturaVida);
    designObjeto(vida3, texturaVida);
  }
}

void desenharCena(void){
  glClear(GL_COLOR_BUFFER_BIT);
  switch(tela_atual){
    case INTRO:
      designObjeto(imagem, texturaIntroducao);
      break;
    case MENU:
      designObjeto(imagem, texturaMenu);
      break;
    case TUTORIAL:
      designObjeto(imagem, texturaTutorial);
      break;
    case PAUSE:
      designObjeto(imagem, texturaPause);
      break;
    case JOGO:
      designObjeto(fundo, texturaFundo);
      desenharNuvem();
      designObjeto(barra, texturaBarra);
      designObjeto(linha, texturaLinha);
      if(explosao){
        desenharExplosao();
        desenharCoracao();
      }else{
        desenharPassaro();
        desenharPlataforma();
        if(apareccer_estrela){
          desenharEstrela();
        }
      }
      hud();
      break;
    case PERDEU:
      designObjeto(imagem, texturaPerdeu);
      char buffer [50];
      int n;
      char *A;
      {
        n = sprintf (buffer, "Score: %.d", score);
        A = (char*)buffer;
        glColor3f(1, 1, 1);
        imprimir(500, 490, 2, A);
      }
      {
        n = sprintf (buffer, "Level %.d", passaro.nivel);
        A = (char*)buffer;
        glColor3f(1, 1, 1);
        imprimir(500, 465, 2, A);
      }
      break;
  }
  glFlush();
}

void atualiza(int idx){
  contadorFrames++;
  if(contadorFrames % 40 == 0){//40 equivale a 1 segundo, ja que é chamada a 25 ms
    contadorTempo++; 
    tempoTotalSegundos++;
    tempoDaFase++;
    if(tempoTotalSegundos > 59){
      tempoTotalMinutos++;
      tempoTotalSegundos = 0;
    }
  }

  switch(tela_atual){
    case INTRO:
      contadorFramesJogo = 0;
      if(contadorTempo > 2){ 
        tela_atual = MENU;
        contadorFrames = 0;
        contadorTempo = 0;
      }
      break;  
    case TUTORIAL:
      contadorFramesMenu++;
      break;
    case MENU:
      contadorFramesJogo = 0;
      musicaMenu();
      contadorFramesMenu++;
      break;
    case JOGO:
      musicMENU.stop();
      musicPERDEU.stop();
      contadorFramesMenu = 0;
      musicaJogo();
      contadorFramesJogo++;
      hud();
      checarPlataforma();
      checarChao();
      if(teclas['w']){
        energia(&linha);
        if(temEnergia){
          voar();
        }else{
          musicaGrito();
          contadorFramesGrito++;
          gravidade();
          passaro.angulo += 8;
        }
      }else{
        gravidade();
        if(!temEnergia){
          passaro.angulo += 8;
        }
      }
      break;
    case PAUSE:
      musicJOGO.pause();
      musicGRITO.stop();
      musicLEVEL.stop();
      musicAW.stop();
      musicPERDEU.stop();
      contadorFramesJogo=0;
      contadorFramesMenu = 0;
      break;
    case PERDEU:
      musicJOGO.stop();
      musicMENU.stop();
      contadorFramesJogo = 0;
      contadorFramesMenu = 0;
      contadorFramesPerdeu++;
      if(contadorFramesPerdeu == TAMANHO_MUSICA_PERDEU){
        musicPERDEU.stop();
      }
      break;   
  }
  glutPostRedisplay();
  glutTimerFunc(25, atualiza, 0);
}

//Seta a cor do fundo (limpa) e seta a propriedades da nave
void inicializa(void){
  glClearColor(0, 0, 0, 0);
  glEnable(GL_BLEND );
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  texturaPassaro = SOIL_load_OGL_texture(
    "img/bird.png",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y
  );      
  if(texturaPassaro == 0) {
    cout << "Erro do SOIL (nave): " << SOIL_last_result() << endl;
  }

  texturaFumaca = SOIL_load_OGL_texture(
    "img/smoke.png",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y
  );      
  if(texturaPassaro == 0) {
    cout << "Erro do SOIL (smoke): " << SOIL_last_result() << endl;
  }
  
  texturaBarra = SOIL_load_OGL_texture(
    "img/barra.png",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y
  );      
  if(texturaBarra == 0) {
    cout << "Erro do SOIL (barra): " << SOIL_last_result() << endl;
  }

  texturaLinha = SOIL_load_OGL_texture(
    "img/linha.png",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y
  );      
  if(texturaLinha == 0) {
    cout << "Erro do SOIL (linha): " << SOIL_last_result() << endl;
  }

  texturaVida = SOIL_load_OGL_texture(
    "img/vida.png",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y
  );      
  if(texturaVida == 0) {
    cout << "Erro do SOIL (vida): " << SOIL_last_result() << endl;
  }

  texturaPlataforma = SOIL_load_OGL_texture(
    "img/egg-sprite.png",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y
  );      
  if(texturaPlataforma == 0) {
    cout << "Erro do SOIL (plataforma): " << SOIL_last_result() << endl;
  }

  texturaNuvem = SOIL_load_OGL_texture(
    "img/cloud.png",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y
  );      
  if(texturaNuvem == 0) {
    cout << "Erro do SOIL (nuvem): " << SOIL_last_result() << endl;
  }

  texturaEstrela = SOIL_load_OGL_texture(
    "img/star.png",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y
  );      
  if(texturaEstrela == 0) {
    cout << "Erro do SOIL (estrela): " << SOIL_last_result() << endl;
  }

  texturaCoracao = SOIL_load_OGL_texture(
    "img/coracao.png",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y
  );      
  if(texturaCoracao == 0) {
    cout << "Erro do SOIL (coracao): " << SOIL_last_result() << endl;
  }
      
  texturaFundo = SOIL_load_OGL_texture(
    "img/background.png",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y
  );      
  if(texturaFundo == 0) {
    cout << "Erro do SOIL (fundo): " << SOIL_last_result() << endl;
  }
    
  texturaIntroducao = SOIL_load_OGL_texture(
    "img/introducao.png",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y
  );      
  if(texturaIntroducao == 0) {
    cout << "Erro do SOIL (introducao): " << SOIL_last_result() << endl;
  }

  texturaPause = SOIL_load_OGL_texture(
    "img/pause.png",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y
  );      
  if(texturaPause == 0) {
    cout << "Erro do SOIL (pause): " << SOIL_last_result() << endl;
  }

  texturaTutorial = SOIL_load_OGL_texture(
    "img/tutorial.png",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y
  );      
  if(texturaTutorial == 0) {
    cout << "Erro do SOIL (tutorial): " << SOIL_last_result() << endl;
  }
    
  texturaMenu = SOIL_load_OGL_texture(
    "img/menu.png",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y
  );      
  if(texturaMenu == 0) {
    cout << "Erro do SOIL (menu): " << SOIL_last_result() << endl;
  }
    
  texturaPerdeu = SOIL_load_OGL_texture(
    "img/perdeu.png",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y
  );      
  if(texturaPerdeu == 0) {
    cout << "Erro do SOIL (perdeu): " << SOIL_last_result() << endl;
  }

  setarObjeto(&fundo, 0, 0, LARGURA_JANELA, ALTURA_JANELA, 0, 0, 0, texturaFundo, 1, 1);
  setarObjeto(&chao, 0, 0, LARGURA_JANELA, ALTURA_CHAO, 0, 0, 0, texturaNULL, 1, 1);
  setarObjeto(&imagem, 350, 80, LADO_IMAGENS, LADO_IMAGENS, 0, 0, 0, texturaNULL, 1, 1);
  setarObjeto(&barra, 1380, 720, LARGURA_BARRA, ALTURA_BARRA, 0, 0, 0, texturaBarra, 1, 1);
  setarObjeto(&vida, 50, 720, LADO_CORACAO, LADO_CORACAO, 0, 0, 0, texturaVida, 1, 1);
  setarObjeto(&vida2, 80, 720, LADO_CORACAO, LADO_CORACAO, 0, 0, 0, texturaVida, 1, 1);
  setarObjeto(&vida3, 110, 720, LADO_CORACAO, LADO_CORACAO, 0, 0, 0, texturaVida, 1, 1);
}

void redimensiona(int w, int h){
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, LARGURA_JANELA, 0, ALTURA_JANELA, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void keyUp(unsigned char key, int x, int y) {  
  teclas[key] = false; // Seta o estado da atual tecla nao pressionada 
  //por algum motivo, quando o Caps Lock estava ativado e apertava-se W maiusculo, w minusculo continuava como true
  if(key=='W') 
    teclas['w'] = false; 
} 

void teclado(unsigned char key, int x, int y){
  switch(key){
    // Tecla ESC
    case 27:
      if(tela_atual == PAUSE || tela_atual == PERDEU){
          exit(0); //so sai do jogo se ja estiver em pause ou na tela de perdeu
      }
      else {  tela_atual = PAUSE; }//se nao estiver em pause, entra em pause primeiro 
      break;
    case 'P':
    case 'p':
      if(tela_atual == PAUSE){ // se estiver em pause volta pro jogo
        tela_atual = JOGO;
      }else if(tela_atual == JOGO){ //se estiver no jogo entra em pause
          tela_atual = PAUSE;
      }
      break;
    case 'R':
    case 'r':
      if(tela_atual == JOGO){ // se estiver em pause volta pro jogo
        tela_atual = PAUSE;
      }else{
        musicJOGO.play();
        score = 0;
        resetarCenario(VIDAS_INICIAIS, COINS_INICIAIS);
        tela_atual = JOGO;
      }
      break;
    case 'T':
    case 't':
      tela_atual = TUTORIAL;
      break;
    case 'W':
    case 'w':
      teclas['w']=true;
      break;
    case 'A':
    case 'a':
      teclas['a']=true;
      passaro.angulo+=8;
      break;
    case 'D':
    case 'd':
      teclas['d']=true;
      passaro.angulo-=8;
      break;
    default:
      break;
  }
  if(passaro.angulo % 360 == 0){
  	passaro.angulo = 0;
  }
}

// rotina principal
int main(int argc, char **argv){
  srand(time(NULL));
  // Acordando o GLUT
  glutInit(&argc, argv);

  // Definindo a versao do OpenGL que vamos usar
  glutInitContextVersion(1, 1);
  glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

  // Configuracao inicial da janela do GLUT
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
  glutInitWindowSize(LARGURA_JANELA, ALTURA_JANELA);
  glutInitWindowPosition(100, 100);

  // Abre a janela
  glutCreateWindow("FLOPPY BIRDS");
  //Sons
  sounds();
  //inicializa
  inicializa();
  // Registra callbacks para alguns eventos
  glutDisplayFunc(desenharCena);
  glutReshapeFunc(redimensiona);
  glutKeyboardFunc(teclado);
  glutKeyboardUpFunc(keyUp);
  // Registra as funcoes com timer, gravidade e checar o chao
  glutTimerFunc(25, atualiza, 0);

  // Entra em loop e nunca sai
  glutMainLoop();
  return 0;
}