// ------------------- הגדרת פינים למנועים -------------------
const int IN1 = 2;  // מנוע של הזרוע
const int IN2 = 3;   // מנוע של הזרוע
const int ENA = 4;

const int IN5 = 10;  // מנוע 3 - קדימה
const int IN6 = 9;   // מנוע 3 - אחורה
const int ENC = 8;   // בקרת מהירות מנוע 3 (PWM)

const int IN7 = 13;  // מנוע 4 - קדימה
const int IN8 = 12;  // מנוע 4 - אחורה
const int END = 11;  // בקרת מהירות מנוע 4 (PWM)

// ------------------- חיישן אולטרסוני (HC-SR04) -------------------
#define TRIG_PIN A2
#define ECHO_PIN A3
#define SAFE_DISTANCE 50  // מרחק בטוח מינימלי (ס"מ)

// ------------------- הצהרות לפונקציות -------------------
float getDistance();
void stopMotors();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();

void setup() {
    Serial.begin(9600);
    
    // הגדרת כל הפינים כיציאות
    pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
    pinMode(IN5, OUTPUT); pinMode(IN6, OUTPUT); pinMode(ENC, OUTPUT);
    pinMode(IN7, OUTPUT); pinMode(IN8, OUTPUT); pinMode(END, OUTPUT); pinMode(ENA, OUTPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    
    stopMotors();  // התחלה עם מנועים כבויים
}

// ------------------- פונקציה לקריאת המרחק מהחיישן -------------------
float getDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    float distance = duration * 0.034 / 2;  // חישוב מרחק בס"מ

    if (duration == 0) {  
        return -1;  // אם אין החזרה מהאולטרסוניק
    }

    return distance;
}

// ------------------- פונקציות לשליטה במנועים -------------------
void moveForward() {
    Serial.println(" תנועה קדימה");
    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
    digitalWrite(IN5, LOW); digitalWrite(IN6, HIGH);
    digitalWrite(IN7, LOW); digitalWrite(IN8, HIGH);
    analogWrite(ENC, 255); analogWrite(END, 255);analogWrite(ENA, 0);
}

void moveBackward() {
    Serial.println("תנועה אחורה");
    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
    digitalWrite(IN5, HIGH); digitalWrite(IN6, LOW);
    digitalWrite(IN7, HIGH); digitalWrite(IN8, LOW);
    analogWrite(ENC, 255); analogWrite(END, 255);analogWrite(ENA, 0);
}

void turnLeft() {
    Serial.println(" פנייה שמאלה");
    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
    digitalWrite(IN5, LOW); digitalWrite(IN6, HIGH);
    digitalWrite(IN7, HIGH); digitalWrite(IN8, LOW);
    analogWrite(ENC, 100); analogWrite(END, 100);analogWrite(ENA, 0);
}

void turnRight() {
    Serial.println(" פנייה ימינה");
    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
    digitalWrite(IN5, HIGH); digitalWrite(IN6, LOW);
    digitalWrite(IN7, LOW); digitalWrite(IN8, HIGH);
    analogWrite(ENC, 100); analogWrite(END, 100);analogWrite(ENA, 0);
}

void stopMotors() {
    Serial.println(" עצירת מנועים");
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
    digitalWrite(IN5, LOW); digitalWrite(IN6, LOW);
    digitalWrite(IN7, LOW); digitalWrite(IN8, LOW);
    analogWrite(ENC, 0); analogWrite(END, 0);analogWrite(ENA, 255);
}

/*void scare() {
    Serial.println(" הפעלת הזרוע");
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
}*/

// ------------------- לולאת התוכנית הראשית -------------------
float lastDistance = -1;  

void loop() {
    // קריאת המרחק מהחיישן
    float distance = getDistance();

    // שליחת המרחק רק אם הוא השתנה ביותר מ-5 ס"מ או קטן מ-50 ס"מ
    if (distance < SAFE_DISTANCE) {  
        Serial.print("DISTANCE: ");
        Serial.println(distance);
        lastDistance = distance;  
    }

    // עצירה אוטומטית אם המרחק קטן מ-50 ס"מ
    if (distance > 0 && distance < SAFE_DISTANCE) {
        Serial.println("⚠ קרוב מדי - עצירה!");
        stopMotors();
        delay(500);
    } 
    
    // אם יש פקודה חדשה מהפייתון והמרחק בטוח
    else if (Serial.available() > 0 && distance >= SAFE_DISTANCE) {
        String command = Serial.readStringUntil('\n');
        command.trim();

        Serial.print("COMMAND: ");
        Serial.println(command);

        if (command == "FORWARD") {
            moveForward();
        } 
        else if (command == "BACKWARD") {
            moveBackward();
        }
        else if (command == "LEFT") {
            turnLeft();
        }
        else if (command == "RIGHT") {
            turnRight();
        }
        else if (command == "STOP") {
            stopMotors();
        }
    }

    delay(100);  // מניעת קריאות יתר
}
