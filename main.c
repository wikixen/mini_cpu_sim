#include <LiquidCrystal.h>

int buttonPin = 7;
int buttonState;
int lastButtonState = HIGH;

int resetPin = 6;
int resetState;
int lastResetState = HIGH;

int R0 = 0;
int R1 = 0;
int PC = 0;

#define LOAD 0
#define ADD 1
#define SUB 2

struct Instruction
{
  int opcode;
  int rd;
  int rs;
  int imm;
};

Instruction program[] = {
    {LOAD, 0, 0, 5}, // R0 = 5
    {LOAD, 1, 0, 3}, // R1 = 3
    {ADD, 0, 1, 0},  // R0 = R0 + R1
    {SUB, 0, 1, 0}   // R0 = R0 - R1
};

const int programSize = 4;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void showState(String instrText)
{
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("PC:");
  lcd.print(PC);
  lcd.print(" ");
  lcd.print(instrText);

  lcd.setCursor(0, 1);
  lcd.print("R0:");
  lcd.print(R0);
  lcd.print(" R1:");
  lcd.print(R1);
}

void resetCPU()
{
  R0 = 0;
  R1 = 0;
  PC = 0;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CPU Reset");
  lcd.setCursor(0, 1);
  lcd.print("R0:0 R1:0");
}

void stepCPU()
{
  if (PC >= programSize)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Program done");
    lcd.setCursor(0, 1);
    lcd.print("R0:");
    lcd.print(R0);
    lcd.print(" R1:");
    lcd.print(R1);
    return;
  }

  Instruction instr = program[PC];
  String instrText = "";

  switch (instr.opcode)
  {
  case LOAD:
    if (instr.rd == 0)
    {
      R0 = instr.imm;
      instrText = "LOAD R0," + String(instr.imm);
    }
    else if (instr.rd == 1)
    {
      R1 = instr.imm;
      instrText = "LOAD R1," + String(instr.imm);
    }
    break;

  case ADD:
    if (instr.rd == 0 && instr.rs == 1)
    {
      R0 = R0 + R1;
      instrText = "ADD R0,R1";
    }
    else if (instr.rd == 1 && instr.rs == 0)
    {
      R1 = R1 + R0;
      instrText = "ADD R1,R0";
    }
    break;

  case SUB:
    if (instr.rd == 0 && instr.rs == 1)
    {
      R0 = R0 - R1;
      instrText = "SUB R0,R1";
    }
    else if (instr.rd == 1 && instr.rs == 0)
    {
      R1 = R1 - R0;
      instrText = "SUB R1,R0";
    }
    break;
  }

  showState(instrText);
  PC++;
}

void setup()
{
  lcd.begin(16, 2);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(resetPin, INPUT_PULLUP);

  lcd.setCursor(0, 0);
  lcd.print("Mini CPU Ready");
  lcd.setCursor(0, 1);
  lcd.print("Press button");
}

void loop()
{
  buttonState = digitalRead(buttonPin);
  resetState = digitalRead(resetPin);

  // Step button
  if (lastButtonState == HIGH && buttonState == LOW)
  {
    stepCPU();
    delay(200);
  }

  // Reset button
  if (lastResetState == HIGH && resetState == LOW)
  {
    resetCPU();
    delay(200);
  }

  lastButtonState = buttonState;
  lastResetState = resetState;
}

Oled :

#include <LiquidCrystal.h>
#include <Wire.h>
#include <U8x8lib.h>

    // ---------------- LCD ----------------
    const int rs = 12,
              en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// ---------------- Buttons ----------------
const int cyclePin = 8;
const int confirmPin = 9;
const int resetPin = 6;

int cycleState = HIGH;
int lastCycleState = HIGH;

int confirmState = HIGH;
int lastConfirmState = HIGH;

int resetState = HIGH;
int lastResetState = HIGH;

// ---------------- OLEDs ----------------
// OLED 1: hardware I2C on A4/A5
U8X8_SSD1306_128X64_NONAME_HW_I2C oled1(U8X8_PIN_NONE);

// OLED 2: software I2C on A3/A2
U8X8_SSD1306_128X64_NONAME_SW_I2C oled2(/* clock=*/A3, /* data=*/A2, /* reset=*/U8X8_PIN_NONE);

// ---------------- ISA ----------------
enum Opcode
{
  OP_ADD,
  OP_SUB,
  OP_AND,
  OP_OR,
  OP_XOR
};

const char *opcodeNames[] = {
    "ADD",
    "SUB",
    "AND",
    "OR",
    "XOR"};

const int opcodeCount = 5;

// ---------------- State Machine ----------------
enum Mode
{
  SELECT_OPCODE,
  SELECT_R0,
  SELECT_R1,
  SHOW_RESULT
};

Mode currentMode = SELECT_OPCODE;

// ---------------- Current Selections ----------------
int opcodeIndex = 0;
int R0 = 0;
int R1 = 0;
int result = 0;

// ---------------- Functions ----------------
void showLCDStandby()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mini CPU Ready");
  lcd.setCursor(0, 1);
  lcd.print("Waiting input");
}

void showOLED1()
{
  oled1.clearDisplay();

  if (currentMode == SELECT_OPCODE)
  {
    oled1.drawString(0, 0, "Select Instr");
    oled1.drawString(0, 2, opcodeNames[opcodeIndex]);
    oled1.drawString(0, 4, "Cycle=Next");
    oled1.drawString(0, 5, "Confirm=Set");
  }
  else
  {
    oled1.drawString(0, 0, "Instruction");
    oled1.drawString(0, 2, opcodeNames[opcodeIndex]);
    oled1.drawString(0, 4, "Locked In");
  }
}

void showOLED2()
{
  oled2.clearDisplay();

  if (currentMode == SELECT_R0)
  {
    oled2.drawString(0, 0, "Set R0");
    oled2.setCursor(0, 2);
    oled2.print(R0);
    oled2.drawString(0, 4, "Cycle=0..10");
    oled2.drawString(0, 5, "Confirm=Set");
  }
  else if (currentMode == SELECT_R1)
  {
    oled2.drawString(0, 0, "Set R1");
    oled2.setCursor(0, 2);
    oled2.print(R1);
    oled2.drawString(0, 4, "Cycle=0..10");
    oled2.drawString(0, 5, "Confirm=Set");
  }
  else if (currentMode == SHOW_RESULT)
  {
    oled2.drawString(0, 0, "Inputs Set");
    oled2.setCursor(0, 2);
    oled2.print("R0=");
    oled2.print(R0);
    oled2.setCursor(0, 3);
    oled2.print("R1=");
    oled2.print(R1);
    oled2.drawString(0, 5, "Press reset");
  }
  else
  {
    oled2.drawString(0, 0, "Waiting...");
  }
}

void updateAllDisplays()
{
  showOLED1();
  showOLED2();

  if (currentMode != SHOW_RESULT)
  {
    showLCDStandby();
  }
}

void computeResult()
{
  switch (opcodeIndex)
  {
  case OP_ADD:
    result = R0 + R1;
    break;
  case OP_SUB:
    result = R0 - R1;
    break;
  case OP_AND:
    result = R0 & R1;
    break;
  case OP_OR:
    result = R0 | R1;
    break;
  case OP_XOR:
    result = R0 ^ R1;
    break;
  }
}

void showFinalOnLCD()
{
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(opcodeNames[opcodeIndex]);
  lcd.print(" ");
  lcd.print(R0);
  lcd.print(",");
  lcd.print(R1);

  lcd.setCursor(0, 1);
  lcd.print("RES:");
  lcd.print(result);
}

void resetSystem()
{
  opcodeIndex = 0;
  R0 = 0;
  R1 = 0;
  result = 0;
  currentMode = SELECT_OPCODE;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Reset");
  lcd.setCursor(0, 1);
  lcd.print("Choose instr");

  showOLED1();
  showOLED2();
  delay(400);
  updateAllDisplays();
}

void handleCyclePress()
{
  switch (currentMode)
  {
  case SELECT_OPCODE:
    opcodeIndex = (opcodeIndex + 1) % opcodeCount;
    break;

  case SELECT_R0:
    R0 = (R0 + 1) % 11; // 0 to 10
    break;

  case SELECT_R1:
    R1 = (R1 + 1) % 11; // 0 to 10
    break;

  case SHOW_RESULT:
    break;
  }

  updateAllDisplays();
}

void handleConfirmPress()
{
  switch (currentMode)
  {
  case SELECT_OPCODE:
    currentMode = SELECT_R0;
    break;

  case SELECT_R0:
    currentMode = SELECT_R1;
    break;

  case SELECT_R1:
    computeResult();
    currentMode = SHOW_RESULT;
    showFinalOnLCD();
    showOLED1();
    showOLED2();
    return;

  case SHOW_RESULT:
    break;
  }

  updateAllDisplays();
}

void setup()
{
  pinMode(cyclePin, INPUT_PULLUP);
  pinMode(confirmPin, INPUT_PULLUP);
  pinMode(resetPin, INPUT_PULLUP);

  lcd.begin(16, 2);

  oled1.begin();
  oled1.setPowerSave(0);
  oled1.setFont(u8x8_font_chroma48medium8_r);

  oled2.begin();
  oled2.setPowerSave(0);
  oled2.setFont(u8x8_font_chroma48medium8_r);

  updateAllDisplays();
}

void loop()
{
  cycleState = digitalRead(cyclePin);
  confirmState = digitalRead(confirmPin);
  resetState = digitalRead(resetPin);

  // Cycle button
  if (lastCycleState == HIGH && cycleState == LOW)
  {
    handleCyclePress();
    delay(180);
  }

  // Confirm button
  if (lastConfirmState == HIGH && confirmState == LOW)
  {
    handleConfirmPress();
    delay(180);
  }

  // Reset button
  if (lastResetState == HIGH && resetState == LOW)
  {
    resetSystem();
    delay(180);
  }

  lastCycleState = cycleState;
  lastConfirmState = confirmState;
  lastResetState = resetState;
}
