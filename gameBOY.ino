//Привет это сделал @sanya_viller с канала - https://www.youtube.com/channel/UC_kp6ysIv56mnSBo0TmS70w?view_as=subscriber
//Это прошивка для gameBOY`я.
//Меняй что хочешь☺ добавляй игры♥, но создай копию файла перед началом😋

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <time.h>
#include <string.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float y1;
float x1;
float Y2;
float X2;
byte d = 1;
byte number_of_games = 5; //кол-во игр
const char *games[] {//перечень всех игр по порядку
  "Flappy bird",
  "Snake",
  "Shooter",
  "Car physics",
  "Ping Pong",
};
byte game_s;

short r;
float k;
//вершины машины
float a_x;
float a_y;
float b_x;
float b_y;
float c_x;
float c_y;
float d_x;
float d_y;
//вершины машины
boolean level[5][5] = {
{0,0,1,0,0},
{0,0,0,0,0},
{1,0,1,0,1},
{0,0,0,0,0},
{0,0,1,0,0},
};
float dist = 0;
byte count_of_rays = 20; //кол-во лучей
byte number_of_ray = 0; //номер луча(для коректного отображения)
byte FOV = 45; //угол обзора
byte y_cursor = 28;
float speed = 1;
short Snake_X [32] = {random(0, 31 +1)*4};
short Snake_Y [32] = {random(0, 15 +1)*4};
//птица
const unsigned char BIRD [] = {
0x70, 0x88, 0x08, 0x0C, 0x0A, 0x92, 0x61, 0x01, 0x01, 0x01, 0x9D, 0x63, 0x41, 0x5A, 0x44, 0x78,
0x40, 0x80, 0x00, 0x00, 0x03, 0x05, 0x05, 0x08, 0x08, 0x08, 0x08, 
};
//птица

float X = 0;
float Y = 7;

void game_over(){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  if (d > 0){
    display.setCursor(10, 14);
    display.print("GAME OVER");
    display.setCursor(10, 36);
    display.print("Score: ");
    if (y_cursor == 20){
      display.print(d-3);
    }else{
      display.print(d);
    }
  }else{
    display.setCursor(10, 28);
    display.print("GAME OVER");
  }
  display.display();
  display.setTextSize(1);
  delay(1500);
  x1 = 50000;
}

void WIN(){ //победа☺
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(16, 28);
  display.print("YOU WIN!");
  display.display();
  display.setTextSize(1);
  delay(1500);
  x1 = 50000;
}

void Move(double Snake){
  if (!(Snake == 1)){
    if (digitalRead(2) == 0) {
      r = 2; //направо
    }
    if (digitalRead(5) == 0) {
      r = 5; //налево
    }
    if (digitalRead(4) == 0) {
      r = 4; //вверх
    }
    if (digitalRead(3) == 0) {
      r = 3; //вниз
    }
  } else {
    if (digitalRead(2) == 0 && r != 5) {
      r = 2; //направо
    }
    if (digitalRead(5) == 0 && r != 2) {
      r = 5; //налево
    }
    if (digitalRead(4) == 0 && r != 3) {
      r = 4; //вверх
    }
    if (digitalRead(3) == 0 && r != 4) {
      r = 3; //вниз
    }
  }
}

void rotate(double speed_of_rotate){
  if (digitalRead(2) == 0) {
      dist += speed_of_rotate; //направо
      if(game_s == 2){
        r += speed_of_rotate;
      }
    }
    if (digitalRead(5) == 0) {
      dist -= speed_of_rotate; //налево
      if(game_s == 2){
        r -= speed_of_rotate;
      }
    }
    //поставить ограничения на поворот
    if (r >= 360){
      r -= 360;
    }
    if (r < 0){
      r += 360;
    }
    //поставить ограничения на поворот
}

void go(double rotate, double move){
  X += move*cos(radians(rotate));
  Y += move*sin(radians(rotate));
}

void draw(String XD, double X0, double Y0, double Menu){
  display.setCursor(X0, Y0);
  if (Y0 == 32-4&&Menu == 1) {
    display.setTextColor(BLACK);
  } else {
    display.setTextColor(WHITE);
  }
  display.print(XD);
}

void menu(){ //Меню геймбоя
  y_cursor = 28;
  game_s = 0;
  display.setCursor(64-4*3, 28);// установка размера шрифта
  display.setTextColor(WHITE); // установка цвета текста
  display.clearDisplay();
  display.print("TECT");
  display.display();
  delay(1000);
  while (!(digitalRead(2) == 0 || digitalRead(2) == 0&&digitalRead(5) == 0)) {
    display.clearDisplay();
    if (digitalRead(4) == 0) {
    y_cursor += 8;
    game_s -= 1;
    }
    if (digitalRead(3) == 0) {
    y_cursor -= 8;
    game_s += 1;
    }
    if (y_cursor < 28-(8*(number_of_games-1))){
      y_cursor = 28;
      game_s = 0;
    }
    if (y_cursor > 28){
      y_cursor = 28-(8*(number_of_games-1));
      game_s = number_of_games-1;
    }
    display.fillRect(0, 32-4, 72, 8, WHITE);
    for(k = 0; k <= number_of_games-1; k ++){
      draw(games[int(k)], 4, y_cursor+(k*8), 1);
    }
    display.display();
    delay(125);
  }
  if (game_s == 0) {//flappy bird
    d = 0; //кол-во очков
    Y = 0; //положение птицы
    X = 0; //коэ-нт притяжения
    Snake_X[0] = 96;
    Snake_Y[0] = random(20, 46 +1);
    for(r = 1; r <= 3; r ++){
      Snake_X[r] = Snake_X[r-1] + random(42, 47 +1); //положения труб по Х
      Snake_Y[r] = random(20, 46 +1); //положения труб по Y
    }
    
  }
  if (game_s == 1){//Змейка
    r = 4; //направление змейки 4-вверх, 3-вниз, 2-направо, 5-влево
    d = 1; //длина змейки
    X = random(0, 31 +1)*4;//положение еды по ширине
    Y = random(0, 15 +1)*4;//положение еды по высоте
    Snake_X [d-1] = {random(4, 28 +1)*4};
    Snake_Y [d-1] = {random(4, 12 +1)*4};
  }
  if (game_s == 2){//Shooter
    r = 0; //направление
    X = 1;
    Y = 1;
    d = 2;
  }
  if (game_s == 3){
    r = 0;
    X = 64;//положение центра машины ро Х
    Y = 32;//положение центра машины ро Y
    x1 = 10; //Ширина машины
    y1 = 5; //Высота машины
  }
  if (game_s == 4){
    Y = 28;
    Y2 = 28;
    y1 = 32;
    x1 = 64;
    d = 0;
    k = 0;
    r = 125;
  }
}

void flappy_bird(){ 
  X -= 0.125;
  if (digitalRead(4) == 0){
    if(x1 == 0){
      X = 2;
      tone(11, 750, 40);
      tone(11, 1000, 20);
      tone(11, 750, 40);
    }
    x1 = 1;
  }else{
    x1 = 0;
  }
  Y -= X;
  display.fillRect(0, 0, 60, 8, WHITE);
  display.setCursor(0,0);
  display.setTextColor(BLACK);
  display.print("Score: ");
  display.print(d);
  display.drawBitmap(2,Y-6,BIRD,18,12,1);
  k = 18;//высота отверстия
  for(r = 0; r <= 2; r ++){
    display.fillRect(Snake_X[r]-2, 0, 4, Snake_Y[r]-k, WHITE);
    display.fillRect(Snake_X[r]-4, Snake_Y[r]-k-2, 8, 2, WHITE);
    display.fillRect(Snake_X[r]-2, Snake_Y[r]+k, 4, 128, WHITE);
    display.fillRect(Snake_X[r]-4, Snake_Y[r]+k, 8, 2, WHITE);
    Snake_X[r] -= 1;
    if (((Y - 6 <= Snake_Y[r] - k  ||  Y + 6 >= Snake_Y[r] + k)  &&  ((Snake_X[r] >= 2)  &&  (Snake_X[r] <= 20))) || Y >= 63){
      Snake_X[0, 31] = 0;
      Snake_Y[0, 31] = 0;
      game_over();
    }
    if(Snake_X[r] == 3){
      d += 1;
    }
    if(Snake_X[r] <= 0){
      Snake_X[r] = random(128, 136 +1);;
      Snake_Y[r] = random(16, 48 +1);
    }
  }
}

void Shooter(){
    rotate(5);
    //идти
    if (digitalRead(4) == 0) {
      go(r, 0.5);//вперёд
    }
    if (digitalRead(3) == 0) {
      go(r, -0.5);//назад
    }
    //идти

    //проверка на касание с окружностью
    if (level[int(Y)][int(X)]){
      if (digitalRead(4) == 0) {
        go(r, -0.5);//вперёд
      }
      if (digitalRead(3) == 0) {
        go(r, 0.5);//назад
      }
    }
    //проверка на касание с окружностью

    Snake_X[0] = r;
    r -= FOV/2;
    number_of_ray = 0;
    for (y1 = 0; y1 <= count_of_rays-1; y1 += 1){
      dist = 0;
      X2 = X;
      Y2 = Y;
      x1 = 0.01;
       if (r >= 360){
        r -= 360;
      }
      if (r < 0){
        r += 360;
      }
      while((level[int(Y2)][int(X2)] == 0)&&(X2<=4&&X2>=0&&Y2<=4&&Y2>=0)){
        x1 = 0.25;
        X2 += x1*cos(radians(r));
        Y2 += x1*sin(radians(r));
        dist += x1;
      }
      display.drawLine(128-(3*5)+(X*3), (Y*3), 128-(3*5)+(X2*3), (Y2*3), WHITE);
      display.fillRect(number_of_ray, 32-(1/dist*15), 135/count_of_rays, 1/dist*30, WHITE);
      number_of_ray += 135/count_of_rays;
      r += FOV/count_of_rays;
    }
    r = Snake_X[0] ;

    //нарисовать карту
    for (k=0;k<=4;k++){
      for (d=0;d<=4;d++){
        if (level[int(k)][int(d)]){
          display.fillRect(128-(3*5)+(d*3), (k*3), 3, 3, WHITE);
        }
      }
    }
    display.drawPixel(128-(3*5)+(X*3), (Y*3), WHITE);
    display.fillRect(0,0,18,26,BLACK);
    display.setCursor(0, 0);
    display.print(X);
    display.setCursor(0, 8);
    display.print(Y);
    display.setCursor(0, 16);
    display.print(r);
    //нарисовать карту
}

void ping_pong(){
  Snake_X[0] = -1;
  Snake_X[1] = 1;
  if(digitalRead(4) == 0){
    speed -= 1;
  }
  if(digitalRead(3) == 0){
    speed += 1;
  }
  Y += speed;
  if(Y <= 0){
    Y = 0;
  }
  if(Y+8 >= 63){
    Y = 55;
  }
  a_x += b_x*(r/100);
  a_y += b_y*(r/100);
  Y2 -= (Y2-a_y)/4;
  if(Y2 <= 0){
    Y2 = 0;
  }
  if(Y2+8 >= 63){
    Y2 = 55;
  }
  if((a_y>=Y&&a_y<=Y+8&&(a_x<=10&&x1>=4))||(a_y>=Y2&&a_y<=Y2+8&&(a_x<=123&&a_x>=119))){
    b_x /= -1;
    r += 25;
  }
  if(a_y-2<=0||a_y+2>=63){
    b_y /= -1;
  }
  if(a_x<=0){
    k += 1;
  }
  if(a_x>=127){
    d += 1;
  }
  if (a_x<=0||a_x>=127){
    a_x = 64;
    a_y = 32;
    b_x = Snake_X[random(0, 1 +1)];
    b_y = Snake_X[random(0, 1 +1)];
    r = 125;
  }
  display.setCursor(64-(2*4), 0);
  display.setTextColor(WHITE);
  display.print(d);
  display.print(":");
  display.print(int(k));
  display.fillCircle(a_x-2, a_y-2, 2, WHITE);
  display.fillRect(4, Y, 4, 8, WHITE);
  display.fillRect(119, Y2, 4, 8, WHITE);
  speed /= 4;
}

void car_physics(){//физика машинки
  rotate(5); //нужный поворот r, dist - поворот машины в данный момент
  r += dist;
  //поставить ограничения на поворот
  if (r >= 360){
    r -= 360;
  }
  if (r < 0){
    r += 360;
  }
  //поставить ограничения на поворот
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.println(r);
  display.print(speed);

  a_x = ((x1/(-2))*cos(radians(r)))-((y1/2)*sin(radians(r)));
  a_y = ((x1/(-2))*sin(radians(r)))+((y1/2)*cos(radians(r)));

  b_x = ((x1/2)*cos(radians(r)))-((y1/2)*sin(radians(r)));
  b_y = ((x1/2)*sin(radians(r)))+((y1/2)*cos(radians(r)));
  
  c_x = ((x1/(-2))*cos(radians(r)))-((y1/(-2))*sin(radians(r)));
  c_y = ((x1/(-2))*sin(radians(r)))+((y1/(-2))*cos(radians(r)));

  d_x = ((x1/2)*cos(radians(r)))-((y1/(-2))*sin(radians(r)));
  d_y = ((x1/2)*sin(radians(r)))+((y1/(-2))*cos(radians(r)));
  
  display.fillTriangle(X+a_x, Y+a_y, X+b_x, Y+b_y, X+c_x, Y+c_y, WHITE);
  display.fillTriangle(X+d_x, Y+d_y, X+b_x, Y+b_y, X+c_x, Y+c_y, WHITE);
  if(digitalRead(4) == 0){
    speed += 0.5;
  }
  if(digitalRead(3) == 0){
    speed -= 0.5;
  }
  speed /= 1.25;
  go(round(r), speed+abs(dist/8));
  dist /= 2;
}

void Snake(){ //Змейка
  //задание последнего направления
  Move(1);
  //задание последнего направления
    //изменение списка
    for (y1 = 0; y1 <= d-2; y1 += 1) {
        Snake_X [int(y1)] = Snake_X[int(y1)+1];
        Snake_Y [int(y1)] = Snake_Y[int(y1)+1];
    }
    
    display.fillRect(X-1, Y-1, 6, 6, WHITE);
    display.drawRect(X, Y, 4, 4, BLACK);
      if (Snake_X [d-1] == X && Snake_Y [d-1] == Y){
        Snake_X [d] = Snake_X [d-1];
        Snake_Y [d] = Snake_Y [d-1];
        d += 1;
        tone(11, 100, 10);
        X = random(0, 31 +1)*4;//положение еды по ширине
        Y = random(0, 15 +1)*4;//положение еды по высоте
      }
    //изменение списка
  
  if (r == 2) {
    Snake_X [d-1] += speed*4;
  }
  if (r == 5) {
    Snake_X [d-1] -= speed*4;
  }
  if (r == 4) {
    Snake_Y [d-1] -= speed*4;
  }
  if (r == 3) {
    Snake_Y [d-1] += speed*4;
  }
  //передвинуться

  //отрисовка змеи
  for (y1 = 0; y1 <= d-1; y1 += 1) {
    if (y1 == d-1){
      display.drawRect(Snake_X[int(y1)], Snake_Y[int(y1)], 4, 4, WHITE);
    } else {
      display.fillRect(Snake_X[int(y1)], Snake_Y[int(y1)], 4, 4, WHITE);
      if (Snake_X[int(y1)] == Snake_X[int(d)-1] && Snake_Y[int(y1)] == Snake_Y[int(d)-1]   || Snake_X[int(d)-1] < 0||Snake_X[int(d)-1] > 124 || Snake_Y[int(d)-1] < 0||Snake_Y[int(d)-1] > 60){ //касание с хвостом? или касание со стеной?
        game_over();
      }
    }
  }
  //отрисовка змеи
  if (d == 32){ //проверка на победу(максимальая длина хвоста 32 - размер списка)
    WIN();
  }
}

void setup()
{
  pinMode(2, INPUT);
  pinMode(5, INPUT);
  pinMode(4, INPUT);
  pinMode(3, INPUT);
  pinMode(11, INPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // инициализация дисплея по интерфейсу I2C, адрес 0x3C
  display.setTextSize(1);
  menu();
}

void loop()
{
  display.clearDisplay();
  if (!(digitalRead(2) == 0&&digitalRead(5) == 0||x1 == 50000)) {
    if (game_s == 0) {
      flappy_bird();
    }
    if (game_s == 1){
      Snake();
    }
    if (game_s == 2){
      Shooter();
    }
    if (game_s == 3){
      car_physics();
    }
    if(game_s == 4){
      ping_pong();
    }
  }else{
    menu();
  }
  display.invertDisplay(false);
  display.display();
  if (game_s == 1){
    if (!(digitalRead(2) == 0 or digitalRead(5) == 0 or digitalRead(4) == 0 or digitalRead(3) == 0)){
      delay((32-d)*(500/32));
    } else {
      delay((32-d)*(100/32));
    }
  }
}
