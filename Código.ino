#include <PS2X_lib.h>
#include <Servo.h>

PS2X ps2x;
Servo servo1, servo2, servo3, servo4;

#define PS2_ATT 6
#define PS2_CLK 7 
#define PS2_CMD 8
#define PS2_DAT 9   

#define DL 12
#define DR 13
#define PMW_L 10
#define PMW_R 11

int error = 0;
byte type = 0;
byte vibrate = 0;

struct ButtonInfo {
  int button;
  const char* name;
};

ButtonInfo buttons[] = {
  {PSB_START, "START"},
  {PSB_SELECT, "SELECT"},
  {PSB_PAD_UP, "Seta cima"},
  {PSB_PAD_DOWN, "Seta baixo"},
  {PSB_PAD_LEFT, "Seta esquerda"},
  {PSB_PAD_RIGHT, "Seta direita"},
  {PSB_TRIANGLE, "Triângulo"},
  {PSB_CROSS, "X"},
  {PSB_SQUARE, "Quadrado"},
  {PSB_CIRCLE, "Círculo"},
  {PSB_L1, "L1"},
  {PSB_L2, "L2"},
  {PSB_R1, "R1"},
  {PSB_R2, "R2"},
  {PSB_L3, "L3"},
  {PSB_R3, "R3"}
};

// Função para movimentar os servos
void moveServo4(int angle) {
  servo4.write(angle);
  Serial.print("Servo em ");
  Serial.print(angle);
  Serial.println(" graus");
}
void moveServo3(int angle) {
  servo3.write(angle);
  Serial.print("Servo em ");
  Serial.print(angle);
  Serial.println(" graus");
}
void moveServo2(int angle) {
  servo2.write(angle);
  Serial.print("Servo em ");
  Serial.print(angle);
  Serial.println(" graus");
}
void moveServo1(int angle) {
  servo1.write(angle);
  Serial.print("Servo em ");
  Serial.print(angle);
  Serial.println(" graus");
}

// Função para controlar os motores
void moveMotor(bool leftDir, bool rightDir, bool enable) {
  digitalWrite(DL, leftDir);
  digitalWrite(DR, rightDir);
  digitalWrite(PMW_L, enable);
  digitalWrite(PMW_R, enable);
}
void moveOneMotor(char Motor, char PMW, bool Direcao, bool enable) {
  digitalWrite(Motor, Direcao);
  digitalWrite(PMW, enable);
}

void setup() {
  Serial.begin(9600);

  servo1.attach(4); //articulação
  servo2.attach(2); //pinça direita
  servo3.attach(3); //pinça esquerda
  servo4.attach(5); //braço

  pinMode(DL, OUTPUT);
  pinMode(DR, OUTPUT);
  pinMode(PMW_L, OUTPUT);
  pinMode(PMW_R, OUTPUT);

  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_ATT, PS2_DAT);

  if (error == 0) {
    Serial.println("Controle PS2 conectado com sucesso!");
  } else {
    Serial.print("Erro ao conectar controle PS2. Código de erro: ");
    Serial.println(error);
    return;
  }

  type = ps2x.readType();
  switch (type) {
    case 0: Serial.println("Controle desconhecido"); break;
    case 1: Serial.println("DualShock encontrado"); break;
    case 2: Serial.println("GuitarHero encontrado"); break;
    case 3: Serial.println("Wireless DualShock encontrado"); break;
  }
}

void loop() {
  //if (error != 0) return;

  ps2x.read_gamepad(false, vibrate);

  int LX = ps2x.Analog(PSS_LX);
  int LY = ps2x.Analog(PSS_LY);
  int RX = ps2x.Analog(PSS_RX);
  int RY = ps2x.Analog(PSS_RY);

  Serial.print("LX: "); Serial.print(LX);
  Serial.print(" | LY: "); Serial.print(LY);
  Serial.print(" || RX: "); Serial.print(RX);
  Serial.print(" | RY: "); Serial.println(RY);

  // Verifica botões pressionados
  for (int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++) {
    if (ps2x.Button(buttons[i].button)) {
      Serial.print(buttons[i].name);
      Serial.println(" pressionado");
    }
  }
  
  //------------AUTÔNOMO------------

  //Coleta uma esfera
  if(ps2x.Button(PSB_SELECT) && ps2x.Button(PSB_CIRCLE)) {
    moveMotor(true, true, true);
    delay(3000);
    moveMotor(false, false, false);
  }

  //Coleta um quadrado
  if(ps2x.Button(PSB_SELECT) && ps2x.Button(PSB_SQUARE)) {
    moveMotor(true, true, true);
    delay(2200);
    moveMotor(false, true, true);
    delay(900);
    moveMotor(true, true, true);
    delay(500);
    moveMotor(false, true, true);
    delay(200);
    moveMotor(true, true, true);
    delay(1500);
    moveMotor(false, true, true);
    delay(250);
    moveMotor(true, true, true);
    delay(500);
    moveMotor(false, false, false);
  }

  //Pontua dois quadrados e uma esfera
  if(ps2x.Button(PSB_SELECT) && ps2x.Button(PSB_TRIANGLE)) {
    moveServo2(160);
    moveServo3(30);
    delay(200);
    moveMotor(true, true, true);
    delay(1600);
    moveMotor(true, false, true);
    delay(600);
    moveMotor(true, true, true);
    delay(1300);
    moveMotor(false, false, false);
    delay(200);
    moveMotor(false, true, true);
    delay(550);
    moveMotor(false, false, false);
    delay(400);
    moveMotor(true, true, true);
    delay(500);
    moveMotor(false, false, false);
    delay(200);
    moveServo2(160);
    moveServo3(30);
    delay(1000);
    moveServo4(150);
    moveServo1(30);
    delay(2000);
    moveServo2(80);
    moveServo3(110);
    delay(1000);
    moveServo4(0);
    moveServo1(95);
    moveMotor(false, false, true);
    delay(3000);
    moveMotor(false, false, false);
  }

  if(ps2x.Button(PSB_SELECT) && ps2x.Button(PSB_CROSS)) {
    moveServo2(80);
    moveServo3(110);
    delay(200);
    moveMotor(false, false, true);
    delay(2500);
    moveMotor(false, false, false);
    delay(200);
    moveServo2(160);
    moveServo3(30);
    delay(200);
    moveMotor(true, true, true);
    delay(2000);
    moveMotor(false, true, true);
    delay(300);
    moveMotor(true, true, true);
    delay(1100);
    moveMotor(false, false, false);
  }


  //------------TELE-OP-------------

  // Movimentação frente e trás
  if (LY <= 126)
    moveMotor(LOW, LOW, true);  // Frente
  else if (LY >= 129)
    moveMotor(HIGH, HIGH, true);    // Ré
  else
    moveMotor(LOW, LOW, false);   // Parado

  // Movimentação giro
  if (RX >= 129)
    moveMotor(LOW, HIGH, true);   // Direita
  else if (RX <= 126)
    moveMotor(HIGH, LOW, true);   // Esquerda

  // Servo da pinça direita
  if (ps2x.ButtonPressed(PSB_L1)) {
    moveServo2(160);
  }    
  if (ps2x.ButtonPressed(PSB_L2)) {
    moveServo2(80);
  }   

  // Servo da pinça esquerda
  if (ps2x.ButtonPressed(PSB_R1)) {
    moveServo3(30);
  }    
  if (ps2x.ButtonPressed(PSB_R2)) {
    moveServo3(110);
  }   

  // Servos do braço
  if (ps2x.ButtonPressed(PSB_CROSS)) {
    moveServo4(0);
    moveServo1(95);
  }    
  if (ps2x.ButtonPressed(PSB_TRIANGLE)) {
    moveServo4(180);
    moveServo1(30);
  } 
  if(ps2x.ButtonPressed(PSB_SQUARE)) {
    moveServo4(60);
    moveServo1(120);
  }
  if(ps2x.ButtonPressed(PSB_CIRCLE)) {
    moveServo4(49);
    moveServo1(140);
  }

  if(ps2x.ButtonPressed(PSB_PAD_UP)) {
    moveServo4(0);
    moveServo1(0);
    moveServo2(160);
    moveServo3(30);
  }

  delay(200);
}
