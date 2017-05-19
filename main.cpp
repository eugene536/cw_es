#include <Arduino.h>
#include <SoftwareSerial.h>

int d = 1000;
int const cnt_symb = 'Z' - 'A';
int cnt_true = 0;
int cnt_runs = 0;
int max_runs = 2; // must be < 10, :(
char cur_symb = 'A';

enum {
    e_bt_tx = 10,
    e_bt_rx = 11,

    e_lcd_rs = 9,
    e_lcd_e = 8,

    e_lcd_d5 = 4,
    e_lcd_d4 = 5,
    e_lcd_d3 = 6,
    e_lcd_d2 = 7
};

struct Lcd {
    void clear() {
        command(0, 0, 0, 0);
        command(0, 0, 0, 1);
    }

    void print(char s) {
        send(s);
    }
     
    void print(const char *s) {
        unsigned int sz = strlen(s);
        for (unsigned int i = 0; i < sz; i++) {
            send(s[i]);
        }
    }
     
    void print(int val) {
        print(char('0' + val));
    }

    void setup_outputs() {
        pinMode(e_lcd_rs, OUTPUT);
        pinMode(e_lcd_e, OUTPUT);
        pinMode(e_lcd_d5, OUTPUT);
        pinMode(e_lcd_d4, OUTPUT);
        pinMode(e_lcd_d3, OUTPUT);
        pinMode(e_lcd_d2, OUTPUT);
        delay(50);

        setup_bit_capacity();

        // setup parameters
        command(0, 0, 1, 0);
        command(1, 0, 0, 0);

        // turn on display
        command(0, 0, 0, 0);
        command(1, 1, 0, 0);

        clear();

        // set output mode
        command(0, 0, 0, 0);
        command(0, 1, 1, 0);
    }

private:
    void write4(int DB7, int DB6, int DB5, int DB4) {
      digitalWrite(e_lcd_d2, DB7);
      digitalWrite(e_lcd_d3, DB6);
      digitalWrite(e_lcd_d4, DB5);
      digitalWrite(e_lcd_d5, DB4);

      // pulse
      digitalWrite(e_lcd_e, LOW);
      delayMicroseconds(1);    
      digitalWrite(e_lcd_e, HIGH);
      delayMicroseconds(1); 
      digitalWrite(e_lcd_e, LOW);
      delayMicroseconds(2000);
    }

    void write4(int x) {
        write4(x >> 3, (x >> 2) & 1, (x >> 1) & 1, x & 1);
    }

    void command(int DB7, int DB6, int DB5, int DB4) {
      digitalWrite(e_lcd_rs, LOW);

      write4(DB7, DB6, DB5, DB4);
    }

    void send(uint8_t value) {
      digitalWrite(e_lcd_rs, HIGH);

      write4(value >> 4);
      write4(value & 0xf);
    }
 
    void setup_bit_capacity() {
        for (int i = 0; i < 3; ++i) {
            command(0, 0, 1, 1);
            delayMicroseconds(4500);
        }
        command(0, 0, 1, 0);
        delayMicroseconds(4500);
    }
};

SoftwareSerial BT(e_bt_tx, e_bt_rx); 
Lcd lcd;
//LiquidCrystal lcd(e_lcd_rs, e_lcd_e, e_lcd_d5, e_lcd_d4, e_lcd_d3, e_lcd_d2);

void blink(int out) {
    for (int i = 0; i < 2; ++i) {
      digitalWrite(out, HIGH);
      delay(100);         
      digitalWrite(out, LOW); 
      delay(100);
    }
}

void showSymbol() {
    cur_symb = 'A' + random(0, cnt_symb);
    //lcd.setCursor(0,0);
    lcd.clear();
    lcd.print(cur_symb);
}

void setup() {
    BT.begin(9600);
    BT.println("Hello from Arduino");
    randomSeed(analogRead(0));
    
    //lcd.begin(8, 2);
     
    lcd.setup_outputs();
    showSymbol();
}

void loop() {
    if (cnt_runs >= max_runs) {
        //lcd.setCursor(0,0);
        lcd.clear();

        lcd.print(cnt_true);
        lcd.print(" / ");
        lcd.print(max_runs);

        delay(3000);

        showSymbol();
        cnt_runs = 0;
        cnt_true = 0;
    }

    if (BT.available()) {
        char answ = BT.read();

        if (toupper(answ) == cur_symb) {
            cnt_true++;

            showSymbol();
            BT.println("true");
        } else {
            BT.println("false");
        }

        cnt_runs++;
    }
}

int main() {
    init();

    setup();

    for (;;) {
        loop();
    }

    return 0;
}
