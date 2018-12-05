#include <Ethernet.h>
#include <SoftwareSerial.h>
#include <PI_TI.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <LiquidCrystal_I2C.h>

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//IPAddress server_addr(189, 14, 200, 70);
IPAddress server_addr(192, 168, 77, 199);
//char mac[] = "12217961780";
//char user[] = "grupo4";
//char password[] = "5QrinX1I4d";
char user[] = "root";
char password[] = "swordfish";
EthernetClient client;
const char banco[] = "call zadmin_banco.G4_CHAMADA(%s,'80-D2-D0-0A-2F-DF')";
const char saida[] = "select status from zadmin_banco.G4_RETORNO_DISPLAY";
//char cartao[15];
char query[60];
MySQL_Connection conn((Client *)&client);
SoftwareSerial mySerial(7, 20);  
RFID sensorAcesso(mySerial);
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE);

void setup() {
  lcd.begin (16,2);
  Serial.begin(9600);
  while (!Serial){;}
  Ethernet.begin(mac_addr);
  Serial.println(F("Connecting..."));
  if (conn.connect(server_addr, 3306, user, password)) {
    Serial.println(F("Connected."));
  }
  else{
    Serial.println(F("Failed."));
  }
  mySerial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
}

void loop() {
  
  sensorAcesso.run();
  lcd.setCursor(0,0);
  lcd.print("SALA: BL T 13");
  lcd.setCursor(0,1);
  lcd.print("Passe o Cartao");
  delay(0100);
  lcd.clear();
  if(sensorAcesso.codePronto()){
    //String rfid = sensorAcesso.getCodeDecimal();
    //rfid.toCharArray(cartao,15);
    sprintf(query, banco, sensorAcesso.getCodeDecimal());
    Serial.println(query);
    chamada();
  }
}

void chamada(){
    String retorno, v1, v2;
    int a;
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    cur_mem->execute(query);
    cur_mem->execute(saida);
    // Fetch the columns and print them
    column_names *cols = cur_mem->get_columns();
    for (int f = 0; f < cols->num_fields; f++) {
    Serial.print(cols->fields[f]->name);
    if (f < cols->num_fields-1) {
      Serial.print(',');
    }
    }
    Serial.println();
    // Read the rows and print them
    row_values *row = NULL;
      do {
        row = cur_mem->get_next_row();
        if (row != NULL) {
          for (int f = 0; f < cols->num_fields; f++) {
            retorno += row->values[f];
            Serial.print(row->values[f]);
            if (f < cols->num_fields-1) {
              retorno += ',';
              Serial.print(',');
            }
          }
          Serial.println();
        }
      } while (row != NULL);
    // Deleting the cursor also frees up memory used
    delete cur_mem;
    
    a = retorno.indexOf(',');
    v1 = retorno.substring(0,a);
    v2 = retorno.substring(a+1, retorno.length());

    Serial.println(F("Realizando consulta no banco e exibindo resultado no display!"));
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(v1);
    lcd.setCursor(0,1);
    lcd.print(v2);
    delay(3000);
}

