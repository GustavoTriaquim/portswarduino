#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int botaoAlt = 7;
const int botaoNOT = 6;

const int botao1 = 10;
const int botao2 = 9;
const int botao3 = 8;

const int led = 13;

int menuAtual = 0;
int opcaoAtual = -1;
bool modoVisualizacao = false;

void setup() {
  lcd.begin(16, 2);

  pinMode(botaoAlt, INPUT_PULLUP);
  pinMode(botaoNOT, INPUT_PULLUP);

  pinMode(botao1, INPUT_PULLUP);
  pinMode(botao2, INPUT_PULLUP);
  pinMode(botao3, INPUT_PULLUP);

  pinMode(led, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  static int menuAnterior = -1;

  if (!modoVisualizacao) {
    if (menuAtual != menuAnterior) {
      lcd.clear();
      lcd.setCursor(0, 0);

      if (menuAtual == 0) lcd.print("AND/OR/XOR");
      else if (menuAtual == 1) lcd.print("NAND/NOR/XNOR");
      else if (menuAtual == 2) lcd.print("NOT");

      menuAnterior = menuAtual;
      opcaoAtual = -1;
    }
    lcd.setCursor(0, 1);

    int novaOpcao = -1;
    if (digitalRead(botao1) == HIGH) novaOpcao = 1;
    if (digitalRead(botao2) == HIGH) novaOpcao = 2;
    if (digitalRead(botao3) == HIGH) novaOpcao = 3;

    if (novaOpcao != -1 && novaOpcao != opcaoAtual) {
      opcaoAtual = novaOpcao;
      lcd.clear();
      lcd.setCursor(0, 0);

      if (menuAtual == 0) {
        lcd.print("AND/OR/XOR");
        lcd.setCursor(0, 1);

        if (opcaoAtual == 1) lcd.print("AND");
        if (opcaoAtual == 2) lcd.print("OR");
        if (opcaoAtual == 3) lcd.print("XOR");
      } else if (menuAtual == 1) {
        lcd.print("NAND/NOR/XNOR");
        lcd.setCursor(0, 1);

        if (opcaoAtual == 1) lcd.print("NAND");
        if (opcaoAtual == 2) lcd.print("NOR");
        if (opcaoAtual == 3) lcd.print("XNOR");
      } else if (menuAtual == 2) {
        lcd.print("NOT");
        lcd.setCursor(0, 1);

        if (opcaoAtual == 1) lcd.print("NOT");
        if (opcaoAtual == 2) lcd.print("NOT");
        if (opcaoAtual == 3) lcd.print("NOT");
      }

      delay(500);
      modoVisualizacao = true;
    }

    if (digitalRead(botaoNOT) == HIGH) {
      menuAtual = 2;
      delay(300);
    } else if (digitalRead(botaoAlt) == HIGH) {
      menuAtual = (menuAtual == 0) ? 1 : 0;
      delay(300);
    }
  } else {
    bool A = digitalRead(botaoAlt) == HIGH;
    bool B = digitalRead(botaoNOT) == HIGH;
    bool resultado = false;

    lcd.setCursor(0, 1);

    if (menuAtual == 0) {
      if (opcaoAtual == 1) {
        resultado = A && B;
        lcd.print("AND ");
      } else if (opcaoAtual == 2) {
        resultado = A || B;
        lcd.print("OR  ");
      } else if (opcaoAtual == 3) {
        resultado = A ^ B;
        lcd.print("XOR ");
      }
      lcd.print(A);
      lcd.print(" ");
      lcd.print(B);
      lcd.print(" ");
      lcd.print(resultado);
    } else if (menuAtual == 1) {
      if (opcaoAtual == 1) {
        resultado = !(A && B);
        lcd.print("NAND");
        lcd.print(A);
        lcd.print(" ");
        lcd.print(B);
        lcd.print(" ");
        lcd.print(resultado);
      } else if (opcaoAtual == 2) {
        resultado = !(A || B);
        lcd.print("NOR ");
        lcd.print(A);
        lcd.print(" ");
        lcd.print(B);
        lcd.print(" ");
        lcd.print(resultado);
      } else if (opcaoAtual == 3) {
        resultado = !(A ^ B);
        lcd.print("XNOR");
        lcd.print(A);
        lcd.print(" ");
        lcd.print(B);
        lcd.print(" ");
        lcd.print(resultado);
      }
    } else if (menuAtual == 2) {
      resultado = !A;
      lcd.print("NOT ");
      lcd.print(A);
      lcd.print(" ");
      lcd.print(resultado);
    }

    digitalWrite(led, resultado ? HIGH : LOW);

    String lcdData = "[LCD]" + String(menuAtual) + "|" + String(opcaoAtual) + "|" + String(A) + "|-|" + String(resultado);
    Serial.println(lcdData);

    if (digitalRead(botao1) == HIGH || digitalRead(botao2) == HIGH || digitalRead(botao3) == HIGH) {
      modoVisualizacao = false;
      delay(500);
    }
  }

  delay(100);
}
